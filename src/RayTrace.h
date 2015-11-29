// RayTrace.h : RAYTRACE アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_RAYTRACE_H__B4EED835_53CC_11D3_9AB7_000000000000__INCLUDED_)
#define AFX_RAYTRACE_H__B4EED835_53CC_11D3_9AB7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "..\resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CRayTraceApp:
// このクラスの動作の定義に関しては RayTrace.cpp ファイルを参照してください。
//

class CRayTraceApp : public CWinApp
{
public:
	CRayTraceApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRayTraceApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CRayTraceApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RAYTRACE_H__B4EED835_53CC_11D3_9AB7_000000000000__INCLUDED_)
