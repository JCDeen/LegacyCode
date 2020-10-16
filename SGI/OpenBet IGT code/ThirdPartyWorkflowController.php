<?php
require_once 'Controllers/ApiController.php';
require_once 'Helper.php';
require_once 'Router.php';
require_once 'Models/Money.php';
require_once 'Models/Player.php';
require_once 'Models/Lottery/CurrencyTransaction.php';
require_once 'Models/AgeVerification.php';
require_once 'Models/Ecommerce/ThirdPartyWorkflow.php';

class ThirdPartyWorkflowController extends ApiController
{
  protected $Player;

  public function __construct( &$DATA = array() )
  {
    $DATA['PlayerRequired'] = FALSE;
    $DATA['SignatureRequired'] = FALSE;
    parent::__construct( $DATA );
  }

  public function purchase( &$DATA )
  {
    $this->logRequest( "ThirdParty Purchase" );
    $DATA[ 'PlayerCardId' ] = $this->getPlayerCardId();

    if( empty( $DATA[ 'PlayerCardId' ] ) ||
        empty( $DATA[ 'Items' ] ) ||
        empty( $DATA[ 'AuditData' ] ) ||
        empty( $DATA[ 'AuditData' ][ 'Geolocation' ] ) ||
        ! is_array( $DATA[ 'AuditData' ][ 'Geolocation' ] ) ||
        empty( $DATA[ 'PartnerTimestamp' ] ) ||
        empty( $DATA[ 'PartnerPurchaseId' ] ) )
    { $this->apiFailure( 400, cStatusInvalidRequest ); }

    // Check if the registration form has an override check
    $GeoLocationRequired = intval( (string)Helper::contentNode( array( 'api', 'lottocart', 'geolocationrequired' ) ) );
    if( $GeoLocationRequired )
    {
      $this->requireGeoLocation( eGeoLocationRequestTypeWager,
          $DATA['AuditData']['Geolocation']['IpAddress'],
          $DATA['AuditData']['Geolocation'] );
    }

    $DATA[ 'AuditData' ][ 'Referrer' ] = $this->TokenData[ 'Referrer' ];
    $DATA[ 'AuditData' ][ 'SessionId' ] = $this->TokenData[ 'SessionId' ];
    try
    {
      $this->getPlayer();
      $CanDo = $this->Player->mIlotteryCanDo();
      switch ( $CanDo )
      {
        case IlotteryCanDo::cCanDo:
          break;
        case IlotteryCanDo::cAccountExcluded:
        case IlotteryCanDo::cSelfExcluded:
          throw new MustAccountExcludeExpire( "accountexcluded" );
          return;
        case IlotteryCanDo::cNotAgeVerified:
          throw new MustVerifyAgeException( "agenotverified" );
          break;
        default:
          error_log( "unhandled mIlotteryCanDo state: $CanDo" );
          throw new Exception( "unhandled mIlotteryCanDo state: $CanDo" );
      }
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->reserve( $DATA );

      if( $Result[ 'Status' ] == cStatusOk )
      {
        unset( $Result[ 'Status' ] );
        $this->smartTemplate( array(), $Result );
      }
      else if( $Result[ 'Status' ] == cStatusPurchaseConflict ) 
      { $this->apiFailure( 409, "Purchase already exists" ); }
      else if( $Result[ 'Status' ] == cStatusInsufficientFunds )
      { $this->apiFailure( 560, "InsufficientFunds" ); }
      else if( $Result[ 'Status' ] == cStatusWagerLimitExceeded )
      { $this->apiFailure( 560, "ResponsibleGamingLimits" ); }
      else if( $Result[ 'Status' ] == cStatusEcommerceWalletSystemError )
      { $this->apiFailure( 404, "The purchase has already been voided" ); }
      else 
      { $this->apiFailure( 560, $Result[ 'Status' ] ); }
    }
    catch( MustVerifyAgeException $E )
    {
      error_log( "Player is not verified: ".$E->getMessage() );
      $this->apiFailure( 401, cStatusValidationError );
    }
    catch( MustAccountExcludeExpire $E )
    {
      error_log( "Player Account excluded: ".$E->getMessage() );
      $this->apiFailure( 401, cStatusPlayerExcluded );
    }
    catch( Exception $E )
    {
      error_log( "Exceptions occurred while processing thirdparty purchase: ".$E->getMessage() );
      $this->apiFailure( 500, cStatusServerError );
    }
  }

  public function purchaseinfo( &$DATA )
  {
    try
    {
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->purchaseinfo( $DATA );
      if( ! $Result )
      { $this->apiFailure( 404, 'Purchase was not found' ); }
      else
      { $this->smartTemplate( array(), $Result ); }
    }
    catch( Exception $E )
    {
      error_log( "Exceptions occurred while processing thirdparty purchaseinfo: ".$E->getMessage() );
      $this->apiFailure( 500, "SystemError" );
    }
  }

  public function commit( &$DATA )
  {
    if( empty( $DATA[ 'PartnerPurchaseId' ] ) ||
        empty( $DATA[ 'CommitTimestamp' ] ) )
    { $this->apiFailure( 400, cStatusInvalidRequest ); }
 
    try
    {
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->commit( $DATA );

      if( $Result[ 'Status' ] == cStatusOk )
      {
        unset( $Result[ 'Status' ] );
        $this->smartTemplate( array(), $Result );
      }
      else if( $Result[ 'Status' ] == cStatusPurchaseNotFound ) 
      { $this->apiFailure( 404, "Purchase was not found" ); }
      else if( $Result[ 'Status' ] == cStatusPurchaseConflict ) 
      { $this->apiFailure( 409, "Purchase was not in pending state" ); }
      else
      { $this->apiFailure( 500, "SystemError" ); }
    }
    catch( Exception $E )
    {
      error_log( "Exceptions occurred while processing thirdparty purchase commit: ".$E->getMessage() );
      $this->apiFailure( 500, "SystemError" );
    }
  }

  public function deletepurchase( &$DATA )
  {
    if( empty( $DATA[ 'PartnerPurchaseId' ] ) ||
        empty( $DATA[ 'PartnerCancelId' ] ) )
    { $this->apiFailure( 400, cStatusInvalidRequest ); }

    try
    {
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->rollback( $DATA );

      if( $Result[ 'Status' ] == cStatusOk )
      {
        unset( $Result[ 'Status' ] );
        $this->smartTemplate( array(), $Result );
      }
      else if( $Result[ 'Status' ] == cStatusPurchaseNotFound ) 
      { $this->apiFailure( 404, "Purchase was not found" ); }
      else if( $Result[ 'Status' ] == cStatusPurchaseConflict ) 
      { $this->apiFailure( 409, "Purchase was not in pending state" ); }
      else 
      { $this->apiFailure( 500, "SystemError" ); }
    }
    catch( Exception $E )
    {
      error_log( "Exceptions occurred while processing thirdparty purchase rollback: ".$E->getMessage() );
      $this->apiFailure( 500, "SystemError" );
    }
  }

  public function refundpurchase( &$DATA )
  {
    $this->logRequest( "ThirdParty Refund" );
    if( empty( $DATA[ 'PartnerPurchaseId' ] ) ||
        empty( $DATA[ 'PartnerTimestamp' ] ) ||
        empty( $DATA[ 'PartnerTransactionId' ] ) ||
        empty( $DATA[ 'AuditData' ] ) )
    { $this->apiFailure( 560, cStatusInvalidRequest ); }
 
    try
    {
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->refundPurchase( $DATA );

      if( $Result[ 'Status' ] == cStatusOk )
      {
        unset( $Result[ 'Status' ] );
        $this->smartTemplate( array(), $Result );
      }
      else if( $Result[ 'Status' ] == cStatusPurchaseNotFound ) 
      { $this->apiFailure( 404, "PartnerPurchaseId was not found in system" ); }
      else if( $Result[ 'Status' ] == cStatusPurchaseConflict ) 
      { $this->apiFailure( 409, "Already Refunded" ); }
      else
      { $this->apiFailure( 560, $Result[ 'Status' ] ); }
    }
    catch( Exception $E )
    {
      error_log( "Exceptions occurred while processing thirdparty purchase commit: ".$E->getMessage() );
      $this->apiFailure( 500, "SystemError" );
    }
  }

  public function reportwin( &$DATA )
  {
    $this->logRequest( "ThirdParty Win" );
    if( empty( $DATA[ 'PartnerTimestamp' ] ) ||
        empty( $DATA[ 'PartnerPurchaseId' ] ) ||
        ! isset( $DATA[ 'Amount' ] ) ||
        floatval( $DATA[ 'Amount' ] ) < 0 )
    { $this->apiFailure( 400, cStatusInvalidRequest ); }    
  
    try
    {
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->reportwin( $DATA ); 

      if( $Result[ 'Status' ] == cStatusOk )
      {
        Header( "Http/1.1 200 OK" );
        $Response = array(
          "PlayerBalance" => number_format( $Result[ 'CashBalance' ], 2, '.', '' )
          );
        $this->smartTemplate( array(), $Response );
      }
      else if( $Result[ 'Status' ] == cStatusPurchaseNotFound )
      { $this->apiFailure( 404, "Purchase was not found" ); }
      else if( $Result[ 'Status' ] == cStatusTicketRedeemed )
      { $this->apiFailure( 409, "Duplicate winner payment - PartnerPurchaseId was already paid." ); }
      else 
      { $this->apiFailure( 500, "SystemError" ); }
    }
    catch( Exception $E )
    {
      error_log( "Exceptions occurred while processing win: ".$E->getMessage() );
      $this->apiFailure( 500, "SystemError" );
    }
  }

  public function getbingopurchasedetails( &$DATA )
  {
    $ExceptionPrefix = "getbingopurchasedetails()";
    
    if( empty( $DATA[ 'PurchaseId' ] ) )
    {  $this->apiFailure( 404, cStatusValidationError, NULL, array( 'PurchaseId' => 'Missing' ) );  }
    
    $LoginPlayerCardId = $this->getPlayerCardId(); 

    $Query = "Select playercardid, partnertransactionid, thirdpartypurchaseitem.state as winstatus  " .
        "from purchase, purchaseitem, thirdpartypurchaseitem " .
        "where purchaseitem.purchaseid = {$DATA[ 'PurchaseId' ]} AND " .
        "purchase.purchaseid = {$DATA[ 'PurchaseId' ] } AND " .
        "thirdpartypurchaseitem.purchaseitemid = purchaseitem.purchaseitemid";
    
    $Sql = new SqlExecute(array( 'Verbose' => 0 ) );
    $Sql->sql( $Query );
    $SqlResult = $Sql->execute( array( 'Run' => Helper::GetRunId() ) );
    
    if( ( $SqlResult[ 'Status' ] != cStatusOk ) || ( $SqlResult[ 'NumRows' ] != 1 ) )
    { 
      return $this->apiFailure( 404, 
                                cStatusValidationError, 
                                null, 
                                array( 'PurchaseId' => 'Not Found' ) ); 
    }
    
    $PurchasePlayerCardId = $SqlResult['Rows']['0']->purchase->playercardid;
    $ThirdPartyPurchaseItemObj = $SqlResult['Rows']['0']->thirdpartypurchaseitem;
    $TransactionId = (string) $ThirdPartyPurchaseItemObj->partnertransactionid;
    
    if ( $PurchasePlayerCardId != $LoginPlayerCardId )
    {
      $Summary = "PurchasePlayerCardId $PurchasePlayerCardId != LoginPlayerCardId $LoginPlayerCardId";  
      error_log( "$ExceptionPrefix $Summary" );
      
      $this->apiFailure( 404, 
                         cStatusValidationError, 
                         NULL, 
                         array( 'LoginPlayerCardId' => 'Does Not Match PurchasePlayerCardId',
                                'Summary' => $Summary ) );
    }
            
    try
    {
      $WinStatus = Helper::MapTPPIStateToWinStatus( $ThirdPartyPurchaseItemObj->winstatus );
        
      // Authentication token is held by ApiController.
      $PlayerAuthToken = $this->getAuthToken();
      $ThirdParty = new IGTConnector();
      $Result = $ThirdParty->getTransactionDetails( $PurchasePlayerCardId, $PlayerAuthToken, $TransactionId, $WinStatus );
      $this->smartTemplate( array(), $Result );
    }
    catch( Exception $E )
    {
      $IGTStatusCode = $E->getStatus();
      $RetStatusCode = ( $IGTStatusCode >= 400 && $IGTStatusCode < 600 ) ? $IGTStatusCode : 500;
      $RetStatusMSg = ( $IGTStatusCode >= 400 && $IGTStatusCode < 600 ) ? $E->getMessage() : cStatusSystemError;
      
      error_log( "$ExceptionPrefix Exceptions occurred: " . $E );
      $this->apiFailure( $RetStatusCode, $RetStatusMSg );
    }
  }

  // TODO:
  //    Will add a generic getpurchasedetails function to reduce
  //    amount of code when finalizing Bingo details work.
  public function getsportspurchasedetails( &$DATA )
  {
    $ExceptionPrefix = "getsportspurchasedetails() exception: ";
    
    if( empty( $DATA[ 'PurchaseId' ] ) )
    {  $this->apiFailure( 404, "Invalid PlayerCardId or PurchaseId" );  }
    
    try
    {
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->getsportspurchasedetails( $DATA );
      $this->smartTemplate( array(), $Result );
    }
    catch( Exception $E )
    {
      if ($E->getStatus() == cStatusInvalidRequest)
      {
        error_log( $ExceptionPrefix . $E->getMessage() );
        $this->apiFailure( 400, "Bad Request" );
      }
      else if ($E->getStatus() == cStatusPurchaseNotFound )
      {
        error_log( $ExceptionPrefix . $E->getMessage() );
        $this->apiFailure( 404, "Invalid PlayerCardId or PurchaseId" );
      }
      else
      {
        error_log( $ExceptionPrefix . $E->getMessage() );
        $this->apiFailure( 500, cStatusSystemError );
      }
    }
  }
  
  public function getlegacypurchasedetails( &$DATA )
  {
    if( empty( $DATA[ 'PurchaseId' ] ) )
    {  $this->apiFailure( 404, "Invalid PlayerCardId or PurchaseId" );  }
    
    try
    {
      $ThirdParty = new ThirdPartyWorkflow();
      $Result = $ThirdParty->getlegacypurchasedetails( $DATA );
      $this->smartTemplate( array(), $Result );
    }
    catch( Exception $E )
    {
      if ($E->getStatus() == cStatusInvalidRequest)
      {
        error_log( "getlegacypurchasedetails() exception: " . $E->getMessage() );
        $this->apiFailure( 400, "Bad Request" );
      }
      else if ($E->getStatus() == cStatusPurchaseNotFound )
      {
        error_log( "__FUNCTION__  exception: " . $E->getMessage() );
        $this->apiFailure( 404, "Invalid PlayerCardId or PurchaseId" );
      }
      else
      {
        error_log( "getlegacypurchasedetails() exception: " . $E->getMessage() );
        $this->apiFailure( 500, cStatusSystemError );
      }
    }
  }

}

?>
