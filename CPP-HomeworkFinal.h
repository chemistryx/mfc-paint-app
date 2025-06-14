#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h" // 주 기호입니다.

class CCPPHomeworkFinalApp : public CWinApp {
public:
	CCPPHomeworkFinalApp() noexcept;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCPPHomeworkFinalApp theApp;
