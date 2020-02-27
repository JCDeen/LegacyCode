// BitKeeper: rahul@uttara.gamelogic.com|lib/Ecommerce/Paysafe/PaysafeHelper.cpp|20160304124922|48310
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.

///
/// \file PaysafeHelper.cpp
///

#include "config.h"
#include "PaysafeHelper.h"
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/time_facet.hpp>

namespace Paysafe
{


PaysafeHelper::PaysafeHelper( )
{ }

PaysafeHelper::~PaysafeHelper( )
{ }

std::string PaysafeHelper::mConvertFromTimestamp( const time_t TimeStamp ) const
{
  boost::posix_time::ptime PTime = boost::posix_time::from_time_t( TimeStamp );
  std::stringstream DateStream;
  boost::posix_time::time_facet* TimeFacet =
                                 new boost::posix_time::time_facet( "%Y-%m-%dT%H:%M:%SZ%ZP" );
  DateStream.imbue( std::locale( std::locale::classic( ), TimeFacet ) );
  DateStream << PTime;
  return DateStream.str( );
}

std::string PaysafeHelper::mParseTimestamp( const std::string TimeStamp ) const
{
  std::stringstream TimestampString;
  std::istringstream ss( TimeStamp );
  ss.imbue( std::locale( std::locale::classic( ),
                         new boost::local_time::local_time_input_facet
                         ( "%Y-%m-%d %H:%M:%S %ZP" ) ) );
  boost::posix_time::ptime PTime;
  ss >> PTime;
  boost::posix_time::ptime EpochPtime( boost::gregorian::date( 1970, 1, 1 ) );
  TimestampString << ( PTime - EpochPtime ).total_seconds( );
  return TimestampString.str( );
}

}
