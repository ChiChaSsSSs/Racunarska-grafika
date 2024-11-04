
// IND_18561_Lab2.h : main header file for the IND_18561_Lab2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND18561Lab2App:
// See IND_18561_Lab2.cpp for the implementation of this class
//

class CIND18561Lab2App : public CWinApp
{
public:
	CIND18561Lab2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND18561Lab2App theApp;
