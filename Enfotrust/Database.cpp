// Copyright © 2001 - 2005 enfoTrust networks, inc.

#include "StdAfx.h"

#include "database.h"

#include "error.h"

static char fileversion[] = "$Header: /Source/Snoopy/Database.cpp 14    3/15/04 2:43p Cdeen $";

//-----------------------------------------------------------------------------

HRESULT CDBTile::Initialize()
{
	HRESULT hr;

	// call_insert
	hr = m_CallInsertCmd.SetSession(&m_pSession);
	RETIFERR(hr);
	hr = m_CallInsertCmd.SetStoredProcedure(_T("usp_gs_call_insert"),1,8,0);
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineLongParameter( 0, _T("@ReturnValue"), eDBReturnValue );
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineLongParameter( 1, _T("@RepGroupID"));
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineLongParameter( 2, _T("@RepID"));
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineLongParameter( 3, _T("@StoreID"));
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineStringParameter( 4, _T("@CallStartDateTime"), 50 );
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineStringParameter( 5, _T("@CallEndDateTime"), 50);
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineStringParameter( 6, _T("@CallSID"), 50);
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineLongParameter( 7, _T("@BeaconUsed"));
	RETIFERR(hr);
	hr = m_CallInsertCmd.DefineStringParameter( 8, _T("@ConnectionSID"), 50);
	RETIFERR(hr);
	hr = m_CallInsertCmd.Prepare();
	RETIFERR(hr);

	// questionnaire_insert
	hr = m_QuestionnaireInsertCmd.SetSession(&m_pSession);
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.SetStoredProcedure(_T("usp_gs_questionnaire_insert"),1,18,0);
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 0, _T("@ReturnValue"), eDBReturnValue );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 1,_T("@RepGroupID") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 2,_T("@RepID") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 3,_T("@StoreID") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 4, _T("@QuestionnaireStartDateTime"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 5, _T("@QuestionnaireEndDateTime"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 6, _T("@CallSID"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 7,_T("@SheetID") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 8,_T("@VendorID") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 9,_T("@DepartmentID") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 10, _T("@DeptManagerName"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 11, _T("@AsstManagerName"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 12, _T("@StoreManagerName"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 13, _T("@SignatureSID"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 14, _T("@SignatureDateTime"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 15, _T("@QuestionnaireSID"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 16,_T("@TotalSessions") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineLongParameter( 17,_T("@TotalDuration") );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.DefineStringParameter( 18, _T("@ConnectionSID"), 50 );
	RETIFERR(hr);
	hr = m_QuestionnaireInsertCmd.Prepare();
	RETIFERR(hr);

	// answer_insert
	hr = m_AnswerInsertCmd.SetSession(&m_pSession);
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.SetStoredProcedure(_T("usp_gs_answer_insert"),1,19,0);
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 0, _T("@ReturnValue"), eDBReturnValue );
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 1, _T("@AnswerSequence"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 2, _T("@QuestionSequence"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineStringParameter( 3, _T("@AnswerSID"), 50);
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineStringParameter( 4, _T("@QuestionnaireSID"), 50);
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 5, _T("@QuestionID"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 6, _T("@FollowUp1"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 7, _T("@FollowUp2"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 8, _T("@FollowUp3"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 9, _T("@FollowUp4"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 10, _T("@FollowUp5"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 11, _T("@FollowUp6"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 12, _T("@FollowUp7"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 13, _T("@FollowUp8"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 14, _T("@FollowUp9"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 15, _T("@Bool"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineStringParameter( 16, _T("@Text"), 250);
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineLongParameter( 17, _T("@ChoiceID"));
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineStringParameter( 18, _T("@PictureSID"), 50);
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.DefineStringParameter( 19, _T("@ConnectionSID"), 50);
	RETIFERR(hr);
	hr = m_AnswerInsertCmd.Prepare();
	RETIFERR(hr);

	// signature_insert
	hr = m_SignatureInsertOrUpdateCmd.SetSession(&m_pSession);
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.SetStoredProcedure(_T("usp_gs_signature_insert_or_update"),1,5,0);
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.DefineLongParameter( 0, _T("@ReturnValue"), eDBReturnValue );
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.DefineStringParameter( 1, _T("@SignatureSID"), 50 );
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.DefineStringParameter( 2, _T("@SignatureName"), 50 );
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.DefineStringParameter( 3, _T("@SignatureTitle"), 50 );
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.DefineBlobParameter( 4, _T("@Signature") );
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.DefineStringParameter( 5, _T("@ConnectionSID"), 50 );
	RETIFERR(hr);
	hr = m_SignatureInsertOrUpdateCmd.Prepare();
	RETIFERR(hr);

	return S_OK;
}

//-----------------------------------------------------------------------------

CDBCommand * CDBTile::GetCommandByName(const wstring & sCommandName)
{
	if ( sCommandName == L"usp_gs_call_insert" )
	{
		return &m_CallInsertCmd;
	}
	else if ( sCommandName == L"usp_gs_questionnaire_insert" )
	{
		return &m_QuestionnaireInsertCmd;
	}
	else if ( sCommandName == L"usp_gs_answer_insert" )
	{
		return &m_AnswerInsertCmd;
	}
	else if ( sCommandName == L"usp_gs_signature_insert_or_update" )
	{
		return &m_SignatureInsertOrUpdateCmd;
	}

	return NULL;
}

//-----------------------------------------------------------------------------

