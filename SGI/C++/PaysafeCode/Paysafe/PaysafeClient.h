// BitKeeper: jsandberg@sara.gamelogic.com|lib/Ecommerce/Paysafe/PaysafeClient.h|20170411225119|31307
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.

#ifndef __PAYSAFECLIENT_H__
#define __PAYSAFECLIENT_H__

#ifndef PAYSAFECLIENT_TEST_DECL
#define PAYSAFECLIENT_TEST_DECL
#endif

///
/// \file  PaysafeClient.h
/// \brief Declaration of class: PaysafeClient
///
//#include "PaysafeApi.h"
#include "Reveal/HttpClient.h"
#include "HttpResults.h"
#include "HttpBase.h"
#include "Storeable.h"

// #include "PaysafeRequestParams.h"

namespace Paysafe
{

///
///
  class PaysafeClient : public Reveal::HttpClient
  {
  public:

    PaysafeClient( void );
    virtual ~PaysafeClient( void );

    bool mInitializeHttpClient( const RunId &Run );

    enum TypeOfRequest
    {
      ENUM( "InvalidPaysafeRequest", ePaysafeRequestTypeInvalid = -1 ),
      ENUM( "HostedPageOrder", ePaysafeRequestHostedPageOrder = 0 ),
      ENUM( "CardPaymentSettlement", ePaysafeRequestCardPaymentSettlement = 1 ),
      ENUM( "CardPaymentAuthorization", ePaysafeRequestCardPaymentAuthorization = 2 ),
      ENUM( "CardPaymentRemovePaymentProfile", ePaysafeRequestCardPaymentRemovePaymentProfile = 3 ),
      ENUM( "ProcessRebillUsingOrderId", ePaysafeRequestProcessRebillUsingOrderId = 4 ),
      ENUM( "OrderStatus", ePaysafeRequestOrderStatus = 5 ),
      ENUM( "OrderDelete", ePaysafeRequestOrderDelete = 6 ),
      ENUM( "TransactionStatus", ePaysafeRequestTransactionStatus = 7 ) 
    };

    void mSendRequest( const TypeOfRequest& RequestType,
                       const Storeable& Request,
                       HttpResults* Results,
                       bool& DataMayHaveBeenSent,
                       HttpMethod HtMethod = eHttpMethodPost );

    const TypeOfRequest& mGetRequestType() const
    { return mRequestType; }

    const std::string& mGetAccountNumber() const
    { return mAccountNumber; }

    const std::string& mGetUsername() const
    { return mUsername; }

    const std::string& mGetPassword() const
    { return mPassword; }

    const std::string& mGetUrl() const
    { return mUrl; }

    const std::string& mGetCallbackUrl() const
    { return mCallbackUrl; }

    const std::string& mGetHostedPagePath() const
    { return mHostedPagePath; }

    const std::string& mGetCardPaymentPath() const
    { return mCardPaymentPath; }

    const std::string& mGetCardPaymentProfilePath() const
    { return mCardPaymentProfilePath; }

    const std::string& mGetCardPaymentProfileEndpoint() const
    { return mCardPaymentProfileEndpoint; }

    void mSetCardPaymentProfileEndpoint(  const std::string& ProfileEndpointUrl )
    { mCardPaymentProfileEndpoint = ProfileEndpointUrl; }

    const std::string& mGetCardPaymentSettlementEndpoint() const
    { return mCardPaymentSettlementEndpoint; }

    const std::string& mGetCardPaymentAuthEndpoint() const
    { return mCardPaymentAuthEndpoint; }

    int mGetHttpStatusCode()
    { return mHttpStatusCode; }

    // Valid responses are 200, 201 or 400.
    virtual bool mCheckIfValidHttpStatus( int );

    // Successful responses are 200 or 201.
    virtual bool mCheckIfSuccessHttpStatus( int );

  private:

    std::string mAccountNumber;
    std::string mHostedPagePath;
    std::string mCardPaymentPath;
    std::string mCardPaymentProfilePath;
    std::string mCallbackUrl;
    std::string mCardPaymentSettlementEndpoint;
    std::string mCardPaymentAuthEndpoint;
    std::string mCardPaymentProfileEndpoint;

    TypeOfRequest mRequestType;

    bool mClientHasBeenInitialized;
    int  mHttpStatusCode; // Remember last status code.

    // TODO:
    // Need to default for mCallbackUrl...

    static const int cPaysafeErrorHttpStatus;
    static const std::string cDefaultHostedPagePath;
    static const std::string cDefaultCardPaymentPath;
    static const std::string cDefaultCardPaymentProfilePath;
    static const std::string cDefaultCardPaymentSettlementEndpoint;
    static const std::string cDefaultCardPaymentAuthEndpoint;
    static const std::string cDefaultCardPaymentProfileEndpoint;

    ///
    /// Copying is disabled for this class
    ///
    PaysafeClient( const PaysafeClient &Other );

    ///
    /// Assignment is disabled for this class
    ///
    PaysafeClient &operator=( const PaysafeClient &Other );

    PAYSAFECLIENT_TEST_DECL;
  };
}

#endif // __PAYSAFECLIENT_H__
