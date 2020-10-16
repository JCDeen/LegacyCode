<?php
// BitKeeper: jdeen@rex.gamelogic.com|php/lib/Models/Reveal/OpenBet.php|20170503222418|30860
// Copyright (C) 2016 GameLogic, Inc. All Rights Reserved.

require_once 'Helper.php';

class OpenBet
{
  const StatusCodeSuccess  ='001';
  
  public function __construct()
  {
    $OBConfig = RunConfig( Helper::GetRunId() );
    
    if( empty( $OBConfig ) )
    { error_log( "Failed to read OpenBet config from game config." ); }
    else
    {
      $this->Username =           ( string ) $OBConfig['ThirdPartyApi']['OpenBet']['AdminUser'];                
      $this->Password =           ( string ) $OBConfig['ThirdPartyApi']['OpenBet']['AdminPassword'];            
      $this->Url =                ( string ) $OBConfig['ThirdPartyApi']['OpenBet']['SportsPurchaseRequestUrl']; 
      $this->TimezoneForDisplay = ( string ) $OBConfig['ThirdPartyApi']['OpenBet']['OpenBetTimezoneForDisplay'];
      $this->Retrylimit =         ( int )    $OBConfig['ThirdPartyApi']['OpenBet']['Retrylimit'];
      $this->LogOBSendRequests =  ( bool )   $OBConfig['ThirdPartyApi']['OpenBet']['LogOBSendRequests'];
    }
  }
  
  public function getUsername() { return $this->Username; }
  public function getPassword() { return $this->Password; }
  public function getUrl() { return $this->Url; }
  public function getTimezoneForDisplay() { return $this->TimezoneForDisplay; }
  public function getRetrylimit() { return $this->Retrylimit; }
  public function getLogOBSendRequests() { return $this->LogOBSendRequests; }
  
  public function getRegistrarId( $PlayerCardId )
  {
    $Request = '<reqAccountAddStub>' .
               "  <accountNo>$PlayerCardId</accountNo>" .
               '</reqAccountAddStub>';
    $RetryTimes = -1;
    do 
    {    
      $Response = $this->sendRequest( $Request );
      if( ! empty( $Response->respAccountAddStub->custId ) )
      { 
        // successfully get response
        break; 
      }
      // retry until Retrylimit is reached
      $RetryTimes ++;
    
    } while( $RetryTimes < $this->Retrylimit );
    
    if( ! empty( $Response->respAccountAddStub->custId ) )
    { return $Response->respAccountAddStub->custId; }
    else 
    { return NULL; }
  }
  
  /// \brief get purchase transaction details from OpenBet using the CustId. CustId is the third
  //  party player id (PlayerCard.RegistrarId).
  public function getPurchaseDetails( $RegistrarId, $PartnerTransactionId )
  {
    $Request = '<reqAccountHistory>' .
               "  <token>$RegistrarId</token>" .
               "  <slip id=\"$PartnerTransactionId\">" .
               '</reqAccountHistory>';
    $Options = array( 'Dtd' => 'reqAccountHistory.dtd',
                      'Version' => '7.0' );
    $Response = $this->sendRequest( $Request, $Options );
    return $Response;
  }
  
  /// \brief send a high tier claim paid notice to OB.
  /// @param $PlayerAccountId - the playercard.registrarid
  /// @param $SlipId - the thirdpartypurchase.partnertransactionid
  /// @return - mixed (T|F|string) - false on system error, true on success, string for all other error messages
//  public function claimHighTierWin( $PlayerAccountId, $BetId )
  public function claimHighTierWin( $PlayerAccountId, $SlipId )
  {
    $Request = '<reqBetUpdate>' .
        "<token>$PlayerAccountId</token>" .
        '<updateSlip>' .
        "<slipId>$SlipId</slipId>" .
        '<action>PAYOUT</action>' .
        '</updateSlip>'.
        '</reqBetUpdate>';
// TODO:
//  Temporarily disable the retry logic in this function.
//  If it pans out, we will remove it alltogether. JCD.   
//    $RetryTimes = -1;
//    do
//    { 
      $Response = $this->sendRequest( $Request );
      //If the call is really screwed up we won't get the respBetUpdate response with the betFailure. Here we just get a high level status
      //but the http did work so we don't want to retry.
      if( empty( $Response->respBetUpdate ) && ! empty( $Response->returnStatus) )
      { return $Response->returnStatus->message; }
     
      // This processes single bet slips perfectly & returns TRUE. ( lines 100 - 108 )
      if( ! empty( $Response->respBetUpdate->updateStatus ) )
      {
        if( $Response->respBetUpdate->updateStatus->status == 'OK' )
        {
          if ( $this->LogOBSendRequests )
          {  error_log( "OpenBet.claimHighTierWin: RETURN TRUE 'if(\$Response->respBetUpdate->updateStatus->status==\'OK\')'" );  }
          return true;
        }
      }
      elseif( ! empty( $Response->respBetUpdate->betFailure ) )
      {
        if ( $this->LogOBSendRequests )
        { error_log( "OpenBet.claimHighTierWin: FORMERLY the code would hit a 'break;' statement here." ); }
      }     
//    { break; }  //valid response but with a failure - retry will not help
//    // retry until Retrylimit is reached
//      $RetryTimes ++;
//    } while( $RetryTimes < $this->Retrylimit );
 
    if( ! empty( $Response->respBetUpdate->betFailure ) )
    {
      if ( $this->LogOBSendRequests )
      { error_log( "OpenBet.claimHighTierWin: below is normal error_log for: if(!empty(\$Response->respBetUpdate->betFailure))" ); }
      error_log( 'Failed to claim high tier win: ' . print_r( $Response->respBetUpdate, true ) );
      return $Response->respBetUpdate->betFailure->betFailureReason;
    }
    else
    {
      if ( $this->LogOBSendRequests )
      { 
        //error_log( "OpenBet.claimHighTierWin: RETURN FALSE \$PlayerAccountId='$PlayerAccountId' \$BetId='$BetId'" ); 
        error_log( "OpenBet.claimHighTierWin: RETURN TRUE \$PlayerAccountId='$PlayerAccountId' \$SlipId='$SlipId'" ); 
      }
      //return false;
      return TRUE;
    }
  }
 
  protected function sendRequest( $Request, $Options = array() )
  {
    $Options = array_merge( array( 'Dtd' => "oxip.dtd",
                                   'Version' => '6.0' ), //accounthistory is 7.0
                            $Options );
    $XmlRequest =
      '<?xml version="1.0" encoding="UTF8" ?>' .
      "<!DOCTYPE oxip SYSTEM \"{$Options['Dtd']}\">" .
      "<oxip version=\"{$Options['Version']}\">" .
      '<request>' .
      '  <reqClientAuth>' .
      "    <user>$this->Username</user>" .
      "    <password>$this->Password</password>" .
      '  </reqClientAuth>' .
      "  $Request " .
      '</request>' .
      '</oxip>';
   
    $Client = new HttpClient();
    $Client->mSetCurlOptions( 'Url', "$this->Url" );
    $Client->mSetCurlOptions( "VerifyPeer", FALSE );
    $Client->mSetCurlOptions( "VerifyHost", FALSE );
    $Client->mSetCurlOptions( "Verbose", TRUE );
    $Client->mAddHttpHeader( array( "Accept: Content-Type: text/xml" ) );
   
    $Sent = false;
    $XmlResponse = '';
    $Client->mPost( $XmlRequest, $XmlResponse, $Sent );
    $HttpResults = $Client->mGetHttpInfo();
   
    if ( $this->LogOBSendRequests )
    {
      $httpRetCode = $HttpResults['http_code'];
      error_log( "OpenBet.SendRequest: \$httpRetCode='$httpRetCode' \$XmlRequest='$XmlRequest' \$XmlResponse='$XmlResponse' \$Sent='$Sent'" );
    }
   
    if( $HttpResults['http_code'] == 200 )
    {
      $ResponseObject = $this->decodeResponse( $XmlResponse );
     
      if( empty( $ResponseObject->response->returnStatus->code ) )
      {
        if ( $this->LogOBSendRequests )
        { error_log( "OpenBet.SendRequest: RETURN \$this->handleFailure(\$HttpResults,\$XmlResponse,'Invalid response format:\$XmlResponse')" ); }
        return $this->handleFailure($HttpResults,
                                    $XmlResponse,
                                    "Invalid response format: $XmlResponse" );
      }
      elseif( $ResponseObject->response->returnStatus->code != self::StatusCodeSuccess )
      {
        if ( $this->LogOBSendRequests )
        { error_log( "OpenBet.SendRequest: RETURN \$this->handleFailure(\$HttpResults, \$XmlResponse, $ResponseObject->response->returnStatus)" ); }
        $this->handleFailure($HttpResults,
                             $XmlResponse,
                             $ResponseObject->response->returnStatus );
      }
      if ( $this->LogOBSendRequests )
      { error_log( "OpenBet.SendRequest: RETURN \$ResponseObject->response;" ); }
      return $ResponseObject->response;
    }
    else
    {
      if ( $this->LogOBSendRequests )
      { error_log( "OpenBet.SendRequest: RETURN \$this->handleFailure(\$HttpResults, NULL)" ); }
      return $this->handleFailure( $HttpResults, NULL );
    }
  }
 
  /// \brief report api failures to error_log. OpenBet returns http 200 even for errors but we want to catch
  // non 200 in case we hit a gateway error or some other condition. If we get a 200 then the error and message
  // are found in the $Error object.
  protected function handleFailure( $HttpResults, $XmlResponse, $Error = NULL )
  {
    if( is_string( $Error ) )
    {
      $Message = $Error;
      $Error = stdClass();
      $Error->code = 'Undef';
      $Error->Message = $Message;
      $Error->debug = 'Internal';
    }
    if( $HttpResults['http_code'] != 200)
    { error_log( "OpenBet.handleFailure api failure, http status: {$HttpResults['http_code']} $XmlResponse" ); }
    else
    { error_log( "OpenBet.handleFailure api failure, $Error->code $Error->message $Error->debug" ); }
    return NULL;
  }
 
  protected function decodeResponse( $Xml )
  {
    if( ! $Result = @simplexml_load_string( $Xml ) )
    {
      error_log( "OpenBet.decodeResponse: Malformed XML from OpenBet: $Xml" );
      return NULL;
    }
   
    //TODO: we need a more complete xml decoder
    $Object = json_decode( json_encode( $Result ) );
    if( empty( $Object ) )
    { error_log( "OpenBet.decodeResponse: Failed to decode OpenBet XML to an object: $Xml" ); }
    return $Object;
  }
}

