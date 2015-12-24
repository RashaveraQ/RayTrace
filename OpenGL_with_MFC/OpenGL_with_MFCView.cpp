
// OpenGL_with_MFCView.cpp : COpenGL_with_MFCView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "OpenGL_with_MFC.h"
#endif

#include "OpenGL_with_MFCDoc.h"
#include "OpenGL_with_MFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const TCHAR* ErrorStrings[] = {
	{ _T("No Error") },                   // 0
	{ _T("Unable to get a CD") },         // 1
	{ _T("ChoosePixelFormat failed") },   // 2
	{ _T("SelectPixelFormat failed") },   // 3
	{ _T("wglCreateContext failed") },    // 4
	{ _T("wglMakeCurrect failed") },      // 5
	{ _T("wglDeleteCotext failed") },     // 6
	{ _T("SwapBuffer failed") },          // 7
};

void COpenGL_with_MFCView::ShowError(int e)
{
	AfxMessageBox(ErrorStrings[e]);
}

// COpenGL_with_MFCView

IMPLEMENT_DYNCREATE(COpenGL_with_MFCView, CView)

BEGIN_MESSAGE_MAP(COpenGL_with_MFCView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// COpenGL_with_MFCView コンストラクション/デストラクション

COpenGL_with_MFCView::COpenGL_with_MFCView() : m_hRC(0), m_pDC(0)
{
	// TODO: 構築コードをここに追加します。

}

COpenGL_with_MFCView::~COpenGL_with_MFCView()
{
}

BOOL COpenGL_with_MFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

// COpenGL_with_MFCView 描画

void COpenGL_with_MFCView::OnDraw(CDC* /*pDC*/)
{
	COpenGL_with_MFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	::glPushMatrix();

	::glColor3f(0.0f, 1.0f, 0.0f);
	::auxWireSphere(0.5f);
	::glColor3f(1.0f, 0.0f, 0.0f);
	::auxWireCube(1.0f);

	::glPopMatrix();

	::glFinish();
	if (FALSE == ::SwapBuffers(m_pDC->GetSafeHdc())) ShowError(7);
}

// COpenGL_with_MFCView 診断

#ifdef _DEBUG
void COpenGL_with_MFCView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGL_with_MFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGL_with_MFCDoc* COpenGL_with_MFCView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGL_with_MFCDoc)));
	return (COpenGL_with_MFCDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGL_with_MFCView メッセージ ハンドラー


int COpenGL_with_MFCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pDC = new CClientDC(this);
	if (NULL == m_pDC){
		ShowError(1);
		return FALSE;
	}

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int pixelformat;
	if (0 == (pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd))){
		ShowError(2);
		return FALSE;
	}
	if (FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd)){
		ShowError(3);
		return FALSE;
	}

	if (0 == (m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc()))){
		ShowError(4);
		return FALSE;
	}

	if (FALSE == ::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)){
		ShowError(5);
		return FALSE;
	}

	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	::glClearDepth(1.0f);
	::glEnable(GL_DEPTH_TEST);

	return 0;
}

void COpenGL_with_MFCView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (FALSE == ::wglMakeCurrent(0, 0)) ShowError(2);
	if (FALSE == ::wglDeleteContext(m_hRC)) ShowError(6);
	if (m_pDC) delete m_pDC;
}


BOOL COpenGL_with_MFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}


void COpenGL_with_MFCView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (0 >= cx || 0 >= cy) return;

	::glViewport(0, 0, cx, cy);
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::gluPerspective(40.0f, (GLfloat)cx / (GLfloat)cy, 0.1f, 20.0f);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	// Viewing Transform
	::glTranslatef(0.0f, 0.0f, -5.0f);
	::glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
}



