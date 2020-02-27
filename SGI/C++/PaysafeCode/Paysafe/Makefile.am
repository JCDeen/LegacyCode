# BitKeeper: bhashyam@serak.gamelogic.com|lib/Ecommerce/Paysafe/Makefile.am|20170113025136|41686
# Copyright(C) 2012 Gamelogic Inc. All rights reserved.

include $(top_srcdir)/Makefile.rules

SUBDIRS = PaysafeStoreables

lib_LTLIBRARIES = libproduct-paysafe.la

LIBPAYSAFESOURCES = \
  PaysafeApi.cpp \
  PaysafeClient.cpp \
  PaysafeCreateAccount.cpp \
  PaysafeCreateAdjustment.cpp \
  PaysafeRemovePaymentProfile.cpp \
  PaysafeHelper.cpp \
  PaysafeOrderStateGenerated.cpp \
  PaysafeEventComplete.cpp \
  PaysafeGetAllAccounts.cpp \
  PaysafeAddFunds.cpp \
  PaysafeCreateAccountTransfer.cpp \
  PaysafeAdjustment.cpp \
  PaysafeGetHostedPageDepositUrl.cpp \
  PaysafeDeleteHostedPage.cpp \
  PaysafeTransactionStatus.cpp \
  PaysafeCompletePending.cpp

BUILT_SOURCES =  \
  PaysafeHelper.h \
  PaysafeOrderStateGenerated.cpp \
  PaysafeDefines.h 

libproduct_paysafe_la_SOURCES = ${LIBPAYSAFESOURCES}
libproduct_paysafe_la_LIBADD = \
  PaysafeStoreables/libproduct-paysafestoreables.la

includedir = @includedir@/Paysafe

INCLUDES = \
  -I@common_library_includes@ -I@abs_top_srcdir@/lib -I@abs_top_builddir@/lib \
  -I@abs_top_builddir@  -I@php_includes@  -I/usr/include/libxml2

PaysafeOrderState.gperf: $(srcdir)/PaysafeApi.h
	@RIPCONSTANTS@ -gp $@ \
	  $(srcdir)/PaysafeApi.h:OrderState:PaysafeApi

