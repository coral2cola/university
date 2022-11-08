
// hw4_2018008531.h : main header file for the hw4_2018008531 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Chw42018008531App:
// See hw4_2018008531.cpp for the implementation of this class
//

class Chw42018008531App : public CWinApp
{
public:
	Chw42018008531App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Chw42018008531App theApp;
