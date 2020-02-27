// BitKeeper: rahul@uttara.gamelogic.com|lib/Ecommerce/Paysafe/PaysafeDefines.h|20160304124922|44916
// Copyright(C) 2016 Gamelogic Inc. All rights reserved.
//
#ifndef __Paysafe_PaysafeDefines_h__
#define __Paysafe_PaysafeDefines_h__

#ifndef PAYSAFEDEFINES_TEST_DECL
#define PAYSAFEDEFINES_TEST_DECL
#endif

///
/// \file  PaysafeDefines.h
/// \brief Declaration of class: PaysafeDefines
///

#include "Reveal/RevealApiDefines.h"

namespace Paysafe
{

/*// This will be used for internal reference, can convert to string while sending the request
enum CurrencyCode
{
  ENUM( "CAD", eCurrencyCodeCAD = 0 ),
  ENUM( "USD", eCurrencyCodeUSD = 1 )
};*/

enum PaymentMethod
{
  ENUM( "Credit", ePaymentMethodVisa = 0 ),
  ENUM( "Master", ePaymentMethodMaster = 1 ),
  ENUM( "Interac", ePaymentMethodInterac = 2 )
};

// As same is used in PaysafeSetlement as well, we will be moving this to common place while merging
enum PaysafeState
{
  ENUM( "Failed", ePaysafeStateFailed = -1 ),
  ENUM( "Pending", ePaysafeStatePending  = 0 ),
  ENUM( "Received", ePaysafeStateReceived  = 1 ),
  ENUM( "Completed", ePaysafeStateCompleted  = 2 ),
  ENUM( "Processing", ePaysafeStateProcessing = 3 ),
  ENUM( "Cancelled", ePaysafeStateeCancelled = 4 )
};

// Authorization/Callback will be added later
enum AuthType
{
  ENUM( "settlement", eAuthTypesettlement = 0 ),
  ENUM( "Order", eAuthTypeOrder  = 1 )
};


}

#endif // __Paysafe_PaysafeDefines_h__
