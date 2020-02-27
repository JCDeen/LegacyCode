// BitKeeper: rahul@uttara.gamelogic.com|lib/Ecommerce/Paysafe/PaysafeHelper.h|20160304124922|25097
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.
//
#ifndef __Paysafe_PaysafeHelper_h__
#define __Paysafe_PaysafeHelper_h__

#ifndef PAYSAFEHELPER_TEST_DECL
#define PAYSAFEHELPER_TEST_DECL
#endif

///
/// \file  PaysafeHelper.h
/// \brief Declaration of class: PaysafeHelper
///

#include "Storeable.h"

namespace Paysafe
{

///
///
class PaysafeHelper
{
public:
  PaysafeHelper( );
  virtual ~PaysafeHelper( );
  // As of now CAD and USD only

  // return mm/dd/YYYY HH:MI:SS UTC using incoming timestamp
  // of time_t (in string format)
  std::string mConvertFromTimestamp( const time_t TimeStamp ) const;


  // return time_t (in string format) using incoming timestamp
  // of mm/dd/YYYY HH:MI:SS UTC
  std::string mParseTimestamp( const std::string TimeStamp ) const;

private:
  ///
  /// Copying is disabled for this class
  ///
  PaysafeHelper( const PaysafeHelper &Other );

  ///
  /// Assignment is disabled for this class
  ///
  PaysafeHelper &operator=( const PaysafeHelper &Other );

  PAYSAFEHELPER_TEST_DECL;
};

}

#endif // __Paysafe_PaysafeHelper_h__
