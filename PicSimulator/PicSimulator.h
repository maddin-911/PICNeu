
// PicSimulator.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'stdafx.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CPicSimulatorApp:
// Siehe PicSimulator.cpp für die Implementierung dieser Klasse
//

class CPicSimulatorApp : public CWinApp
{
public:
	CPicSimulatorApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CPicSimulatorApp theApp;