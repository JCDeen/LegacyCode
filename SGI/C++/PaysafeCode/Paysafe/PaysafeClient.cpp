// BitKeeper: bhashyam@serak.gamelogic.com|lib/Ecommerce/Paysafe/PaysafeClient.cpp|20170413162911|27733
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.

///
/// \file PaysafeClient.cpp
///

#include "config.h"
#include "Config.h"
#include "Reveal/RunData.h"
#include "Reveal/RevealApiDefines.h"
#include "PaysafeClient.h"
#include "Storeable.h"
#include "Lottery/CurrencyTransaction.h"
#include "PaysafeStoreables/PaysafeSettlement.h"
#include "PaysafeStoreables/PaysafeOrderRebill.h"
#include "PaysafeStoreables/PaysafeRebillOrderResponse.h"

#include <sstream>
#include <string>

using namespace std;
using namespace PaysafeStoreables;

namespace Paysafe
{
  const int PaysafeClient::cPaysafeErrorHttpStatus = 400;
  const std::string PaysafeClient::cDefaultHostedPagePath = "hosted/v1/orders";
  const std::string PaysafeClient::cDefaultCardPaymentPath = "cardpayments/v1/accounts";
  const std::string PaysafeClient::cDefaultCardPaymentSettlementEndpoint = "settlement";
  const std::string PaysafeClient::cDefaultCardPaymentAuthEndpoint = "auths";
  const std::string PaysafeClient::cDefaultCardPaymentProfilePath = "customervault/v1/profiles";
  const std::string PaysafeClient::cDefaultCardPaymentProfileEndpoint = "";

  PaysafeClient::PaysafeClient( void )
    : Reveal::HttpClient(),
      mAccountNumber(),
      mHostedPagePath(),
      mCallbackUrl(),
      mCardPaymentSettlementEndpoint(),
      mCardPaymentAuthEndpoint(),
      mRequestType( ePaysafeRequestTypeInvalid ),
      mClientHasBeenInitialized( false ),
      mHttpStatusCode()
  {}

  PaysafeClient::~PaysafeClient( void )
  {}

  bool PaysafeClient::mInitializeHttpClient( const RunId &Run )
  {
     mClientHasBeenInitialized = false;

     if( ! Run.mHasValidFormat() )
     {
       WARNS( "Run has invalid format" );
       return false;
     }

     Reveal::RunData RunData( Run );
     if( ! RunData )
     {
       WARNS( "unable to load rundata for run:" << Run );
       return false;
     }

     const ConfigNode *PaysafeConfig = RunData->mFind( "Paysafe" );
     if( ! PaysafeConfig )
     {
       WARNS( "Paysafe node not found for run:" << Run );
       return false;
     }

     // These MUST be present in the config file. If not this init function fails.
     const ConfigNode* AccountNumber = PaysafeConfig->mFindChildByName( "PaysafeAccountNumber" );
     const ConfigNode* User = PaysafeConfig->mFindChildByName( "PaysafeUser" );
     const ConfigNode* Password = PaysafeConfig->mFindChildByName( "PaysafePassword" );
     const ConfigNode *Address = PaysafeConfig->mFindChildByName( "PaysafeAddress" );   // ex: https://api.test.netbanx.com
     const ConfigNode *CallbackUrl = PaysafeConfig->mFindChildByName( "PaysafeCallbackUrl" );   // ex: https://SciPlayHost/api/wallet/providercallback/deposit

     // If any of these are missing, then construct & output an error message, and return false.
     if( !User || !Password || !Address || !AccountNumber || !CallbackUrl )
     {
       std::string StrMissingParms;
       StrMissingParms += ( User ? "" : "PaysafeUser " );
       StrMissingParms += ( Password ? "" : "PaysafePassword " );
       StrMissingParms += ( Address ? "" : "Address " );
       StrMissingParms += ( AccountNumber ? "" : "AccountNumber" );
       StrMissingParms += ( CallbackUrl ? "" : "CallbackUrl" );

       WARNS( "Required configuration parameters not found in config file: "
              << StrMissingParms );
       return false;
     }

     mUsername = User->mGetMyValueAsString().mString();
     mPassword = Password->mGetMyValueAsString().mString();
     mUrl = Address->mGetMyValueAsString().mString();
     mAccountNumber = AccountNumber->mGetMyValueAsString().mString();
     mCallbackUrl = CallbackUrl->mGetMyValueAsString().mString();

     const ConfigNode* HostedPagePath = PaysafeConfig->mFindChildByName( "PaysafeHostedPagePath" );                     // ex: hosted/v1/orders
     const ConfigNode* CardPaymentPath = PaysafeConfig->mFindChildByName( "PaysafeCardPaymentPath" );                   // ex: cardpayments/v1/accounts
     const ConfigNode* CardPaymentSettlementEndpoint = PaysafeConfig->mFindChildByName( "PaysafeCardPaymentSettlementEndpoint" ); // ex: settlement
     const ConfigNode *CardPaymentAuthEndpoint = PaysafeConfig->mFindChildByName( "PaysafeCardPaymentAuthEndpoint" );   // ex: auths

     // If an entire element or the content is missing from config file then use the predefined constant, instead.
     mHostedPagePath = ( !HostedPagePath || HostedPagePath->mIsEmpty() ) ? cDefaultHostedPagePath : HostedPagePath->mGetMyValueAsString().mString();
     mCardPaymentPath = ( !CardPaymentPath || CardPaymentPath->mIsEmpty() ) ? cDefaultCardPaymentPath : CardPaymentPath->mGetMyValueAsString().mString();
     mCardPaymentSettlementEndpoint = ( !CardPaymentSettlementEndpoint || CardPaymentSettlementEndpoint->mIsEmpty() ) ? cDefaultCardPaymentSettlementEndpoint : CardPaymentSettlementEndpoint->mGetMyValueAsString().mString();
     mCardPaymentAuthEndpoint = ( !CardPaymentAuthEndpoint || CardPaymentAuthEndpoint->mIsEmpty() ) ? cDefaultCardPaymentAuthEndpoint : CardPaymentAuthEndpoint->mGetMyValueAsString().mString();

     // For now we init this directly and do not look for it in config file:
     mCardPaymentProfilePath = cDefaultCardPaymentProfilePath;
     mCardPaymentProfileEndpoint = cDefaultCardPaymentProfileEndpoint;

     mAddHttpHeader( "Content-Type", "application/json" );
     mAddHttpHeader( "Accept", "application/json" );

     // Eventually this value needs to be based on a decision:
     mClientHasBeenInitialized = true;

     return mClientHasBeenInitialized;
  }

  // TODO:
  // 	Need some way to specify if a given RequestType should be sent using GET, POST or other method.
  //	Probably some more pre-defined constants.
  void PaysafeClient::mSendRequest
   ( const TypeOfRequest& RequestType,
     const Storeable& Request,
     HttpResults *Results,
     bool& DataMayHaveBeenSent,
     HttpMethod HtMethod )
  {
     if( ! mClientHasBeenInitialized )
     {
       WARNS( "Attempting to mSendRequest to Paysafe but HttpClient is not Initialized!" );
       throw ApiStatusException( RevealApi::cStatusInvalidRequest ) ;
     }

     std::string BaseUrl = mUrl; // Save the URL so we can reset it before leaving.
     mUrl.append( "/" );
     mSetLogRequests( true );
     mSetLogResponses( true );

     std::ostringstream Stream;

     // Build out the URL according to RequestType.
     switch ( RequestType )
     {
       case ePaysafeRequestHostedPageOrder:
         mUrl.append( mHostedPagePath );
         Request.mToCorrectJson( Stream );
         break;

       case ePaysafeRequestProcessRebillUsingOrderId:
         mUrl.append( mHostedPagePath );
         mUrl.append( "/" );
         mUrl.append( static_cast< const PaysafeOrderRebill& > ( Request ).mGetId() );
         Request.mToCorrectJson( Stream );
         break;

       case ePaysafeRequestCardPaymentAuthorization:
       {
         mUrl.append( mCardPaymentPath ); // Typically "cardpayments/v1/accounts" .
         mUrl.append( "/" );
         mUrl.append( mAccountNumber );   // Same as ACCOUNT_ID in the docs.
         mUrl.append( "/" );
         mUrl.append( mCardPaymentAuthEndpoint ); // Typically "auths".
         Request.mToCorrectJson( Stream );
       }
       break;

       // Settle funds previously authorized on a card.
       case ePaysafeRequestCardPaymentSettlement:
       {
         mUrl.append( mCardPaymentPath );         // Typically "cardpayments/v1/accounts" .
         mUrl.append( "/" );
         mUrl.append( mAccountNumber );           // Same as ACCOUNT_ID in the docs.
         mUrl.append( "/" );
         mUrl.append( mCardPaymentAuthEndpoint ); // Typically "auths".
         mUrl.append( "/" );
         mUrl.append( static_cast< const PaysafeSettlement& > ( Request ).mGetAuthId() );
         mUrl.append( "/" );
         mUrl.append( mCardPaymentSettlementEndpoint ); // Typically "settlement".
         Request.mToCorrectJson( Stream );
       }
       break;

       case ePaysafeRequestCardPaymentRemovePaymentProfile:
       {
         if ( ! mCardPaymentProfileEndpoint.empty() )
         {
           mUrl.append( mCardPaymentProfilePath );
           mUrl.append( "/" );
           mUrl.append( mCardPaymentProfileEndpoint );
         }
         else
         {
           WARNS( "mCardPaymentProfileEndpoint was NOT set." );
           return;
         }
       }
       break;

       // both order status and order deletion use the same URI, only different methods
       case ePaysafeRequestOrderDelete:
       case ePaysafeRequestOrderStatus:
       {
         mUrl.append( mHostedPagePath );
         mUrl.append( "/" );
         mUrl.append( static_cast< const PaysafeOrderRebill& > ( Request ).mGetId() );
       }
       break;
       
       case ePaysafeRequestTransactionStatus:
       {
         mUrl.append( mCardPaymentPath );         // Typically "cardpayments/v1/accounts" .
         mUrl.append( "/" );
         mUrl.append( mAccountNumber );           // Same as ACCOUNT_ID in the docs.
         mUrl.append( "/" );
         mUrl.append( "auths" ); // Typically "auths".
         mUrl.append( "?merchantRefNum=" );
         mUrl.append( static_cast< const PaysafeOrderRebill& > ( Request ).mGetMerchantRefNum() );
       }
       break;

       case ePaysafeRequestTypeInvalid:
       default:
         WARNS( "  eTypeInvalid not supported: " << Request.mToString() );
         break;
     }

     Results->mReset();
     mSend( HtMethod, Stream.str().c_str(), Results, DataMayHaveBeenSent );

     mHttpStatusCode = Results->mGetHttpStatusCode();
     mUrl = BaseUrl;	// Reset mUrl to base Url for future use.
  }

  bool PaysafeClient::mCheckIfValidHttpStatus( int Status )
  {
    // paysafe can return really any http status including 500 with a rollbackable response (!)
    return true;
  }

  bool PaysafeClient::mCheckIfSuccessHttpStatus( int Status )
  {
    // Successful responses are 200 or 201.
    return ( ( Status == 200 ) ||  ( Status == 201 ) );
  }
}
