// ConfigurationSettings.h: interface for the CConfigurationSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGURATIONSETTINGS_H__9BFEFA09_6B91_4521_A6BC_BA0443E576F8__INCLUDED_)
#define AFX_CONFIGURATIONSETTINGS_H__9BFEFA09_6B91_4521_A6BC_BA0443E576F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MSG_SIZE 150

class CConfigurationSettings  
{
public:
	void ResetAllSettings();
	void ResetConfigureDlgSettings();
	void WriteInstanceRecord();
	bool IsFirstInstance();

	CConfigurationSettings();
	virtual ~CConfigurationSettings();

	bool IntToBool(int nIntVal);
	int BoolToInt(bool mBoolValue);

	CWinApp *pApp;

	void WriteAllSettings();
	void WriteConfigureDlgSettings();
	void GetConfigureDlgSettings();
	void GetAllSettings();

	bool m_bAutoConnect;

	bool m_bEchoToCmdTrail;
	bool m_bMessageBox;
	bool m_bTriggerRepeatedly;
	char m_szNotificationMessage[MAX_MSG_SIZE + 1];

	bool m_bAddLineNumbers;
	bool m_bEchoAppDebugging;
	bool m_bSaveToFileOnExit;

	char m_szDigitalOutPreset0[12];
	char m_szDigitalOutPreset1[12];
	char m_szDigitalOutPreset2[12];
	char m_szDigitalOutPreset3[12];
	char m_szDigitalInputTriggerVal[12];
	bool m_bEnableDigitalWordTrigger;

	int m_nAnalogDataCaptureMode;

	bool m_bShowDigitalAsBinary;
	bool m_bShowAnalogAsRaw;

	bool m_bEnableAnalogScanning;
	bool m_bEnableDigitalScanning;

	bool m_bShowAboutBoxAtStartup;

	bool m_bContinousScan;

	int	m_nTimerInterval;

	char m_szIP_Address[50]; // What should max size be ?
	int m_nPortNumber;
};

#endif // !defined(AFX_CONFIGURATIONSETTINGS_H__9BFEFA09_6B91_4521_A6BC_BA0443E576F8__INCLUDED_)
