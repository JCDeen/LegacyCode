// BitKeeper: jdeen@anya.gamelogic.com|lib/Ecommerce/ExternalProviderApi.cpp|20160701052909|61700
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.

///
/// \file ExternalProviderApi.cpp
///

#include "config.h"
#include "ProviderApi.h"
#include "ExternalProviderApi.h"
#include "Galileo/GalileoApi.h"
#include "GCA/GCAApi.h"
#include "HttpResults.h"
#include "Reveal/RevealApiDefines.h"

namespace Ecommerce
{

  ExternalProviderApi::ExternalProviderApi( const RunId& RunID )
    : ProviderApi( RunID )
  {}

  ExternalProviderApi::~ExternalProviderApi( void )
  {}

  bool ExternalProviderApi::mVerifyResults( HttpResults &Results,
                               bool &DataMayHaveBeenSent,
                               EcommerceReply* Reply )
  {
	return ProviderApi::mVerifyResults( Results, DataMayHaveBeenSent, Reply );
  }

}
