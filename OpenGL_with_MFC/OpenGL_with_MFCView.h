
// OpenGL_with_MFCView.h : COpenGL_with_MFCView クラスのインターフェイス
//

#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"

class COpenGL_with_MFCView : public CView
{
protected: // シリアル化からのみ作成します。
	COpenGL_with_MFCView();
	DECLARE_DYNCREATE(COpenGL_with_MFCView)

// 属性
public:
	COpenGL_with_MFCDoc* GetDocument() const;
private:
	HGLRC m_hRC;
	CDC* m_pDC;

// 操作
private:
	void ShowError(int e);

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~COpenGL_with_MFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // OpenGL_with_MFCView.cpp のデバッグ バージョン
inline COpenGL_with_MFCDoc* COpenGL_with_MFCView::GetDocument() const
   { return reinterpret_cast<COpenGL_with_MFCDoc*>(m_pDocument); }
#endif

