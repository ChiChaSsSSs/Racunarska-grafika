
// 2016Bilijar.h : main header file for the 2016Bilijar application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy2016BilijarApp:
// See 2016Bilijar.cpp for the implementation of this class
//

class CMy2016BilijarApp : public CWinApp
{
public:
	CMy2016BilijarApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy2016BilijarApp theApp;
