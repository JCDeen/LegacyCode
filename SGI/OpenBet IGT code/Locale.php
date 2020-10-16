<?
// BitKeeper: lzhu@ralph.gamelogic.com|php/lib/Models/Reveal/Locale.php|20160909193513|06563
// Copyright (C) 2009 GameLogic, Inc. All Rights Reserved.

require_once 'Models/Storeable.php';

class Locale extends Storeable
{
  /// \brief construct this storeable
  /// \param Storeable a stdClass or associative array 
  public function __construct( $Storeable = array() )
  {
    parent::__construct( $Storeable );

    $this->mRelations = 
      array(
	    Storeable::HasMany => array( 'Description' ),
	    );
  }

  public static function mFind( $Params = array() )
  { return parent::mFind( get_class(), $Params ); }
  
  public static function mFindByCode( $Code, $Options = array() )
  {
    $Criteria[] = StoreableCriteria::equal( 'locale.localecode', $Code );  
    $Params = array_merge( $Options, array( 'Criteria' => $Criteria ) );
    $Locale = self::mFind( $Params );
    if( $Locale && count( $Locale ) == 1 )
    { return $Locale[0]; }
    return null;
  }
  
}