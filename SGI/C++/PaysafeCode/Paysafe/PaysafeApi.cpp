// BitKeeper: bhashyam@serak.gamelogic.com|lib/Ecommerce/Paysafe/PaysafeApi.cpp|20170302034948|10955 
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.

///
/// \file PaysafeApi.cpp
///

#include "config.h"
#include "Reveal/RevealApiDefines.h"
#include "PaysafeApi.h"
#include "Ecommerce/EcommerceAccount.h"
#include <boost/regex.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/time_facet.hpp>
#include <boost/assign/list_of.hpp>
#include "SqlStatement.h"
#include "Ecommerce/EcommerceAccountInfo.h"
#include "Lottery/CurrencyTransaction.h"
#include "Ecommerce/TransactionStatusRequest.h"
#include "Ecommerce/TransactionStatusReply.h"
#include "Ecommerce/AccountAdjustmentRequest.h"
#include "Reveal/GiftCode.h"
#include "Ecommerce/Paysafe/PaysafeStoreables/PaysafeOrder.h"
#include "Ecommerce/Paysafe/PaysafeStoreables/PaysafeOrderResponse.h"
#include "Ecommerce/Wallet/WalletSqlTransaction.h"
#include "Ecommerce/Wallet/Wallet.h"
#include "JsonUtil.h"
#include <json/json.h>
#include "Ecommerce/Paysafe/PaysafeStoreables/Error.h"
#include "Ecommerce/Wallet/WalletConfiguration.h"

using namespace std;
using namespace Ecommerce;
using namespace Lottery;
using namespace RevealApi;
using namespace Wallet;

namespace Paysafe
{
  PaysafeApi::PaysafeApi( const RunId& RunID )
    : Ecommerce::InternalProviderApi( RunID )
  {
    if( ! mClient.mInitializeHttpClient( mGetRunId() ) )
    {
      WARNS( "Failed to initialize client" );
      throw ApiStatusException( RevealApi::cStatusInvalidRequest );
    }
  }

  PaysafeApi::~PaysafeApi( void )
  {}

  bool PaysafeApi::mCheckIfResultsIsRollbackable( HttpResults& Results )
  {
    if( mCheckIfValidHttpStatus( Results.mGetHttpStatusCode()  ) )
    { return false; }
    if( Results.mGetResultsSize() > 0 )
    {
      PaysafeStoreables::Error E;
      const int cInternalError = 1000; // this error indicates internal error at Paysafe
      if( E.mPopulateFromJson( Results.mGetResults(), NULL, NULL, false, false ) &&
          ( E.mGetErrorCode().mGetCode() != cInternalError ) )
      {
        // TODO: go through the list of possible error codes and figure out which
        // ones are truly rollbackable, i.e., are not pending states in paysafe
        return true;
      }
      else
      { return false; }
    }
    else
    { return false; }
  }

  void PaysafeApi::mGetAccountsById( Ecommerce::GetAccountsByIdReply* Reply,
                         const Bonus::PlayerCardId& PlayerCardId )
  {}

  void PaysafeApi::mGetACHAccounts( Ecommerce::GetACHAccountsReply* Reply,
                        const Bonus::PlayerCardId& PlayerCardId )
  {}

  void PaysafeApi::mGetCreditCardPaymentAccounts( Ecommerce::GetCreditCardPaymentAccountsReply* Reply,
                                      const std::string& AccountNUmber,
                                      const Bonus::PlayerCardId& )
  {}

  void PaysafeApi::mRemoveCreditCardPaymentAccount( Ecommerce::RemoveCreditCardPaymentAccountReply* Reply,
                                                const std::string& EcommerceAccountNumber,
                                                const std::string& CcAccountId )
  {}

  void PaysafeApi::mReverseAdjustment( Ecommerce::ReverseAdjustmentReply* Reply,
				   const std::string& AccountNumber,
				   const std::string mTransactionReferenceId)
  {}

  ReturnVal<std::string> PaysafeApi::mGetLiabilityAccount( const RunId &Run )
  { return ReturnVal<std::string>( true, "" ); }
  
  std::string PaysafeApi::mGetWithdrawalAccountNumber( const Bonus::PlayerCard& PlayerCard )
  { 
    Wallet::Wallet W;
    W.mFetch( PlayerCard.mGetPlayerCardId() );
    boost::shared_ptr<WalletAccount> Account = W.mGetAccount( EcommerceAccount::eAccountTypeUnrestrictedMoney );
    Account->mGetWalletAccountId();
    std::string AsString = StringHelper::ToString( Account->mGetWalletAccountId() );  
    return AsString; 
  }
  

  void PaysafeApi::mUpdateAccount( Ecommerce::UpdateAccountReply* Reply,
			       const Bonus::PlayerCard& Card,
			       const std::string &AccountNumber,
			       const std::string &TransactionId )
  {}

  void PaysafeApi::mWithdrawCheck( Ecommerce::RequestCheckReply* Reply,
                               Bonus::PlayerCard& Card,
                               const Money& Amount,
                               Lottery::CurrencyTransaction& CT )
  {}

  void PaysafeApi::mPurchaseGift( Ecommerce::GiftCodePurchaseReply* Reply,
			      Bonus::PlayerCard& Card,
			      const Money& Amount,
			      const std::string GiftCode,
			      Lottery::CurrencyTransaction& CT )
  {}

  void PaysafeApi::mRedeemGift( Ecommerce::RedeemGiftCodeReply* Reply,
			    Bonus::PlayerCard& Card,
			    const std::string GiftCode,
			    Lottery::CurrencyTransaction& CT )
  {}


  boost::shared_ptr<EcommerceCreditCardAccount>
  PaysafeApi::mGetDefaultCreditCardAccount( const Bonus::PlayerCardId& PlayerId )
  {
    Wallet::Wallet W;
    if( ! W.mFetch( PlayerId ) )
    {
      THROW_APISTATUSEXCEPTION
        ( RevealApi::cStatusSystemError,
          "Unable to find wallet for playercardid " << PlayerId );
    }

    for( Wallet::Wallet::CreditProfilesIterator Iter = W.mGetCreditProfiles().begin();
         Iter != W.mGetCreditProfiles().end(); ++Iter )
    {
      if( Iter->second->mIsStateCreated() && Iter->second->mIsStateDefault() &&
          ! Iter->second->mIsStateDeleted() )
      {
        // NOTE - currently only the profileid is needed so we are not filling others
        boost::shared_ptr<EcommerceCreditCardAccount> CAccount
          ( new EcommerceCreditCardAccount() );
        CAccount->mSetAccountId( Iter->second->mGetWalletCreditProfileId().mToString() );
        return CAccount;
      }
    }
    throw ApiStatusException( RevealApi::cStatusEcommerceWalletCreditProfileNotFound );
  }

}
