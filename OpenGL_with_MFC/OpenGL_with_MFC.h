
// OpenGL_with_MFC.h : OpenGL_with_MFC アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// COpenGL_with_MFCApp:
// このクラスの実装については、OpenGL_with_MFC.cpp を参照してください。
//

class COpenGL_with_MFCApp : public CWinApp
{
public:
	COpenGL_with_MFCApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGL_with_MFCApp theApp;
