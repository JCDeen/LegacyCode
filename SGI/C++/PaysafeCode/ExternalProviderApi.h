// BitKeeper: jdeen@anya.gamelogic.com|lib/Ecommerce/ExternalProviderApi.h|20160301232156|34444
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.
//
#ifndef __Ecommerce_ExternalProviderApi_h__
#define __Ecommerce_ExternalProviderApi_h__

#ifndef EXTERNALPROVIDERAPI_TEST_DECL
#define EXTERNALPROVIDERAPI_TEST_DECL
#endif

///
/// \file  ExternalProviderApi.h
/// \brief Declaration of class: ExternalProviderApi
///

#include "ProviderApi.h"

namespace Ecommerce
{

///
///
class ExternalProviderApi : public ProviderApi
{
public:
  ExternalProviderApi( const RunId& );

  virtual ~ExternalProviderApi( void );

  // \throw ApiStatusException
  virtual void mCreateAccount( CreateAccountReply* Reply,
                               Bonus::PlayerCard& Card,
                               SqlTransaction& Transaction ) = 0;

protected:
  bool mVerifyResults( HttpResults &Results,
		       bool &DataMayHaveBeenSent,
		       Ecommerce::EcommerceReply* Reply );

private:
  ///
  /// Copying is disabled for this class
  ///
  ExternalProviderApi( const ExternalProviderApi &Other );

  ///
  /// Assignment is disabled for this class
  ///
  ExternalProviderApi &operator=( const ExternalProviderApi &Other );

  EXTERNALPROVIDERAPI_TEST_DECL;
};

}

#endif // __Ecommerce_ExternalProviderApi_h__
