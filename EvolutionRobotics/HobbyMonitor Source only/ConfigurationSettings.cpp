// ConfigurationSettings.cpp: implementation of the CConfigurationSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HobbyIOMonitor.h"
#include "ConfigurationSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfigurationSettings::CConfigurationSettings()
{
	pApp = AfxGetApp();
//	TRACE("Application name is %s\n", pApp->m_pszExeName);
}

CConfigurationSettings::~CConfigurationSettings()
{

}

void CConfigurationSettings::GetAllSettings()
{
	////////////////////////////////////
	// -------- Main Dialog --------- //
	////////////////////////////////////
	// Analog Inputs:
	m_nAnalogDataCaptureMode = pApp->GetProfileInt("Analog Inputs", "Readings Mode", 0); // 0=floating; 1=stickey lows; 2= stickey highs.
	m_bShowAnalogAsRaw = IntToBool(pApp->GetProfileInt("Analog Inputs", "Display Format", 1));
	m_bEnableAnalogScanning = IntToBool(pApp->GetProfileInt("Analog Inputs", "Enable Analog Scanning", 1));

	// Digital Inputs:
	m_bShowDigitalAsBinary = IntToBool(pApp->GetProfileInt("Digital Inputs", "Display Format", 1));
	m_bEnableDigitalScanning = IntToBool(pApp->GetProfileInt("Digital Inputs", "Enable Digital Scanning", 1));
	m_bEnableDigitalWordTrigger = IntToBool(pApp->GetProfileInt("Digital Inputs", "Enable Digital Word Trigger", 1));

	// Scan Settings:
  	m_bContinousScan = IntToBool(pApp->GetProfileInt("Scan Settings", "Continuous Scan", 0));
	m_nTimerInterval = pApp->GetProfileInt("Scan Settings", "Scan Interval", 150);

	// Socket Connection:
	strcpy(m_szIP_Address, pApp->GetProfileString("Socket Connection", "IP Address", "127.0.0.1"));
	m_nPortNumber = pApp->GetProfileInt("Socket Connection", "Port Number", 9000);

	///////////////////////////////////
	// Get the rest of the settings. //
	///////////////////////////////////
	GetConfigureDlgSettings();
}

void CConfigurationSettings::GetConfigureDlgSettings()
{
	// Digital Word Trigger:
	m_bEchoToCmdTrail = IntToBool(pApp->GetProfileInt("Digital Word Trigger", "EchoToCmdTrail", 1));
	m_bMessageBox = IntToBool(pApp->GetProfileInt("Digital Word Trigger", "MessageBox ", 0));
	m_bTriggerRepeatedly = IntToBool(pApp->GetProfileInt("Digital Word Trigger", "TriggerRepeatedly", 0));
	strcpy(m_szDigitalInputTriggerVal, pApp->GetProfileString("Digital Word Trigger", "InputTriggerValue", "00000001"));
	strncpy(m_szNotificationMessage, pApp->GetProfileString("Digital Word Trigger", "NotificationMessage", "Digital Input Word Triggered"), MAX_MSG_SIZE);
	m_szNotificationMessage[MAX_MSG_SIZE] = '\0';

	// Digital Output Presets:
	strcpy(m_szDigitalOutPreset0, pApp->GetProfileString("Digital Output Presets", "DO0 Value ", "00000000"));
	strcpy(m_szDigitalOutPreset1, pApp->GetProfileString("Digital Output Presets", "DO1 Value ", "00001111"));
	strcpy(m_szDigitalOutPreset2, pApp->GetProfileString("Digital Output Presets", "DO2 Value ", "11110000"));
	strcpy(m_szDigitalOutPreset3, pApp->GetProfileString("Digital Output Presets", "DO3 Value ", "11111111"));

	// Cmd Trail Settings:
  	m_bAddLineNumbers = IntToBool(pApp->GetProfileInt("Cmd Trail Settings", "AddLineNumbers", 1));
  	m_bEchoAppDebugging = IntToBool(pApp->GetProfileInt("Cmd Trail Settings", "EchoDebugging", 0));
  	m_bSaveToFileOnExit = IntToBool(pApp->GetProfileInt("Cmd Trail Settings", "SaveToFileOnExit", 0));

	// Misc Settings:
  	//m_bLabel_AI_15AsBattery = IntToBool(pApp->GetProfileInt("Misc Settings", "Label AI15 as Battery", 0));
  	m_bAutoConnect  = IntToBool(pApp->GetProfileInt("Misc Settings", "AutoConnect", 0));
	m_bShowAboutBoxAtStartup = IntToBool(pApp->GetProfileInt("Misc Settings", "ShowLicenseAtStartup", 0));
}

void CConfigurationSettings::WriteAllSettings()
{
	///////////////////////////////////////////
	// Writes evidence that this application // 
	// has been run at least once.           //
	///////////////////////////////////////////
	WriteInstanceRecord();

	////////////////////////////////////
	// -------- Main Dialog --------- //
	////////////////////////////////////

	// Analog Inputs:
	pApp->WriteProfileInt("Analog Inputs", "Readings Mode", m_nAnalogDataCaptureMode); // 0=floating; 1=stickey lows; 2= stickey highs.
	pApp->WriteProfileInt("Analog Inputs", "Display Format", BoolToInt(m_bShowAnalogAsRaw));
	pApp->WriteProfileInt("Analog Inputs", "Enable Analog Scanning", BoolToInt(m_bEnableAnalogScanning));

	// Digital Inputs:
	pApp->WriteProfileInt("Digital Inputs", "Display Format", BoolToInt(m_bShowDigitalAsBinary));
	pApp->WriteProfileInt("Digital Inputs", "Enable Digital Scanning", BoolToInt(m_bEnableDigitalScanning));
	pApp->WriteProfileInt("Digital Inputs", "Enable Digital Word Trigger", BoolToInt(m_bEnableDigitalWordTrigger));

	// Scan Settings:
  	pApp->WriteProfileInt("Scan Settings", "Continuous Scan", BoolToInt(m_bContinousScan));
	pApp->WriteProfileInt("Scan Settings", "Scan Interval", m_nTimerInterval);

	// Socket Connection:
	pApp->WriteProfileString("Socket Connection", "IP Address", m_szIP_Address);
	pApp->WriteProfileInt("Socket Connection", "Port Number", m_nPortNumber);

	///////////////////////////////////
	// Get the rest of the settings. //
	///////////////////////////////////
	WriteConfigureDlgSettings();
}

void CConfigurationSettings::WriteConfigureDlgSettings()
{
	// Digital Word Trigger:
	pApp->WriteProfileInt("Digital Word Trigger", "EchoToCmdTrail", BoolToInt(m_bEchoToCmdTrail));
	pApp->WriteProfileInt("Digital Word Trigger", "MessageBox ", BoolToInt(m_bMessageBox));
	pApp->WriteProfileInt("Digital Word Trigger", "TriggerRepeatedly", BoolToInt(m_bTriggerRepeatedly));
	pApp->WriteProfileString("Digital Word Trigger", "InputTriggerValue", m_szDigitalInputTriggerVal);
	pApp->WriteProfileString("Digital Word Trigger", "NotificationMessage", m_szNotificationMessage);

	// Digital Output Presets:
	pApp->WriteProfileString("Digital Output Presets", "DO0 Value ", m_szDigitalOutPreset0);
	pApp->WriteProfileString("Digital Output Presets", "DO1 Value ", m_szDigitalOutPreset1);
	pApp->WriteProfileString("Digital Output Presets", "DO2 Value ", m_szDigitalOutPreset2);
	pApp->WriteProfileString("Digital Output Presets", "DO3 Value ", m_szDigitalOutPreset3);

	// Cmd Trail Settings:
  	pApp->WriteProfileInt("Cmd Trail Settings", "AddLineNumbers", BoolToInt(m_bAddLineNumbers));
  	pApp->WriteProfileInt("Cmd Trail Settings", "EchoDebugging", BoolToInt(m_bEchoAppDebugging));
  	pApp->WriteProfileInt("Cmd Trail Settings", "SaveToFileOnExit", BoolToInt(m_bSaveToFileOnExit));

	// Misc Settings:
  	//pApp->WriteProfileInt("Misc Settings", "Label AI15 as Battery", BoolToInt(m_bLabel_AI_15AsBattery));
  	pApp->WriteProfileInt("Misc Settings", "AutoConnect", BoolToInt(m_bAutoConnect));
  	pApp->WriteProfileInt("Misc Settings", "ShowLicenseAtStartup", BoolToInt(m_bShowAboutBoxAtStartup));
}

bool CConfigurationSettings::IntToBool(int nIntVal)
{
	if (nIntVal != 0)
		return true;
	else
		return false;
}

int CConfigurationSettings::BoolToInt(bool mBoolValue)
{
	if (mBoolValue)
		return 1;
	else
		return 0;
}

bool CConfigurationSettings::IsFirstInstance()
{
	if (pApp->GetProfileInt("Execution Audit", "HasBeenRun", 0) == 0)
		return true;
	else
		return false;
}

/***
char dbuffer [9];
   char tbuffer [9];
   _strdate( dbuffer );
   printf( "The current date is %s \n", dbuffer );
   _strtime( tbuffer );
   printf( "The current time is %s \n", tbuffer );
}
****/

void CConfigurationSettings::WriteInstanceRecord()
{
	pApp->WriteProfileInt("Execution Audit", "HasBeenRun", 1);
}

void CConfigurationSettings::ResetConfigureDlgSettings()
{
	m_bEchoToCmdTrail = true;
	m_bMessageBox = false;
	m_bTriggerRepeatedly = false;
	strcpy(m_szDigitalInputTriggerVal, "00000001");
	strcpy(m_szNotificationMessage, "Digital Input Word Triggered");

	// Digital Output Presets:
	strcpy(m_szDigitalOutPreset0, "00000000");
	strcpy(m_szDigitalOutPreset1, "00001111");
	strcpy(m_szDigitalOutPreset2, "11110000");
	strcpy(m_szDigitalOutPreset3, "11111111");

	// Cmd Trail Settings:
  	m_bAddLineNumbers = true;
  	m_bEchoAppDebugging = false;
  	m_bSaveToFileOnExit = false;

	// Misc Settings:
  	//m_bLabel_AI_15AsBattery = false;
  	m_bAutoConnect = false;
	m_bShowAboutBoxAtStartup = true;
}

void CConfigurationSettings::ResetAllSettings()
{
	////////////////////////////////////
	// -------- Main Dialog --------- //
	////////////////////////////////////
	// Analog Inputs:
	m_nAnalogDataCaptureMode = 0; // 0=floating; 1=stickey lows; 2= stickey highs.
	m_bShowAnalogAsRaw = true;
	m_bEnableAnalogScanning = true;

	// Digital Inputs:
	m_bShowDigitalAsBinary = true;
	m_bEnableDigitalScanning = true;
	m_bEnableDigitalWordTrigger = true;

	// Scan Settings:
  	m_bContinousScan = false;
	m_nTimerInterval = 150;

	// Socket Connection:
	strcpy(m_szIP_Address, "127.0.0.1");
	m_nPortNumber = 9000;


	/////////////////////////////////////
	// Reset the rest of the settings. //
	/////////////////////////////////////
	ResetConfigureDlgSettings();
}
