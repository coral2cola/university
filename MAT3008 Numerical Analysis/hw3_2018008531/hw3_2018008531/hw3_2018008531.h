
// hw3_2018008531.h : main header file for the hw3_2018008531 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Chw32018008531App:
// See hw3_2018008531.cpp for the implementation of this class
//

class Chw32018008531App : public CWinApp
{
public:
	Chw32018008531App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Chw32018008531App theApp;
