<?php
// BitKeeper: jdeen@rex.gamelogic.com|php/lib/Models/Reveal/IGTConnector.php|20170301215518|46387
// Copyright (C) 2016 GameLogic, Inc. All Rights Reserved.

require_once 'Helper.php';

class IGTConnector
{  
  public function __construct()
  {
    $Config = RunConfig( Helper::GetRunId() );
    
    if( empty( $Config ) )
    { error_log( "Failed to read IGT config from game config." ); }
    else
    {
      $this->Url = (string) $Config['ThirdPartyApi']['IGT']['BingoPurchaseRequestUrl']; 
      $this->TimezoneForDisplay = (string) $Config['ThirdPartyApi']['IGT']['IGTBingoTimezoneForDisplay']; 
    }
  }

  public function getTransactionDetails( $PlayerCardId, $PlayerAuthToken, $TransactionId, $WinStatus )  
  {
    $IgtParameters = array ( 'getTransactionInVO' => 
        array( "playerCardId" => $PlayerAuthToken,  //name mismatch is historical
               "transactionId" => $TransactionId,
               "registrarId" => "",        // Can be null, according to Choudhury.
               "partnerPurchaseId" => ""   // Can be null, according to Choudhury.
        ) );

    try 
    {
      $Client = new SoapClient( $this->Url . '?wsdl' );
      $IgtResults = $Client->getTransactionDetails( $IgtParameters );
    } 
    catch( Exception $E )
    {
      error_log("Failed to contact IGT Connector: $E");
      throw new ApiStatusException("Unable to contact remote service.", cStatusSystemError);
    }
    
    $IgtDetailsResults = $IgtResults->getTransactionDetailsResult;   // "getTransactionDetailsResult" is a node name, not a function
    if ( $IgtDetailsResults->result->result == 0 )  // 0 is success
    {
      // Translate some of IGT's results into the Swagger page's formats.
      $SwaggerStatusStr = "OK";

      $SavedTZ = date_default_timezone_get(); // Save so we can restore it.      
      date_default_timezone_set( $this->TimezoneForDisplay );
      
      $SwaggerStartDate = strtotime( $IgtDetailsResults->startDate );
      $SwaggerEndDate = strtotime( $IgtDetailsResults->endDate );
      $SwaggerStartDate = ( $SwaggerStartDate ) ? $SwaggerStartDate : 0;
      $SwaggerEndDate = ( $SwaggerEndDate ) ? $SwaggerEndDate : 0;
      
      date_default_timezone_set( $SavedTZ ); // Restore original timezone.

      $GameIdName = ( isset ( $IgtDetailsResults->gameID ) ) ? "gameID" : "gameId"; // Coded defensively because ESB changed their code.
       
      return $Result = 
        array
        (
            // These values are simply passed thru to AEM:
            "PlayerCardId" => $PlayerCardId,
            "TransactionId" => (string) $TransactionId,
            "WinStatus" => (string) $WinStatus,
        
            // The rest of these details  come via the IGT Connector:
            "GameId" => (string) $IgtDetailsResults->$GameIdName,
            "GameReplayUrl" => (string) $IgtDetailsResults->gameReplayUrl,
            "GameName" => (string) $IgtDetailsResults->gameName,
            "SessionId" => (string) $IgtDetailsResults->sessionId,
            "StartDate" => $SwaggerStartDate,
            "EndDate" => $SwaggerEndDate,
            "Duration" => $IgtDetailsResults->duration,
            "WagerAmount" => $IgtDetailsResults->wagerAmount,
            "WinAmount" => $IgtDetailsResults->winAmount,
            "Status" => (string) $SwaggerStatusStr
        );
    } 
    else
    {
      $IGTStatusCode = $IgtDetailsResults->result->result;
      $IGTStatusMessage = ( string ) $IgtDetailsResults->result->description;
      $ErrLogMsg = "IGT error result=" . ( string ) $IGTStatusCode . 
      	" desc=" . $IGTStatusMessage . " \$PlayerCardId=$PlayerCardId \$TransactionId=" . ( string ) $TransactionId;
      error_log( $ErrLogMsg );
      
      $RetStatusCode = ( $IGTStatusCode >= 400 && $IGTStatusCode < 600 ) ? $IGTStatusCode : 500;
      $RetStatusMsg = ( $IGTStatusCode >= 400 && $IGTStatusCode < 600 ) ? $IGTStatusMessage : cStatusSystemError;
      throw new ApiStatusException( "Error $RetStatusCode $RetStatusMsg", $RetStatusCode ); // System Error
    }
  }
}

