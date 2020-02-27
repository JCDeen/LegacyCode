// BitKeeper: jdeen@anya.gamelogic.com|lib/Ecommerce/InternalProviderApi.h|20160301232156|31859
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.
//
#ifndef __Ecommerce_InternalProviderApi_h__
#define __Ecommerce_InternalProviderApi_h__

#ifndef INTERNALPROVIDERAPI_TEST_DECL
#define INTERNALPROVIDERAPI_TEST_DECL
#endif

///
/// \file  InternalProviderApi.h
/// \brief Declaration of class: InternalProviderApi
///
#include "ProviderApi.h"

namespace Ecommerce
{

///
///
class InternalProviderApi : public Ecommerce::ProviderApi
{
public:
  InternalProviderApi( const RunId& );

  virtual ~InternalProviderApi( void );

  void mCreateAccount( CreateAccountReply* Reply,
                       Bonus::PlayerCard& Card,
                       SqlTransaction& Transaction );
                       
protected:
  bool mVerifyResults( HttpResults &Results,
		       bool &DataMayHaveBeenSent,
		       Ecommerce::EcommerceReply* Reply );

private:
  ///
  /// Copying is disabled for this class
  ///
  InternalProviderApi( const InternalProviderApi &Other );

  ///
  /// Assignment is disabled for this class
  ///
  InternalProviderApi &operator=( const InternalProviderApi &Other );

  INTERNALPROVIDERAPI_TEST_DECL;
};

}

#endif // __Ecommerce_InternalProviderApi_h__
