
// 2023Robot.h : main header file for the 2023Robot application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMy2023RobotApp:
// See 2023Robot.cpp for the implementation of this class
//

class CMy2023RobotApp : public CWinApp
{
public:
	CMy2023RobotApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy2023RobotApp theApp;
