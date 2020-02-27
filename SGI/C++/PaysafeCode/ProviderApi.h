// BitKeeper: bhashyam@serak.gamelogic.com|lib/Ecommerce/ProviderApi.h|20170321150616|26807
// Copyright(C) 2012 Gamelogic Inc. All rights reserved.
//
#ifndef __Ecommerce_ProviderApi_h__
#define __Ecommerce_ProviderApi_h__

#ifndef PROVIDERAPI_TEST_DECL
#define PROVIDERAPI_TEST_DECL
#endif

///
/// \file  ProviderApi.h
/// \brief Declaration of class: ProviderApi
///

#include "Reveal/RevealReply.h"
#include "Reveal/RunId.h"
#include "Money.h"
#include "Bonus/PlayerCard.h"
#include "GetAccountsByIdReply.h"
#include "GetAllAccountsReply.h"
#include "CreateAccountReply.h"
#include "GetACHAccountsReply.h"
#include "GetCreditCardPaymentAccountsReply.h"
#include "RemoveCreditCardPaymentAccountReply.h"
#include "GetAccountsByIdReply.h"
#include "GetACHAccountsReply.h"
#include "AccountAdjustmentReply.h"
#include "ReverseAdjustmentReply.h"
#include "AccountTransferReply.h"
#include "EcommerceAccount.h"
#include "UpdateAccountReply.h"
#include "TransactionStatusReply.h"
#include "EventCompleteReply.h"
#include "EventCompleteRequest.h"
#include "AccountTransferRequest.h"
#include "RemovePaymentProfileReply.h"
#include "RemovePaymentProfileRequest.h"
#include "GiftCodePurchaseReply.h"
#include "RedeemGiftCodeReply.h"
#include "RequestCheckReply.h"
#include "HostedPageDepositRequest.h"
#include "HostedPageDepositReply.h"
#include "FundWithPaymentProfileRequest.h"
#include "FundWithPaymentProfileReply.h"
#include "Bonus/PlayerCard.h"
#include <boost/shared_ptr.hpp>
#include "ReturnVal.h"
#include "HttpResults.h"

namespace Ecommerce
{

///
///
class ProviderApi
{
public:

  typedef boost::shared_ptr< ProviderApi > pointer_type;

  enum CheckRequestFlavor
  {
    ENUM( "Invalid", eCheckRequestFlavorInvalid = -1 ),
    ENUM( "Adjustment", eCheckRequestFlavorAdjustment = 0 ),
    ENUM( "Check", eCheckRequestFlavorCheck = 1 )
  };

  ProviderApi( const RunId& );

  virtual ~ProviderApi( void );

  static ProviderApi::pointer_type mCreate( const RunId& );
  void mSetRevealSystem( RevealSystem* System )
  { mSystem = System; }
  RevealSystem* mGetRevealSystem( void )
  { return mSystem; }

  const RunId& mGetRunId( void ) const
  { return mRunId; }

  // \throw ApiStatusException
  virtual void mCreateAccount( CreateAccountReply* Reply,
                               Bonus::PlayerCard& Card,
                               SqlTransaction& Transaction ) = 0;

  // \throw ApiStatusException
  virtual void mGetAccountsById( GetAccountsByIdReply* Reply,
                                 const Bonus::PlayerCardId& PlayerCardId ) = 0;

  // \throw ApiStatusException
  virtual void mGetAllAccounts( GetAllAccountsReply* Reply,
                                const Bonus::PlayerCardId& PlayerCardId ) = 0;

  // \throw ApiStatusException
  virtual void mGetACHAccounts( GetACHAccountsReply* Reply,
                                const Bonus::PlayerCardId& PlayerCardId ) = 0;

  // \throw ApiStatusException
  virtual void mGetCreditCardPaymentAccounts( GetCreditCardPaymentAccountsReply* Reply,
                                              const std::string& AccountNUmber,
                                              const Bonus::PlayerCardId& ) = 0;

  // \throw ApiStatusException
  virtual void mRemoveCreditCardPaymentAccount( RemoveCreditCardPaymentAccountReply* Reply,
                                                const std::string& EcommerceAccountNumber,
                                                const std::string& CcAccountId ) = 0;
  
  // \throw ApiStatusException
  virtual void mCreateAdjustment( AccountAdjustmentReply* Reply, 
                                  const Bonus::PlayerCard& PlayerCard,
                                  const std::string& AccountNumber,
                                  const Money& Amount, 
                                  const Lottery::CurrencyTransaction& CurrencyTransaction,
                                  const std::string& Description,
                                  const std::string& ToAccountNumber = std::string() ) = 0;
  
  // \throw ApiStatusException
  virtual void mReverseAdjustment( ReverseAdjustmentReply* Reply, 
				   const std::string& AccountNumber,
				   const std::string mTransactionReferenceId) = 0;

  // create a pending account transfer with provider, by default we will do nothing
  virtual void mCreatePendingAccountTransfer
  ( const Ecommerce::AccountTransferRequest::TransferType TransferType,
    const Lottery::CurrencyTransaction& CurrencyTransaction, Money& NonRgAmount )
  {}

  virtual void mAdjustPendingAccountTransfer
  ( const Lottery::CurrencyTransaction& CurrencyTransaction, std::string& ProvTxId )
  {}

  // \throw ApiStatusException
  virtual void mCreateAccountTransfer( Ecommerce::AccountTransferReply* Reply,
                                       const Bonus::PlayerCard& PlayerCard,
                                       const std::string& FromAccountNumber, 
                                       const std::string& ToAccountNumber, 
                                       const Money& Amount, 
                                       const std::string& mTransactionId, 
				       const std::string& ProfileId,
				       const Ecommerce::AccountTransferRequest::TransferType TransferType,
                                       const std::string& Message ) = 0;

  virtual ReturnVal<std::string> mGetLiabilityAccount( const RunId &Run ) = 0;

  virtual std::string mGetWithdrawalAccountNumber( const Bonus::PlayerCard& PlayerCard ) = 0;

  // \throw ApiStatusException
  virtual void mUpdateAccount( UpdateAccountReply* Reply, 
			       const Bonus::PlayerCard& Card, 
			       const std::string &AccountNumber,
			       const std::string &TransactionId ) = 0;

  virtual void mTransactionStatus( TransactionStatusReply* Reply,
                                   const Bonus::PlayerCard& Card ) = 0;

  virtual void mEventComplete( Ecommerce::EventCompleteReply* Reply,
                               Ecommerce::EventCompleteRequest* Request,
                               bool& Ignored ) = 0;

  virtual void mRemovePaymentProfile( Ecommerce::RemovePaymentProfileReply* Reply,
                                      Ecommerce::RemovePaymentProfileRequest* Request ) = 0;

  virtual void mWithdrawCheck( Ecommerce::RequestCheckReply* Reply,
                               Bonus::PlayerCard& Card,
                               const Money& Amount,
                               Lottery::CurrencyTransaction& CT ) = 0;

  // Gifting additions
  virtual void mPurchaseGift( Ecommerce::GiftCodePurchaseReply* Reply,
			      Bonus::PlayerCard& Card,
			      const Money& Amount,
			      const std::string GiftCode,
			      Lottery::CurrencyTransaction& CT ) = 0;
			      
  virtual void mRedeemGift( Ecommerce::RedeemGiftCodeReply* Reply,
			    Bonus::PlayerCard& Card,
			    const std::string GiftCode,
			    Lottery::CurrencyTransaction& CT ) = 0;


  virtual void mAddFunds
  ( FundWithPaymentProfileReply* Reply,
    FundWithPaymentProfileRequest* Request ) = 0;

  virtual void mGetHostedPageDepositUrl
  ( HostedPageDepositReply* Reply,
    const HostedPageDepositRequest* Request ) = 0;

  virtual bool mDeleteHostedPage( const CurrencyTransaction& CT )
  { return true; }

  // \throw ApiStatusException if no default credit card is found
  // Not implemented in base class, and currently only implemented in PaysafeApi
  virtual boost::shared_ptr<EcommerceCreditCardAccount>
  mGetDefaultCreditCardAccount( const Bonus::PlayerCardId& PlayerId )
  { throw ApiStatusException( "unimplemented" ); }

  // Check with ecommerce provider about a given Pending purchase CT and re-attempt/complete it
  // if Redo is true.  If Redo is false then only void the transaction.
  // Returns invalid CurrencyTransactionId if unsuccessful otherwise returns CT transaction ID
  // that relates to the completed transaction.  This can be different from Pending's ID because
  // it could be voided and re-created by the underlying provider.
  virtual bool mCompletePending
  ( Lottery::CurrencyTransaction& Pending,
    Lottery::CurrencyTransaction& NewPending, RevealSystem* System, const RunId& Run,
    bool Redo = true ) = 0;

protected:

// by default providers will provide us with a 200 HTTP for "successful" processing
  virtual bool mCheckIfValidHttpStatus( int Status )
  { return ( Status == 200 ); }

  // allow specific providers to define what can be rollbacked
  virtual bool mCheckIfHttpStatusRollbackable( int Status )
  { return false; }

  virtual bool mCheckIfResultsIsRollbackable( HttpResults& Results )
  { return false; }

  bool mVerboseClient;

  virtual bool mVerifyResults( HttpResults &Results,
		       bool &DataMayHaveBeenSent,
		       Ecommerce::EcommerceReply* Reply );

  // void a currencytransaction
  void mVoidTransaction( const Ecommerce::EventCompleteReply* Reply  );

private:

  RunId mRunId;
  RevealSystem* mSystem;

  ///
  /// Copying is disabled for this class
  ///
  ProviderApi( const ProviderApi &Other );

  ///
  /// Assignment is disabled for this class
  ///
  ProviderApi &operator=( const ProviderApi &Other );

  PROVIDERAPI_TEST_DECL;
};

}

ENUMERATOR( ProviderCheckRequestFlavor, Ecommerce::ProviderApi::CheckRequestFlavor );

#endif // __Ecommerce_ProviderApi_h__
