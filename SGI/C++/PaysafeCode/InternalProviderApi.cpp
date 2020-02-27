// BitKeeper: jdeen@anya.gamelogic.com|lib/Ecommerce/InternalProviderApi.cpp|20160711163643|22541
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.

///
/// \file InternalProviderApi.cpp
///

#include "config.h"
#include "ProviderApi.h"
#include "InternalProviderApi.h"
#include "HttpResults.h"

namespace Ecommerce
{
  InternalProviderApi::InternalProviderApi( const RunId& RunID )
    : ProviderApi( RunID )
  {}

  InternalProviderApi::~InternalProviderApi( void )
  {}

  // For now this is an empty implementation since connection is internal.
  bool InternalProviderApi::mVerifyResults( HttpResults &Results,
                               bool &DataMayHaveBeenSent,
                               Ecommerce::EcommerceReply* Reply )
  {
    if( Results.mGetStatus() == eHttpResultsStatusNotSet )
    { 
      // no HTTP - so nothing to verify, return success
      return true;
    }
    else
    {
      return ProviderApi::mVerifyResults( Results, DataMayHaveBeenSent, Reply );
    }
  }
  
  void InternalProviderApi::mCreateAccount( CreateAccountReply* Reply,
                       Bonus::PlayerCard& Card,
                       SqlTransaction& Transaction )
  {}
     
}
