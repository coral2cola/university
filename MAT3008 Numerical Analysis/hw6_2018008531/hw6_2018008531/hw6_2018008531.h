
// hw6_2018008531.h : main header file for the hw6_2018008531 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Chw62018008531App:
// See hw6_2018008531.cpp for the implementation of this class
//

class Chw62018008531App : public CWinApp
{
public:
	Chw62018008531App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Chw62018008531App theApp;

#define ORIGIN_X 20
#define ORIGIN_Y 150
#define X_AXIS_LENGTH 800
#define Y_AXIS_LENGTH 500
#define BAR_WIDTH 5
#define BAR_HEIGHT 10
#define BAR_COLOR RGB(0,255,0)