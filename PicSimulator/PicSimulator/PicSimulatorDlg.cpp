
// PicSimulatorDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "PicSimulator.h"
#include "PicSimulatorDlg.h"
#include "afxdialogex.h"
#include "windows.h"

#include "PIC16F84.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_UPDATE_CONTROL    WM_APP + 0x10
#define WM_BREAK    WM_APP + 0x20

// CAboutDlg-Dialogfeld für Anwendungsbefehl "Info"

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

// Implementierung
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPicSimulatorDlg-Dialogfeld



CPicSimulatorDlg::CPicSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PICSIMULATOR_DIALOG, pParent)
	, sliderFrq(0)
	, checkBank0Sram(0)
	, checkBank1Sram(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPicSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REG_W, lblRegW);
	DDX_Control(pDX, IDC_REG_PLC, lblPLC);
	DDX_Control(pDX, IDC_REG_TIMER0, lblTimer0);
	DDX_Control(pDX, LBL_FREQ, lblFreq);
	DDX_Control(pDX, IDC_REG_C, lblRegC);
	DDX_Control(pDX, IDC_REG_CD, lblRegDC);
	DDX_Control(pDX, IDC_REG_Z, lblRegZ);
	DDX_Control(pDX, IDC_LIST_PROG_MEM, lstProgMem);
	DDX_Control(pDX, CHECK_TRISB_7, checkTrisb7);
	DDX_Control(pDX, CHECK_TRISB_6, checkTrisb6);
	DDX_Control(pDX, CHECK_TRISB_5, checkTrisb5);
	DDX_Control(pDX, CHECK_TRISB_4, checkTrisb4);
	DDX_Control(pDX, CHECK_TRISB_3, checkTrisb3);
	DDX_Control(pDX, CHECK_TRISB_2, checkTrisb2);
	DDX_Control(pDX, CHECK_TRISB_1, checkTrisb1);
	DDX_Control(pDX, CHECK_TRISB_0, checkTrisb0);
	DDX_Control(pDX, CHECK_TRISA_0, checkTrisa0);
	DDX_Control(pDX, CHECK_TRISA_1, checkTrisa1);
	DDX_Control(pDX, CHECK_TRISA_2, checkTrisa2);
	DDX_Control(pDX, CHECK_TRISA_3, checkTrisa3);
	DDX_Control(pDX, CHECK_TRISA_4, checkTrisa4);
	DDX_Control(pDX, CHECK_A0, checkPortA0);
	DDX_Control(pDX, CHECK_A1, checkPortA1);
	DDX_Control(pDX, CHECK_A2, checkPortA2);
	DDX_Control(pDX, CHECK_A3, checkPortA3);
	DDX_Control(pDX, CHECK_A4, checkPortA4);
	DDX_Control(pDX, CHECK_B0, checkPortB0);
	DDX_Control(pDX, CHECK_B1, checkPortB1);
	DDX_Control(pDX, CHECK_B2, checkPortB2);
	DDX_Control(pDX, CHECK_B3, checkPortB3);
	DDX_Control(pDX, CHECK_B4, checkPortB4);
	DDX_Control(pDX, CHECK_B5, checkPortB5);
	DDX_Control(pDX, CHECK_B6, checkPortB6);
	DDX_Control(pDX, CHECK_B7, checkPortB7);
	DDX_Control(pDX, CHECK_OPTION0, checkOption0);
	DDX_Control(pDX, CHECK_OPTION1, checkOption1);
	DDX_Control(pDX, CHECK_OPTION2, checkOption2);
	DDX_Control(pDX, CHECK_OPTION3, checkOption3);
	DDX_Control(pDX, CHECK_OPTION4, checkOption4);
	DDX_Control(pDX, CHECK_OPTION5, checkOption5);
	DDX_Control(pDX, CHECK_OPTION6, checkOption6);
	DDX_Control(pDX, CHECK_OPTION7, checkOption7);
	DDX_Control(pDX, CHECK_INTCON0, checkIntcon0);
	DDX_Control(pDX, CHECK_INTCON1, checkIntcon1);
	DDX_Control(pDX, CHECK_INTCON2, checkIntcon2);
	DDX_Control(pDX, CHECK_INTCON3, checkIntcon3);
	DDX_Control(pDX, CHECK_INTCON4, checkIntcon4);
	DDX_Control(pDX, CHECK_INTCON5, checkIntcon5);
	DDX_Control(pDX, CHECK_INTCON6, checkIntcon6);
	DDX_Control(pDX, CHECK_INTCON7, checkIntcon7);
	DDX_Control(pDX, CHECK_STATUS0, checkStatus0);
	DDX_Control(pDX, CHECK_STATUS1, checkStatus1);
	DDX_Control(pDX, CHECK_STATUS2, checkStatus2);
	DDX_Control(pDX, CHECK_STATUS3, checkStatus3);
	DDX_Control(pDX, CHECK_STATUS4, checkStatus4);
	DDX_Control(pDX, CHECK_STATUS5, checkStatus5);
	DDX_Control(pDX, CHECK_STATUS6, checkStatus6);
	DDX_Control(pDX, CHECK_STATUS7, checkStatus7);
	DDX_Control(pDX, CHECK_INTCON0, checkIntcon0);
	DDX_Control(pDX, CHECK_INTCON1, checkIntcon1);
	DDX_Control(pDX, CHECK_INTCON2, checkIntcon2);
	DDX_Control(pDX, CHECK_INTCON3, checkIntcon3);
	DDX_Control(pDX, CHECK_INTCON4, checkIntcon4);
	DDX_Control(pDX, CHECK_INTCON5, checkIntcon5);
	DDX_Control(pDX, CHECK_INTCON6, checkIntcon6);
	DDX_Control(pDX, CHECK_INTCON7, checkIntcon7);
	DDX_Control(pDX, CHECK_STATUS0, checkStatus0);
	DDX_Control(pDX, CHECK_STATUS1, checkStatus1);
	DDX_Control(pDX, CHECK_STATUS2, checkStatus2);
	DDX_Control(pDX, CHECK_STATUS3, checkStatus3);
	DDX_Control(pDX, CHECK_STATUS4, checkStatus4);
	DDX_Control(pDX, CHECK_STATUS5, checkStatus5);
	DDX_Control(pDX, CHECK_STATUS6, checkStatus6);
	DDX_Control(pDX, CHECK_STATUS7, checkStatus7);
	DDX_Control(pDX, IDC_LIST_SRAM, lstSram);
	DDX_Control(pDX, IDC_LIST_STACK, lstStack);
	DDX_Control(pDX, IDC_SLEEP, lblSleep);
	DDX_Control(pDX, CHECK_WDT, checkWDT);
	DDX_Control(pDX, IDC_BUTTON_RUN, btnRun);
	DDX_Control(pDX, IDC_SLIDER_FREQ, freqSlider);
	DDX_Control(pDX, IDC_LIST_EEPROM, lstEEprom);
	DDX_Control(pDX, IDC_EDIT1, sramEdit);
	DDX_Control(pDX, IDC_EDIT2, sramAddr);
}

BEGIN_MESSAGE_MAP(CPicSimulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CPicSimulatorDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CPicSimulatorDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_STEP, &CPicSimulatorDlg::OnBnClickedButtonStep)
	ON_BN_CLICKED(CHECK_TRISB_7, &CPicSimulatorDlg::OnBnClickedTrisb7)
	ON_BN_CLICKED(CHECK_TRISB_6, &CPicSimulatorDlg::OnBnClickedTrisb6)
	ON_BN_CLICKED(CHECK_TRISB_5, &CPicSimulatorDlg::OnBnClickedTrisb5)
	ON_BN_CLICKED(CHECK_TRISB_4, &CPicSimulatorDlg::OnBnClickedTrisb4)
	ON_BN_CLICKED(CHECK_TRISB_3, &CPicSimulatorDlg::OnBnClickedTrisb3)
	ON_BN_CLICKED(CHECK_TRISB_2, &CPicSimulatorDlg::OnBnClickedTrisb2)
	ON_BN_CLICKED(CHECK_TRISB_1, &CPicSimulatorDlg::OnBnClickedTrisb1)
	ON_BN_CLICKED(CHECK_TRISB_0, &CPicSimulatorDlg::OnBnClickedTrisb0)
	ON_BN_CLICKED(CHECK_TRISA_4, &CPicSimulatorDlg::OnBnClickedTrisa4)
	ON_BN_CLICKED(CHECK_TRISA_3, &CPicSimulatorDlg::OnBnClickedTrisa3)
	ON_BN_CLICKED(CHECK_TRISA_2, &CPicSimulatorDlg::OnBnClickedTrisa2)
	ON_BN_CLICKED(CHECK_TRISA_1, &CPicSimulatorDlg::OnBnClickedTrisa1)
	ON_BN_CLICKED(CHECK_TRISA_0, &CPicSimulatorDlg::OnBnClickedTrisa0)
	ON_BN_CLICKED(CHECK_A0, &CPicSimulatorDlg::OnBnClickedA0)
	ON_BN_CLICKED(CHECK_A1, &CPicSimulatorDlg::OnBnClickedA1)
	ON_BN_CLICKED(CHECK_A2, &CPicSimulatorDlg::OnBnClickedA2)
	ON_BN_CLICKED(CHECK_A3, &CPicSimulatorDlg::OnBnClickedA3)
	ON_BN_CLICKED(CHECK_A4, &CPicSimulatorDlg::OnBnClickedA4)
	ON_BN_CLICKED(CHECK_B0, &CPicSimulatorDlg::OnBnClickedB0)
	ON_BN_CLICKED(CHECK_B1, &CPicSimulatorDlg::OnBnClickedB1)
	ON_BN_CLICKED(CHECK_B2, &CPicSimulatorDlg::OnBnClickedB2)
	ON_BN_CLICKED(CHECK_B3, &CPicSimulatorDlg::OnBnClickedB3)
	ON_BN_CLICKED(CHECK_B4, &CPicSimulatorDlg::OnBnClickedB4)
	ON_BN_CLICKED(CHECK_B5, &CPicSimulatorDlg::OnBnClickedB5)
	ON_BN_CLICKED(CHECK_B6, &CPicSimulatorDlg::OnBnClickedB6)
	ON_BN_CLICKED(CHECK_B7, &CPicSimulatorDlg::OnBnClickedB7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_FREQ, &CPicSimulatorDlg::OnNMCustomdrawSliderFreq)
	ON_BN_CLICKED(CHECK_OPTION0, &CPicSimulatorDlg::OnBnClickedOption0)
	ON_BN_CLICKED(CHECK_OPTION1, &CPicSimulatorDlg::OnBnClickedOption1)
	ON_BN_CLICKED(CHECK_OPTION2, &CPicSimulatorDlg::OnBnClickedOption2)
	ON_BN_CLICKED(CHECK_OPTION3, &CPicSimulatorDlg::OnBnClickedOption3)
	ON_BN_CLICKED(CHECK_OPTION4, &CPicSimulatorDlg::OnBnClickedOption4)
	ON_BN_CLICKED(CHECK_OPTION5, &CPicSimulatorDlg::OnBnClickedOption5)
	ON_BN_CLICKED(CHECK_OPTION6, &CPicSimulatorDlg::OnBnClickedOption6)
	ON_BN_CLICKED(CHECK_OPTION7, &CPicSimulatorDlg::OnBnClickedOption7)
	ON_BN_CLICKED(CHECK_INTCON1, &CPicSimulatorDlg::OnBnClickedIntcon1)
	ON_BN_CLICKED(CHECK_INTCON0, &CPicSimulatorDlg::OnBnClickedIntcon0)
	ON_BN_CLICKED(CHECK_INTCON2, &CPicSimulatorDlg::OnBnClickedIntcon2)
	ON_BN_CLICKED(CHECK_INTCON3, &CPicSimulatorDlg::OnBnClickedIntcon3)
	ON_BN_CLICKED(CHECK_INTCON4, &CPicSimulatorDlg::OnBnClickedIntcon4)
	ON_BN_CLICKED(CHECK_INTCON5, &CPicSimulatorDlg::OnBnClickedIntcon5)
	ON_BN_CLICKED(CHECK_INTCON6, &CPicSimulatorDlg::OnBnClickedIntcon6)
	ON_BN_CLICKED(CHECK_INTCON7, &CPicSimulatorDlg::OnBnClickedIntcon7)
	ON_BN_CLICKED(CHECK_STATUS0, &CPicSimulatorDlg::OnBnClickedStatus0)
	ON_BN_CLICKED(CHECK_STATUS1, &CPicSimulatorDlg::OnBnClickedStatus1)
	ON_BN_CLICKED(CHECK_STATUS2, &CPicSimulatorDlg::OnBnClickedStatus2)
	ON_BN_CLICKED(CHECK_STATUS3, &CPicSimulatorDlg::OnBnClickedStatus3)
	ON_BN_CLICKED(CHECK_STATUS4, &CPicSimulatorDlg::OnBnClickedStatus4)
	ON_BN_CLICKED(CHECK_STATUS5, &CPicSimulatorDlg::OnBnClickedStatus5)
	ON_BN_CLICKED(CHECK_STATUS6, &CPicSimulatorDlg::OnBnClickedStatus6)
	ON_BN_CLICKED(CHECK_STATUS7, &CPicSimulatorDlg::OnBnClickedStatus7)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CPicSimulatorDlg::OnBnClickedButtonHelp)
	ON_BN_CLICKED(CHECK_WDT, &CPicSimulatorDlg::OnBnClickedWdt)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CPicSimulatorDlg::OnBnClickedButtonRun)
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_UPDATE_CONTROL, OnUpdateControl)
	ON_MESSAGE(WM_BREAK, OnBreak)
	ON_BN_CLICKED(IDC_BUTTON1, &CPicSimulatorDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPicSimulatorDlg-Meldungshandler

BOOL CPicSimulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Hinzufügen des Menübefehls "Info..." zum Systemmenü.

	// IDM_ABOUTBOX muss sich im Bereich der Systembefehle befinden.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Symbol für dieses Dialogfeld festlegen.  Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	CRect rect;
	lstProgMem.GetClientRect(&rect);
	int nColInterval = rect.Width() / 3 - 5;
	lstProgMem.SetExtendedStyle(LVS_EX_CHECKBOXES); //initialisiert Checkboxen im ProgMem-List

	lstStack.InsertColumn(0, _T("Addr."), LVCFMT_LEFT, nColInterval *1.5); //Stack-Liste braucht nur eine Spalte

	lstProgMem.InsertColumn(0, _T("Break"), LVCFMT_LEFT, nColInterval*0.8); //Spalte für BReakpoint-Checkboxen
	lstProgMem.InsertColumn(1, _T("Addr."), LVCFMT_LEFT, nColInterval*0.9); //Spalte für Adressen
	lstProgMem.InsertColumn(2, _T("Instruktion"), LVCFMT_LEFT, nColInterval *1.325); //Spalte für Instruktionen

	lstSram.SetExtendedStyle(LVS_EX_FULLROWSELECT); //bessere Darstellung für SRAM
	lstSram.InsertColumn(0, _T("Addr."), LVCFMT_LEFT, nColInterval);
	lstSram.InsertColumn(1, _T("Bank 0"), LVCFMT_LEFT, nColInterval);
	lstSram.InsertColumn(2, _T("Bank 1"), LVCFMT_LEFT, nColInterval);

	lstEEprom.SetExtendedStyle(LVS_EX_FULLROWSELECT); //bessere Darstellung für EEPROM
	lstEEprom.InsertColumn(0, _T("Addr."), LVCFMT_LEFT, nColInterval);
	lstEEprom.InsertColumn(1, _T("Wert"), LVCFMT_LEFT, nColInterval*2);

	CString strText;

	for (int i = 0; i < 128; i++)  //initialisiert die SRAM-Speicher-Liste
	{
		strText.Format(TEXT("0x%X"), i); //in Spalte eins stehen hexadez. formatiert die Adressen

		// Insert the item, select every other item.
		lstSram.InsertItem(LVIF_TEXT | LVIF_STATE, i, strText, //neue Items (jede Adresse eines) werden initialisiert
			(i % 2) == 0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);

		strText.Format(TEXT("0x%X"), pic16f84.memoryControl.getMem(i, 0)); //neue Spalte wird an jedes Item gehängt, Inhalt ist SPeicher von Bank 0
		lstSram.SetItemText(i, 1, strText);

		strText.Format(TEXT("0x%X"), pic16f84.memoryControl.getMem(i, 1)); //neue Spalte wird an jedes Item gehängt, Inhalt ist SPeicher von Bank 1
		lstSram.SetItemText(i, 2, strText);
	}

	for (int i = 0; i < 1000; i++)
	{
		strText.Format(TEXT(""), i); //erste Spalte muss leer sein, da nur Breakpoint-Checkbox erforderlich

		// Insert the item, select every other item.
		lstProgMem.InsertItem(LVIF_TEXT | LVIF_STATE, i, strText,
			(i % 2) == 0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);

		strText.Format(TEXT("0x%X"), i); //zweite Spalte = Adresse
		lstProgMem.SetItemText(i, 1, strText);

		strText.Format(TEXT("0x0")); //dritte Spalte = Instruktion, zunächst alle 0
		lstProgMem.SetItemText(i, 2, strText);
	}

	for(int i = 0; i < 64; i++)
	{
		strText.Format(TEXT("0x%X"), i); //erste Spalte = Adresse

		// Insert the item, select every other item.
		lstEEprom.InsertItem(LVIF_TEXT | LVIF_STATE, i, strText,
			(i % 2) == 0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);

		strText.Format(TEXT("0x0")); //zweite Spalte = EEPROM-Inhalt
		lstEEprom.SetItemText(i, 1, strText);
	}

	refreshGUI(); //Steuerelemente werden gleich mit korrekten, aktuellen Werten gefüllt

	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CPicSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen.  Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CPicSimulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CPicSimulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPicSimulatorDlg::OnBnClickedButtonLoad()
{
	CFileDialog fileDlg(TRUE);///Filter und Titel 

	// Display the file dialog. When user clicks OK, fileDlg.DoModal()
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		CString file = fileDlg.GetFolderPath() + "\\" + fileDlg.GetFileName();
		pic16f84.reset(true); //Bei neuem Listing: Standardwerte wiederherstellen
		pic16f84.parseLST((LPCTSTR)file);
		refreshGUI();
		refreshProgMem(); //Neudarstellung erforderlich, ist nicht in RefreshGUI
	}
}

void CPicSimulatorDlg::OnBnClickedButtonStep()
{
	bool isBreakPoint = ListView_GetCheckState(lstProgMem, pic16f84.memoryControl.getPLC());
	if(!isBreakPoint)
	{
		pic16f84.execCycle();
		refreshGUI();
	}
}

void CPicSimulatorDlg::OnBnClickedButtonReset()
{
	pic16f84.reset(false);
	refreshGUI();
}

LRESULT CPicSimulatorDlg::OnUpdateControl(WPARAM wParam, LPARAM lParam)
{
	refreshGUI();

	return 0;
}

LRESULT CPicSimulatorDlg::OnBreak(WPARAM wParam, LPARAM lParam)
{
	picThread.join();
	btnRun.SetWindowTextW(_T("Run"));
	//	lockGUI(true);
	refreshGUI();

	MessageBox(_T("Breakpoint erreicht"), _T("Pic Simulator"),
		MB_ICONINFORMATION | MB_OK);

	return 0;
}

void CPicSimulatorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int freq = freqSlider.GetPos();
	if (freq < 1)
		freq = 1;
	freq *= 200;

	CString strFreq;
	strFreq.Format(_T("%i MHz"), freq);

	lblFreq.SetWindowTextW(strFreq);
}

void CPicSimulatorDlg::refreshGUI()
{
	TCHAR buf[32];

	refreshSram();
	refreshEEprom();

	_itot_s(pic16f84.memoryControl.getW(), buf, 16);
	lblRegW.SetWindowTextW(buf);

	_itot_s(pic16f84.memoryControl.getPLC(), buf, 16);
	lblPLC.SetWindowTextW(buf);

	_itot_s(pic16f84.memoryControl.getMem(1), buf, 16);
	lblTimer0.SetWindowTextW(buf);

	_itot_s(pic16f84.memoryControl.getStatus(MemoryControl::FLAG_CARRY), buf, 16);
	lblRegC.SetWindowTextW(buf);

	_itot_s(pic16f84.memoryControl.getStatus(MemoryControl::FLAG_DIGIT), buf, 16);
	lblRegDC.SetWindowTextW(buf);

	_itot_s(pic16f84.memoryControl.getStatus(MemoryControl::FLAG_ZERO), buf, 16);
	lblRegZ.SetWindowTextW(buf);

	LPCTSTR sleepstr = _T("deaktiviert");
	if(pic16f84.memoryControl.getSleep()) sleepstr = _T("aktiviert");
	lblSleep.SetWindowTextW(sleepstr);

	checkPortA0.SetCheck(pic16f84.memoryControl.getMem(5) & 1);
	checkPortA1.SetCheck(pic16f84.memoryControl.getMem(5) & 1);
	
	checkTrisa0.SetCheck(pic16f84.memoryControl.getTris(0, false));
	checkTrisa1.SetCheck(pic16f84.memoryControl.getTris(1, false));
	checkTrisa2.SetCheck(pic16f84.memoryControl.getTris(2, false));
	checkTrisa3.SetCheck(pic16f84.memoryControl.getTris(3, false));
	checkTrisa4.SetCheck(pic16f84.memoryControl.getTris(4, false));

	checkPortA0.SetCheck(pic16f84.memoryControl.getPort(0, false));
	checkPortA1.SetCheck(pic16f84.memoryControl.getPort(1, false));
	checkPortA2.SetCheck(pic16f84.memoryControl.getPort(2, false));
	checkPortA3.SetCheck(pic16f84.memoryControl.getPort(3, false));
	checkPortA4.SetCheck(pic16f84.memoryControl.getPort(4, false));

	checkTrisb0.SetCheck(pic16f84.memoryControl.getTris(0, true));
	checkTrisb1.SetCheck(pic16f84.memoryControl.getTris(1, true));
	checkTrisb2.SetCheck(pic16f84.memoryControl.getTris(2, true));
	checkTrisb3.SetCheck(pic16f84.memoryControl.getTris(3, true));
	checkTrisb4.SetCheck(pic16f84.memoryControl.getTris(4, true));
	checkTrisb5.SetCheck(pic16f84.memoryControl.getTris(5, true));
	checkTrisb6.SetCheck(pic16f84.memoryControl.getTris(6, true));
	checkTrisb7.SetCheck(pic16f84.memoryControl.getTris(7, true));

	checkPortB0.SetCheck(pic16f84.memoryControl.getPort(0, true));
	checkPortB1.SetCheck(pic16f84.memoryControl.getPort(1, true));
	checkPortB2.SetCheck(pic16f84.memoryControl.getPort(2, true));
	checkPortB3.SetCheck(pic16f84.memoryControl.getPort(3, true));
	checkPortB4.SetCheck(pic16f84.memoryControl.getPort(4, true));
	checkPortB5.SetCheck(pic16f84.memoryControl.getPort(5, true));
	checkPortB6.SetCheck(pic16f84.memoryControl.getPort(6, true));
	checkPortB7.SetCheck(pic16f84.memoryControl.getPort(7, true));

	checkOption0.SetCheck(pic16f84.memoryControl.getOption(0));
	checkOption1.SetCheck(pic16f84.memoryControl.getOption(1));
	checkOption2.SetCheck(pic16f84.memoryControl.getOption(2));
	checkOption3.SetCheck(pic16f84.memoryControl.getOption(3));
	checkOption4.SetCheck(pic16f84.memoryControl.getOption(4));
	checkOption5.SetCheck(pic16f84.memoryControl.getOption(5));
	checkOption6.SetCheck(pic16f84.memoryControl.getOption(6));
	checkOption7.SetCheck(pic16f84.memoryControl.getOption(7));

	checkIntcon0.SetCheck(pic16f84.memoryControl.getIntcon(0));
	checkIntcon1.SetCheck(pic16f84.memoryControl.getIntcon(1));
	checkIntcon2.SetCheck(pic16f84.memoryControl.getIntcon(2));
	checkIntcon3.SetCheck(pic16f84.memoryControl.getIntcon(3));
	checkIntcon4.SetCheck(pic16f84.memoryControl.getIntcon(4));
	checkIntcon5.SetCheck(pic16f84.memoryControl.getIntcon(5));
	checkIntcon6.SetCheck(pic16f84.memoryControl.getIntcon(6));
	checkIntcon7.SetCheck(pic16f84.memoryControl.getIntcon(7));

	checkStatus0.SetCheck(pic16f84.memoryControl.getStatus(0));
	checkStatus1.SetCheck(pic16f84.memoryControl.getStatus(1));
	checkStatus2.SetCheck(pic16f84.memoryControl.getStatus(2));
	checkStatus3.SetCheck(pic16f84.memoryControl.getStatus(3));
	checkStatus4.SetCheck(pic16f84.memoryControl.getStatus(4));
	checkStatus5.SetCheck(pic16f84.memoryControl.getStatus(5));
	checkStatus6.SetCheck(pic16f84.memoryControl.getStatus(6));
	checkStatus7.SetCheck(pic16f84.memoryControl.getStatus(7));

	lstStack.DeleteAllItems(); //Neudarstellung des Stacks, alle alten Items werden gelöscht um Platz für neue zu schaffen
	for(int i = 0; i < 8; i++) {
		CString strText;

		strText.Format(TEXT("0x%X"), pic16f84.memoryControl.getStack(i));

		// Insert the item, select every other item.
		lstStack.InsertItem(LVIF_TEXT | LVIF_STATE, i, strText,
			(i % 2) == 0 ? LVIS_SELECTED : 0, LVIS_SELECTED, 0, 0);
	}

}

void CPicSimulatorDlg::refreshProgMem()
{
	CString strText;

	for (int i = 0; i < 1000; i++)
	{
		strText.Format(TEXT("0x%X"),pic16f84.memoryControl.getProg_mem(i));
		lstProgMem.SetItemText(i, 2, strText); //nur Spalte zwei braucht ein Update
	}
}

void CPicSimulatorDlg::refreshSram()
{
	CString strText;

	for(int i = 0; i < 128; i++)
	{
		strText.Format(TEXT("0x%X"), pic16f84.memoryControl.getMem(i, 0));
		lstSram.SetItemText(i, 1, strText);

		strText.Format(TEXT("0x%X"), pic16f84.memoryControl.getMem(i, 1));
		lstSram.SetItemText(i, 2, strText);
	}
}

void CPicSimulatorDlg::refreshEEprom()
{
	CString strText;

	for(int i = 0; i < 64; i++)
	{
		strText.Format(TEXT("0x%X"), pic16f84.memoryControl.getEeprom(i));
		lstEEprom.SetItemText(i, 1, strText);
	}
}

//Durch Verwendung von Mutex Locks ist ein Sperren der GUI nicht mehr erforderlich
//void CPicSimulatorDlg::lockGUI(bool lock)
//{
//	freqSlider.EnableWindow(lock);

//	checkTrisa0.EnableWindow(lock);
//	checkTrisa1.EnableWindow(lock);
//	checkTrisa2.EnableWindow(lock);
//	checkTrisa3.EnableWindow(lock);
//	checkTrisa4.EnableWindow(lock);

//	checkPortA0.EnableWindow(lock);
//	checkPortA1.EnableWindow(lock);
//	checkPortA2.EnableWindow(lock);
//	checkPortA3.EnableWindow(lock);
//	checkPortA4.EnableWindow(lock);

//	checkTrisb0.EnableWindow(lock);
//	checkTrisb1.EnableWindow(lock);
//	checkTrisb2.EnableWindow(lock);
//	checkTrisb3.EnableWindow(lock);
//	checkTrisb4.EnableWindow(lock);
//	checkTrisb5.EnableWindow(lock);
//	checkTrisb6.EnableWindow(lock);
//	checkTrisb7.EnableWindow(lock);

//	checkPortB0.EnableWindow(lock);
//	checkPortB1.EnableWindow(lock);
//	checkPortB2.EnableWindow(lock);
//	checkPortB3.EnableWindow(lock);
//	checkPortB4.EnableWindow(lock);
//	checkPortB5.EnableWindow(lock);
//	checkPortB6.EnableWindow(lock);
//	checkPortB7.EnableWindow(lock);

//	checkOption0.EnableWindow(lock);
//	checkOption1.EnableWindow(lock);
//checkOption2.EnableWindow(lock);
	//checkOption3.EnableWindow(lock);
	//checkOption4.EnableWindow(lock);
	//checkOption5.EnableWindow(lock);
	//checkOption6.EnableWindow(lock);
	//	checkOption7.EnableWindow(lock);

	//checkIntcon0.EnableWindow(lock);
	//checkIntcon1.EnableWindow(lock);
	//checkIntcon2.EnableWindow(lock);
	//checkIntcon3.EnableWindow(lock);
	//checkIntcon4.EnableWindow(lock);
	//checkIntcon5.EnableWindow(lock);
	//checkIntcon6.EnableWindow(lock);
	//checkIntcon7.EnableWindow(lock);

	//checkStatus0.EnableWindow(lock);
	//checkStatus1.EnableWindow(lock);
	//checkStatus2.EnableWindow(lock);
	//checkStatus3.EnableWindow(lock);
	//checkStatus4.EnableWindow(lock);
	//checkStatus5.EnableWindow(lock);
	//checkStatus6.EnableWindow(lock);
	//checkStatus7.EnableWindow(lock);
	//}

void CPicSimulatorDlg::OnBnClickedTrisb7()
{
	pic16f84.memoryControl.setTris(7, true, checkTrisb7.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisb6()
{
	pic16f84.memoryControl.setTris(6, true, checkTrisb6.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisb5()
{
	pic16f84.memoryControl.setTris(5, true, checkTrisb5.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisb4()
{
	pic16f84.memoryControl.setTris(4, true, checkTrisb4.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisb3()
{
	pic16f84.memoryControl.setTris(3, true, checkTrisb3.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisb2()
{
	pic16f84.memoryControl.setTris(2, true, checkTrisb2.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisb1()
{
	pic16f84.memoryControl.setTris(1, true, checkTrisb1.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisb0()
{
	pic16f84.memoryControl.setTris(0, true, checkTrisb0.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedTrisa4()
{
	pic16f84.memoryControl.setTris(4, false, checkTrisa4.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisa3()
{
	pic16f84.memoryControl.setTris(3, false, checkTrisa3.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisa2()
{
	pic16f84.memoryControl.setTris(2, false, checkTrisa2.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisa1()
{
	pic16f84.memoryControl.setTris(1, false, checkTrisa1.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedTrisa0()
{
	pic16f84.memoryControl.setTris(0, false, checkTrisa0.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedA0()
{
	pic16f84.memoryControl.setPort(0, false, checkPortA0.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedA1()
{
	pic16f84.memoryControl.setPort(1, false, checkPortA1.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedA2()
{
	pic16f84.memoryControl.setPort(2, false, checkPortA2.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedA3()
{
	pic16f84.memoryControl.setPort(3, false, checkPortA3.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedA4()
{
	pic16f84.memoryControl.setPort(4, false, checkPortA4.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB0()
{
	pic16f84.memoryControl.setPort(0, true, checkPortB0.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB1()
{
	pic16f84.memoryControl.setPort(1, true, checkPortB1.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB2()
{
	pic16f84.memoryControl.setPort(2, true, checkPortB2.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB3()
{
	pic16f84.memoryControl.setPort(3, true, checkPortB3.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB4()
{
	pic16f84.memoryControl.setPort(4, true, checkPortB4.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB5()
{
	pic16f84.memoryControl.setPort(5, true, checkPortB5.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB6()
{
	pic16f84.memoryControl.setPort(6, true, checkPortB6.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedB7()
{
	pic16f84.memoryControl.setPort(7, true, checkPortB7.GetCheck());
}


void CPicSimulatorDlg::OnNMCustomdrawSliderFreq(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	*pResult = 0;
}


void CPicSimulatorDlg::OnBnClickedOption0()
{
	pic16f84.memoryControl.setOption(0, checkOption0.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedOption1()
{
	pic16f84.memoryControl.setOption(1, checkOption1.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedOption2()
{
	pic16f84.memoryControl.setOption(2, checkOption2.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedOption3()
{
	pic16f84.memoryControl.setOption(3, checkOption3.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedOption4()
{
	pic16f84.memoryControl.setOption(4, checkOption4.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedOption5()
{
	pic16f84.memoryControl.setOption(5, checkOption5.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedOption6()
{
	pic16f84.memoryControl.setOption(6, checkOption6.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedOption7()
{
	pic16f84.memoryControl.setOption(7, checkOption7.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedIntcon0()
{
	pic16f84.memoryControl.setIntcon(0, checkIntcon0.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedIntcon1()
{
	pic16f84.memoryControl.setIntcon(1, checkIntcon1.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedIntcon2()
{
	pic16f84.memoryControl.setIntcon(2, checkIntcon2.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedIntcon3()
{
	pic16f84.memoryControl.setIntcon(3, checkIntcon3.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedIntcon4()
{
	pic16f84.memoryControl.setIntcon(4, checkIntcon4.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedIntcon5()
{
	pic16f84.memoryControl.setIntcon(5, checkIntcon5.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedIntcon6()
{
	pic16f84.memoryControl.setIntcon(6, checkIntcon6.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedIntcon7()
{
	pic16f84.memoryControl.setIntcon(7, checkIntcon7.GetCheck());
}



void CPicSimulatorDlg::OnBnClickedStatus0()
{
	pic16f84.memoryControl.setStatus(0, checkStatus0.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedStatus1()
{
	pic16f84.memoryControl.setStatus(1, checkStatus1.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedStatus2()
{
	pic16f84.memoryControl.setStatus(2, checkStatus2.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedStatus3()
{
	pic16f84.memoryControl.setStatus(3, checkStatus3.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedStatus4()
{
	pic16f84.memoryControl.setStatus(4, checkStatus4.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedStatus5()
{
	pic16f84.memoryControl.setStatus(5, checkStatus5.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedStatus6()
{
	pic16f84.memoryControl.setStatus(6, checkStatus6.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedStatus7()
{
	pic16f84.memoryControl.setStatus(7, checkStatus7.GetCheck());
}

void CPicSimulatorDlg::OnBnClickedButtonHelp()
{
	LPCWSTR path = _T("help.pdf"); ///PFAD
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
} 

void CPicSimulatorDlg::OnBnClickedWdt()
{
	pic16f84.memoryControl.setWatchdog(checkWDT.GetCheck());
}


void CPicSimulatorDlg::OnBnClickedButtonRun()
{
	if(picThreadRunning) {
		picThreadRunning = false;
		picThread.join();
		btnRun.SetWindowTextW(_T("Run"));
		//lockGUI(true);
		refreshGUI();
	}
	else {
		int freq = freqSlider.GetPos();
		if(freq < 1)
			freq = 1; 
		freq *= 50; //Wert anpassen, um vernünftige Frequenz zu erhalten

		btnRun.SetWindowTextW(_T("Stop"));
		//lockGUI(false);
		picThreadRunning = true;

		for (int i = 0; i < 1000; i++) {
			breakpionts[i] = ListView_GetCheckState(lstProgMem, i);
		}

		PIC16F84 *picRef = &pic16f84; //Übergabe des aktuellen Objektes durch Pointer
		bool *run = &picThreadRunning;
		HWND *phObjectHandle = new HWND;
		bool *bp = breakpionts; 
		*phObjectHandle = GetSafeHwnd();

		picThread = std::thread([picRef, run, freq, bp ,phObjectHandle] { //Erstellung eines neuen Threads, um GUI nicht zu behindern

			while(*run) {
				std::chrono::milliseconds start = std::chrono::duration_cast< std::chrono::milliseconds >( //Startzeit speichern
					std::chrono::system_clock::now().time_since_epoch()
				);

				for (int i = 0; i < freq / 10; i++) {
					picRef->execCycle();
					if (bp[picRef->memoryControl.getPLC()]) { //auf Breakpoint überprüfen
						*run = false;
						::PostMessage(*phObjectHandle, WM_BREAK, 0, 0);
						break;
					}						
				}

				if (*run)
					::PostMessage(*phObjectHandle, WM_UPDATE_CONTROL, 0, 0); //GUI aktualisieren

				std::chrono::milliseconds currentTime = std::chrono::duration_cast< std::chrono::milliseconds >( //Initialisierung der Warte-Zeit
					std::chrono::system_clock::now().time_since_epoch()
				);

				while(currentTime < start + std::chrono::milliseconds(100)) { //Wenn Instruktion vor Taktzyklus zu Ende ist: Warten!
					currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::system_clock::now().time_since_epoch()
					);
				}
			}
		});
	}
}

void CPicSimulatorDlg::OnBnClickedButton1() //Ändern von Speicherzelle
{
	bool bank = false; //damit keine Wahl von Radiobutton erforderlich ist
	if(checkBank0Sram != 0)bank = true;

	CString sWindowText; //Wildes Rumgecaste, danke an Stackoverflow... :D
	char buffer[500];
	sramEdit.GetWindowText(sWindowText);
	wcstombs_s(NULL, buffer, sWindowText, 500);
	int number = strtol(buffer,NULL, 16);

	sramAddr.GetWindowText(sWindowText);
	wcstombs_s(NULL, buffer, sWindowText, 500);
	int addr = strtol(buffer, NULL, 16);

	pic16f84.memoryControl.setMem(addr, number, bank);

	refreshGUI();
}
