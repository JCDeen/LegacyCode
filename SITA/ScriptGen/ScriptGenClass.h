// ScriptGenClass.h: interface for the ScriptGenClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ScriptGenClass_H__4800E6E0_DAFF_11D2_80F0_006008BFC7B0__INCLUDED_)
#define AFX_ScriptGenClass_H__4800E6E0_DAFF_11D2_80F0_006008BFC7B0__INCLUDED_

#pragma message ("ScriptGenClass.h included ....")

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define Windows 1       // Windows
//#define Windows 0     // Unix

#define CMXG_NOT_FOUND THE_SAFE_CHAR

class ScriptGenClass
{
public:
	ScriptGenClass();
	virtual ~ScriptGenClass();

	int m_nCommandLineDebugMode;
	char *m_pMainframeStatusText;

	char *m_pCityCodeAlpha;
	char *m_pCityAirportCodeA[5], *m_pCityAirportNameA[5], *m_pCityAirportCodeB[5], *m_pCityAirportNameB[5];

protected:
	bool m_bError;	// Status of latest operation.
	int	 m_nDebug;

public:
	/////////////////
	// Operations. //
	/////////////////

	bool GetErrorStatus()		{ return m_bError; }; // FALSE == no error.

	void Simulated_GetDataFromInputFile();
	void Pre_ParseDataFromInputFile();
	void ParseDataFromInputFile();
	void BuildOutputToFile();

};

#endif // !defined(AFX_ScriptGenClass_H__4800E6E0_DAFF_11D2_80F0_006008BFC7B0__INCLUDED_)
