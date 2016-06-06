
// PicSimulatorDlg.h: Headerdatei
//

#pragma once
#include "PIC16F84.h"
#include "afxwin.h"
#include "afxcmn.h"

#include <thread>
//#include "P:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\atlmfc\include\afxwin.h"


// CPicSimulatorDlg-Dialogfeld
class CPicSimulatorDlg : public CDialogEx
{
// Konstruktion
public:
	CPicSimulatorDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICSIMULATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	void refreshGUI();
	void refreshProgMem();
	void refreshSram();
	void refreshEEprom();
	//void lockGUI(bool lock);

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	PIC16F84 pic16f84;

	bool breakpionts[1000];
	std::thread picThread;
	bool picThreadRunning = false;

	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonStep();
	CStatic lblRegW;
	CStatic lblPLC;
	CStatic lblTimer0;
	CStatic lblFreq;
	CButton checkPortA0;
	CButton checkPortA1;
	CStatic lblRegC;
	CStatic lblRegDC;
	CStatic lblRegZ;
	CListCtrl lstProgMem;
	CButton checkTrisb7;
	CButton checkTrisb6;
	CButton checkTrisb5;
	CButton checkTrisb4;
	CButton checkTrisb3;
	CButton checkTrisb2;
	CButton checkTrisb1;
	CButton checkTrisb0;
	afx_msg void OnBnClickedTrisb7();
	afx_msg void OnBnClickedTrisb6();
	afx_msg void OnBnClickedTrisb5();
	afx_msg void OnBnClickedTrisb4();
	afx_msg void OnBnClickedTrisb3();
	afx_msg void OnBnClickedTrisb2();
	afx_msg void OnBnClickedTrisb1();
	afx_msg void OnBnClickedTrisb0();
	afx_msg void OnBnClickedTrisa4();
	afx_msg void OnBnClickedTrisa3();
	afx_msg void OnBnClickedTrisa2();
	afx_msg void OnBnClickedTrisa1();
	afx_msg void OnBnClickedTrisa0();
	CButton checkTrisa0;
	CButton checkTrisa1;
	CButton checkTrisa2;
	CButton checkTrisa3;
	CButton checkTrisa4;
	afx_msg void OnBnClickedA0();
	afx_msg void OnBnClickedA1();
	afx_msg void OnBnClickedA2();
	afx_msg void OnBnClickedA3();
	afx_msg void OnBnClickedA4();
	CButton checkPortA2;
	CButton checkPortA3;
	CButton checkPortA4;
	CButton checkPortB0;
	CButton checkPortB1;
	CButton checkPortB2;
	CButton checkPortB3;
	CButton checkPortB4;
	CButton checkPortB5;
	CButton checkPortB6;
	CButton checkPortB7;
	afx_msg void OnBnClickedB0();
	afx_msg void OnBnClickedB1();
	afx_msg void OnBnClickedB2();
	afx_msg void OnBnClickedB3();
	afx_msg void OnBnClickedB4();
	afx_msg void OnBnClickedB5();
	afx_msg void OnBnClickedB6();
	afx_msg void OnBnClickedB7();
	int sliderFrq;
	afx_msg void OnNMCustomdrawSliderFreq(NMHDR *pNMHDR, LRESULT *pResult);
	CButton checkOption0;
	CButton checkOption1;
	CButton checkOption2;
	CButton checkOption3;
	CButton checkOption4;
	CButton checkOption5;
	CButton checkOption6;
	CButton checkOption7;
	afx_msg void OnBnClickedOption0();
	afx_msg void OnBnClickedOption1();
	afx_msg void OnBnClickedOption2();
	afx_msg void OnBnClickedOption3();
	afx_msg void OnBnClickedOption4();
	afx_msg void OnBnClickedOption5();
	afx_msg void OnBnClickedOption6();
	afx_msg void OnBnClickedOption7();
	CButton checkIntcon0;
	CButton checkIntcon1;
	CButton checkIntcon2;
	CButton checkIntcon3;
	CButton checkIntcon4;
	CButton checkIntcon5;
	CButton checkIntcon6;
	CButton checkIntcon7;
	afx_msg void OnBnClickedIntcon0();
	afx_msg void OnBnClickedIntcon1();
	afx_msg void OnBnClickedIntcon2();
	afx_msg void OnBnClickedIntcon3();
	afx_msg void OnBnClickedIntcon4();
	afx_msg void OnBnClickedIntcon5();
	afx_msg void OnBnClickedIntcon6();
	afx_msg void OnBnClickedIntcon7();
	CButton checkStatus0;
	CButton checkStatus1;
	CButton checkStatus2;
	CButton checkStatus3;
	CButton checkStatus4;
	CButton checkStatus5;
	CButton checkStatus6;
	CButton checkStatus7;
	afx_msg void OnBnClickedStatus0();
	afx_msg void OnBnClickedStatus1();
	afx_msg void OnBnClickedStatus2();
	afx_msg void OnBnClickedStatus3();
	afx_msg void OnBnClickedStatus4();
	afx_msg void OnBnClickedStatus5();
	afx_msg void OnBnClickedStatus6();
	afx_msg void OnBnClickedStatus7();
	afx_msg void OnBnClickedButtonHelp();
	LRESULT OnUpdateControl(WPARAM wParam, LPARAM lParam);
	LRESULT OnBreak(WPARAM wParam, LPARAM lParam);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CListCtrl lstSram;
	CListCtrl lstStack;
	CStatic lblSleep;
	CButton checkWDT;
	afx_msg void OnBnClickedWdt();
	afx_msg void OnBnClickedButtonRun();
	CButton btnRun;
	CSliderCtrl freqSlider;
	CListCtrl lstEEprom;
	CComboBox dropdownSram;
	int checkBank0Sram;
	int checkBank1Sram;
	CEdit sramEdit;
	CButton changeSram;
	CButton ramchange;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	CEdit sramAddr;
};
