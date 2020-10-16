<?
// BitKeeper: jdeen@rex.gamelogic.com|php/lib/Models/Reveal/Description.php|20170909193518|46387
// Copyright (C) 2016 GameLogic, Inc. All Rights Reserved.

require_once 'Models/Storeable.php';
require_once 'Models/Reveal/Locale.php';

class Description extends Storeable
{
  static $DbTable = array(
   eDescriptionContextAction => array( "action","name","displayname" ),
   eDescriptionContextSweepstakes => array( "sweepstakesschedule","name","displayas" ),
   eDescriptionContextJackpot => array("jackpot","name","webdescription" ),
   eDescriptionContextEitGame => array("digitalinstantgame","gamename","gamename" ),
   eDescriptionContextPrize => array( "prize","prizename","prizename" ),
   eDescriptionContextPromotion => array( "promotion","prizename","prizedescription" ),
   eDescriptionContextGamePieceToken => array( "gamepiecetoken","name","name" ),
   eDescriptionContextGamePieceGame => array( "gamepiecegame", "name", "name" ),
   eDescriptionContextWebCodeRun => array( "webcoderun","description","description" ),
   eDescriptionContextSurvey => array( "survey","shortdescription","longdescription" ),
   eDescriptionContextSecurityQuestion => array( "securityquestion","questiontext", "questiontext" ),
   eDescriptionContextTermsOfService => array( "termsofservice","tostext", "tostext" )
  );
  
  /// \brief construct this storeable
  /// \param Storeable a stdClass or associative array 
  public function __construct( $Storeable = array() )
  {
    parent::__construct( $Storeable );

    $this->mRelations = 
      array(
	    Storeable::HasOne => array( 'Locale' ),
	    );
  }

  public static function mFind( $Params = array() )
  { return parent::mFind( get_class(), $Params ); }
  
  // valid $Context values are:
  // eDescriptionContextAction
  // eDescriptionContextSweepstakes
  // eDescriptionContextJackpot
  // eDescriptionContextEitGame
  // eDescriptionContextPrize
  // eDescriptionContextPromotion
  // eDescriptionContextGamePieceToken
  // eDescriptionContextGamePieceGame
  // eDescriptionContextWebCodeRun
  // eDescriptionContextSurvey
  // eDescriptionContextSecurityQuestion
  // eDescriptionContextTermsOfService;
  // note the $ObjectId must be the id of an object of the given context.
  // These two are the getters you want to use:
  public static function mFindLocalizedShortDescriptionFor( $ObjectId, $Context, $LocaleCode, $Options = array() )
  { return mFindLocalizedDescriptionFor( $ObjectId, $Context, $LocaleCode, 
     eDescriptionTypeShort, $Options ); }
 
  public static function mFindLocalizedLongDescriptionFor( $ObjectId, $Context, $LocaleCode, $Options = array() )
  { return mFindLocalizedDescriptionFor( $ObjectId, $Context, $LocaleCode, 
     eDescriptionTypeLong, $Options ); }

  // this is internal   
  private static function mFindLocalizedDescriptionFor( $ObjectId, $Context, $LocaleCode, $Type, $Options = array() )
  {
      $Locale = Locale::mFindByCode( $LocaleCode );
      if( ! $Locale )
      { return null; }
      $Description = self::mFindLocalizedDescriptionWithId( $ObjectId, $Context, $Locale->localeid, $Type, $Options );
      if( $Description )
      { return $Description->description; }
      return null;
  }
  
  private static function mFindLocalizedDescriptionWithId( $ObjectId, $Context, $LocaleId, $Type, $Options = array() )
  {
      $Params = array_merge( $Options, array( 
       StoreableCriteria::equal( 'description.contextid', $ObjectId ),
       StoreableCriteria::equal( 'description.context', $Context ),
       StoreableCriteria::equal( 'description.localeid', $LocaleId ),
       StoreableCriteria::equal( 'description.type', $Type ) ) );
      $Description = self::mFind( $Params );
      if( $Description && count( $Description ) )
      { return $Description[0]; }
      return null;
  }
  
  // experimental: multi-row query with localization strings
  private static function mFindLocalizedQueryWithId( $Criteria, $Context, $LocaleId, $Options = array() )
  {
      $Table = self::$DbTable[ $Context ];
      $Sql = new SqlExecute();
      $Sql->select( "*", $Table[0] . 
        " outer join locale outer join description",
        array( 'conditions' => array( 
         "description.contextid=".$Table[0].".".$Table[0]."id",
         "description.context=".$Context,
         "locale.localeid=description.localeid" ) ) );
      $Result = $Sql->execute();
      return $Result;
  }
  
  // these two are the setters you want to use:
  public static function mSetLocalizedShortDescriptionFor( $ObjectId, $Context, $LocaleCode, $DescriptionText, $Options = array() )
  { return mSetLocalizedDescriptionFor( $ObjectId, $Context, $LocaleCode, 
     eDescriptionTypeShort, $DescriptionText, $Options ); }
 
  public static function mSetLocalizedLongDescriptionFor( $ObjectId, $Context, $LocaleCode, $DescriptionText $Options = array() )
  { return mSetLocalizedDescriptionFor( $ObjectId, $Context, $LocaleCode, 
     eDescriptionTypeLong, $DescriptionText, $Options ); }
     
  
  private static function mSetLocalizedDescriptionFor( $ObjectId, $Context, $LocaleCode, $Type, $DescriptionText, $Options = array() )
  {
      $Locale = Locale::mFindByCode( $LocaleCode );
      if( ! $Locale )
      { 
        error_log( "Bad set localized description, locale: ".$LocaleCode );
        return false; 
      }
      $Table = self::$DbTable[ $Context ];
      if( ! $Table )
      {
        error_log( "Bad context in set localized description: ".$Context );
        return false;
      }
      
      // check to see that the $ObjectId is in fact an object of type $Context
      $Chk = new SqlExecute();
      $Chk->select( array( $Table[0]."id" ), $Table[0],
        array( 'conditions' => array( $Table[0]."id = ".$ObjectId ) ) );
      $ChkResult = $Chk->execute();
      if( $SqlResult['Status'] != cStatusOk )
      {
        error_log( "set localized description: unable to find ".$ObjectId." in ".$Table[0] );
        return false;
      }
      
      $Sql = new SqlExecute();
      // see if the description already exists
      $Result = self::mFindLocalizedDescriptionWithId( $ObjectId, $Context, $Locale->localeid, $Type, $Options );
      if( ! $Result || ! count( $Result ) )
      {
        $Sql->insert( array( 
           "description" => $DescriptionText,
           "type" => $Type,
           "context" => $Context,
           "contextid" => $ObjectId,
           "localelid" => $Locale->localeid ), "description" );
      }
      else
      {
        $Sql->update( array( 
           "description" => $DescriptionText ), 
           "description",
           array( 'conditions' => array ( 
            "descriptionid = ".$Result[0]->descriptionid ) ) );
      }
      $SqlResult = $Sql->execute( );
      return( $SqlResult['Status'] == cStatusOk );
  }    
  
}