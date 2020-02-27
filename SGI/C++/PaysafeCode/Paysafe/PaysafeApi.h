// BitKeeper: bhashyam@serak.gamelogic.com|lib/Ecommerce/Paysafe/PaysafeApi.h|20170501184943|27851 
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.
//
#ifndef __Paysafe_PaysafeApi_h__
#define __Paysafe_PaysafeApi_h__

#ifndef PAYSAFEAPI_TEST_DECL
#define PAYSAFEAPI_TEST_DECL
#endif

#include "Ecommerce/InternalProviderApi.h"
#include "PaysafeClient.h"
#include "Lottery/LotteryDefines.h"
#include "Ecommerce/Wallet/WalletAccountTransaction.h"
#include "Ecommerce/Wallet/WalletConfiguration.h"

namespace Paysafe
{

  class PaysafeApi : public Ecommerce::InternalProviderApi
  {
  public:
    enum OrderState
    {
      ENUM( "invalid", ePaysafeOrderStateInvalid = -1 ),
      ENUM( "success", ePaysafeOrderStateSuccess = 0 ),
      ENUM( "declined", ePaysafeOrderStateDeclined = 1 ),
      ENUM( "voided", ePaysafeOrderStateVoided = 2 ),
      ENUM( "abandoned", ePaysafeOrderStateTimeout = 3 ),
      ENUM( "cancelled", ePaysafeOrderStateCancelled = 4 ),
      ENUM( "errored", ePaysafeOrderStateErrored = 5 ),
      ENUM( "pending", ePaysafeOrderStatePending = 6 ),
    };

    PaysafeApi( const RunId& );

    virtual ~PaysafeApi( void );
    
    virtual void mCreateAccount( Ecommerce::CreateAccountReply* Reply,
                         Bonus::PlayerCard& Card,
                         SqlTransaction& Transaction );

    virtual void mGetAccountsById( Ecommerce::GetAccountsByIdReply* Reply,
                           const Bonus::PlayerCardId& PlayerCardId );

    virtual void mGetAllAccounts( Ecommerce::GetAllAccountsReply* Reply,
                          const Bonus::PlayerCardId& PlayerCardId );

    virtual void mGetACHAccounts( Ecommerce::GetACHAccountsReply* Reply,
                          const Bonus::PlayerCardId& PlayerCardId );

    virtual void mGetCreditCardPaymentAccounts( Ecommerce::GetCreditCardPaymentAccountsReply* Reply,
                                        const std::string& AccountNUmber,
                                        const Bonus::PlayerCardId& );

    virtual void mRemoveCreditCardPaymentAccount( Ecommerce::RemoveCreditCardPaymentAccountReply* Reply,
                                                  const std::string& EcommerceAccountNumber,
                                                  const std::string& CcAccountId );

    virtual void mCreateAdjustment( Ecommerce::AccountAdjustmentReply* Reply,
                                    const Bonus::PlayerCard& PlayerCard,
                                    const std::string& AccountNumber,
                                    const Money& Amount,
                                    const Lottery::CurrencyTransaction& CurrencyTransaction,
                                    const std::string& Description,
                                    const std::string& ToAccountNumber );

    // \throw ApiStatusException upon error
    virtual void mCreatePendingAccountTransfer
    ( const Ecommerce::AccountTransferRequest::TransferType TransferType,
      const Lottery::CurrencyTransaction& CurrencyTransaction, Money& NonRgAmount );

    virtual void mAdjustPendingAccountTransfer
    ( const Lottery::CurrencyTransaction& CurrencyTransaction, std::string& ProvTxId );

    virtual void mReverseAdjustment( Ecommerce::ReverseAdjustmentReply* Reply,
  				   const std::string& AccountNumber,
  				   const std::string mTransactionReferenceId);

    virtual void mCreateAccountTransfer( Ecommerce::AccountTransferReply* Reply,
                                 const Bonus::PlayerCard& PlayerCard,
                                 const std::string& FromAccountNumber,
                                 const std::string& ToAccountNumber,
                                 const Money& Amount,
                                 const std::string& mTransactionId,
                                 const std::string& ProfileId,
                                 const Ecommerce::AccountTransferRequest::TransferType TransferType,
                                 const std::string& Message );

    virtual ReturnVal<std::string> mGetLiabilityAccount( const RunId &Run );

    virtual std::string mGetWithdrawalAccountNumber( const Bonus::PlayerCard& PlayerCard );

    virtual void mUpdateAccount( Ecommerce::UpdateAccountReply* Reply,
  			       const Bonus::PlayerCard& Card,
  			       const std::string &AccountNumber,
  			       const std::string &TransactionId );

    virtual void mTransactionStatus( Ecommerce::TransactionStatusReply* Reply,
                                     const Bonus::PlayerCard& Card );

    virtual void mEventComplete( Ecommerce::EventCompleteReply* Reply,
                                 Ecommerce::EventCompleteRequest* Request,
                                 bool& Ignored );

    virtual void mRemovePaymentProfile( Ecommerce::RemovePaymentProfileReply* Reply,
                                        Ecommerce::RemovePaymentProfileRequest* Request );

    virtual void mWithdrawCheck( Ecommerce::RequestCheckReply* Reply,
                                 Bonus::PlayerCard& Card,
                                 const Money& Amount,
                                 Lottery::CurrencyTransaction& CT );

    virtual void mPurchaseGift( Ecommerce::GiftCodePurchaseReply* Reply,
  			      Bonus::PlayerCard& Card,
  			      const Money& Amount,
  			      const std::string GiftCode,
  			      Lottery::CurrencyTransaction& CT );

    virtual void mRedeemGift( Ecommerce::RedeemGiftCodeReply* Reply,
  			    Bonus::PlayerCard& Card,
  			    const std::string GiftCode,
  			    Lottery::CurrencyTransaction& CT );

    virtual void mGetHostedPageDepositUrl
    ( Ecommerce::HostedPageDepositReply* Reply,
      const Ecommerce::HostedPageDepositRequest* Request );
    
    virtual bool mDeleteHostedPage( const CurrencyTransaction& CT );

    // \throw ApiStatusException if no default creditcard is found
    virtual boost::shared_ptr<EcommerceCreditCardAccount>
    mGetDefaultCreditCardAccount( const Bonus::PlayerCardId& PlayerId );

    virtual void mAddFunds
    ( FundWithPaymentProfileReply* Reply,
      FundWithPaymentProfileRequest* Request );
   
    virtual bool mCompletePending
    ( Lottery::CurrencyTransaction& Pending,
      Lottery::CurrencyTransaction& NewPending,
      RevealSystem* System, const RunId& Run, bool Redo = true );

    void mDoWin( Ecommerce::AccountTransferReply* Reply, const Bonus::PlayerCard& PlayerCard, const Money& Amount, const std::string& mCurrencyTransactionId );
    void mDoPurchase
    ( Ecommerce::AccountTransferReply* Reply, const Bonus::PlayerCard& PlayerCard,
      const Money& Amount, const std::string& mCurrencyTransactionId );
    void mDoRecurringPurchase
    ( Ecommerce::AccountTransferReply* Reply, const Bonus::PlayerCard& PlayerCard,
      const Money& Amount, const std::string& mCurrencyTransactionId );


  protected:

    virtual bool mCheckIfResultsIsRollbackable( HttpResults& Results );
    virtual bool mCheckIfValidHttpStatus( int Status )
    { return mClient.mCheckIfValidHttpStatus( Status ); }

    void mDoFundPurchase
    ( const Bonus::PlayerCard& PlayerCard, const Money& Amount,
      const CurrencyCode& CurrencyCode, const Wallet::WalletCreditProfileId& ProfileId,
      const CurrencyTransactionId& ExtendCTId );

    void mDoTransfer
    ( Ecommerce::AccountTransferReply* Reply, const Bonus::PlayerCard& PlayerCard,
      const Money& Amount, const std::string& mCurrencyTransactionId,
      Wallet::WalletAccountTransaction::Type TypeOfTransaction );

    Wallet::AuditState  mDoWithdrawToACH
    ( Ecommerce::AccountTransferReply* Reply, const Bonus::PlayerCard& PlayerCard,
      const Money& Amount, const std::string& CurrencyTransactionId, const std::string& ProfileId );

    PaysafeClient& mGetClient( void )
    { return mClient; }

    WalletAccountTransaction::Type mGetWalletAccountAdjustmentType( CurrencyTransaction::EventType CurrTransEventType);

    void mRemoveCreditProfile( Ecommerce::RemovePaymentProfileReply* Reply,
                               Bonus::PlayerCardId PCardId,
                               const std::string& ProfileId );

    void mRemoveEftProfile( Ecommerce::RemovePaymentProfileReply* Reply,
                            Bonus::PlayerCardId PCardId,
                            const std::string& ProfileId );
  private:

    PaysafeClient mClient;

    ///
    /// Copying is disabled for this class
    ///
    PaysafeApi( const PaysafeApi &Other );

    ///
    /// Assignment is disabled for this class
    ///
    PaysafeApi &operator=( const PaysafeApi &Other );

    PAYSAFEAPI_TEST_DECL;
  };
}

ENUMERATOR( PaysafeOrderState, Paysafe::PaysafeApi::OrderState );

#endif // __Paysafe_PaysafeApi_h__
