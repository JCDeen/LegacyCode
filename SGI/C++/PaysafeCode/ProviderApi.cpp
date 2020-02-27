// BitKeeper: bhashyam@serak.gamelogic.com|lib/Ecommerce/ProviderApi.cpp|20170118193749|14183
// Copyright(C) 2012 Gamelogic Inc. All rights reserved.

///
/// \file ProviderApi.cpp
///

#include "config.h"
#include "ProviderApi.h"
#include "Galileo/GalileoApi.h"
#include "GCA/GCAApi.h"
#include "Paysafe/PaysafeApi.h"
#include "HttpResults.h"
#include "Reveal/RevealApiDefines.h"

using namespace std;

namespace Ecommerce
{

  ProviderApi::ProviderApi( const RunId& RunID ) :
      mVerboseClient( false ),
      mRunId( RunID )
  {}

  ProviderApi::~ProviderApi( void )
  {}

  boost::shared_ptr<ProviderApi> ProviderApi::mCreate( const RunId& RunID )
  {

    // These next 3 blocks of code are essentially duplicated in PaysafeClient.cpp,
    // do they need to be ? 
    if( ! RunID.mHasValidFormat() )
    {
      WARNS( "RunID has invalid format" );
    }

    Reveal::RunData RunData( RunID );
    if( ! RunData )
    { WARNS( "unable to load rundata for run:" << RunID ); }
    
    // handle providers based on run.
    int NumProviderNodes = 0;

    ConfigNode *ProviderNode = RunData->mFind( "PaymentProvider" );
    if ( !ProviderNode )
    {
      WARNS( "PaymentProvider node not found for run:" << RunID );
    }
    else
    {
      NumProviderNodes = ProviderNode->mGetNumberOfChildren();
    }
    
    if ( NumProviderNodes == 1 )
    {
      // We really need to enumerate these better. Done this way due to time crunch.
      if ( RunData->mFind( "GCA" ) )
      {
        return boost::shared_ptr<ProviderApi>( new GCA::GCAApi( RunID ) );
      }
      else if ( RunData->mFind( "Galileo" ) )
      {
	return boost::shared_ptr<ProviderApi>( new Galileo::GalileoApi( RunID ) );
      }
      else  if ( RunData->mFind( "Paysafe" ) )
      {
	return boost::shared_ptr<ProviderApi>( new Paysafe::PaysafeApi( RunID ) );
      }
      else
      {
	PAGE( "Unexpected PaymentProvider node found");
	throw ApiStatusException( RevealApi::cStatusInvalidRequest );
      }
    }
    else
    {
      PAGE( "PaymentProvider node has incorrect number of provider nodes: %d", NumProviderNodes);
      throw ApiStatusException( RevealApi::cStatusInvalidRequest ) ;
    }
  
    // We should default to GCA. But should we instead throw an exception here ?
    return boost::shared_ptr<ProviderApi>( new GCA::GCAApi( RunID ) );
  }

  bool ProviderApi::mVerifyResults( HttpResults &Results,
                       bool &DataMayHaveBeenSent,
                       Ecommerce::EcommerceReply* Reply )
  {
    std::string ErrorMsg = "";

    /// Check the results
    if( ! Results.mIsSuccess() )
    {

	  /// Clean up the error message so we don't log GCA authentication data.
      /// We need to remove the Url{ .... } section from the error message.
      if( Results.mGetStatus() == eHttpResultsStatusSeeMessages )
      {
        size_t index = Results.mGetErrorMessage().find( "Url{" );
        if( index != std::string::npos )
	    {
          ErrorMsg = Results.mGetErrorMessage().substr( 0, index );
          index = Results.mGetErrorMessage().find( "Error{" );
          if( index != std::string::npos )
          { ErrorMsg.append( Results.mGetErrorMessage().substr( index ) ); }
        }
        else
        { ErrorMsg = Results.mGetErrorMessage(); }
      }

      if( DataMayHaveBeenSent )
      {
        WARNS( "Request failed may have been sent. Error: "
               << ErrorMsg );
        Reply->mSetRollbackState( Ecommerce::EcommerceReply::eCannotRollBack );
      }
      else
      {
        WARNS( "Request failed was not sent. Error: "
               << ErrorMsg );
        Reply->mSetRollbackState( Ecommerce::EcommerceReply::eCanRollBack );
      }
      throw ApiStatusException( RevealApi::cStatusInvalidRequest );
    }

    /// Check if HTTP Status indicates non-rollbackable state
    else if( ! mCheckIfValidHttpStatus( Results.mGetHttpStatusCode() ) )
    {
      Reply->mSetRollbackState( Ecommerce::EcommerceReply::eCannotRollBack );
      WARNS( "Non-rollbackable http status: " << Results.mGetHttpStatusCode() );
      throw ApiStatusException( RevealApi::cStatusInvalidRequest );
    }

    // Check if HTTP status indicates rollbackable state
    else if( mCheckIfHttpStatusRollbackable( Results.mGetHttpStatusCode() ) )
    {
      WARNS( "Rollbackable Status Code: " << Results.mGetHttpStatusCode() );
      Reply->mSetRollbackState( Ecommerce::EcommerceReply::eCanRollBack );
      throw ApiStatusException( RevealApi::cStatusInvalidRequest );
    }

    /// Check for returned response data
    else if( ! ( Results.mGetResultsSize() > 0 ) )
    {
      Reply->mSetRollbackState( Ecommerce::EcommerceReply::eCannotRollBack );
      WARNS( "Request failed. can't rollback. Error: "
             << ErrorMsg );
      throw ApiStatusException( RevealApi::cStatusInvalidRequest );
    }

    else if( mCheckIfResultsIsRollbackable( Results ) )
    {
      Reply->mSetRollbackState( Ecommerce::EcommerceReply::eCanRollBack );
      WARNS( "Request failed but can be rolled back. Error: "
             << ErrorMsg );
      throw ApiStatusException( RevealApi::cStatusInvalidRequest );
    }

    // if we got here, everything should be fine (from the HTTP standpoint)
    return true;
  }

}
