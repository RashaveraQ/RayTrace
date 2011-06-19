// RayTraceView.cpp : CRayTraceView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "RayTrace.h"

#include "RayTraceDoc.h"
#include "RayTraceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CONTINUED		0
#define FINISHED		1
#define REFMAX          1
#define START_SQUARE	32

/////////////////////////////////////////////////////////////////////////////
// CRayTraceView

IMPLEMENT_DYNCREATE(CRayTraceView, CView)

BEGIN_MESSAGE_MAP(CRayTraceView, CView)
	//{{AFX_MSG_MAP(CRayTraceView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_VIEW_RAYTRACE, OnViewRaytrace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RAYTRACE, OnUpdateViewRaytrace)
	ON_COMMAND(ID_VIEW_WIREFRAME, OnViewWireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME, OnUpdateViewWireframe)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_SYSKEYDOWN()
	ON_COMMAND(ID_VIEW_D3DWIREFRAME, OnViewD3dwireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_D3DWIREFRAME, OnUpdateViewD3dwireframe)
	ON_COMMAND(ID_VIEW_FLATSHADING, OnViewFlatshading)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FLATSHADING, OnUpdateViewFlatshading)
	ON_COMMAND(ID_VIEW_GOURAUDSHADING, OnViewGouraudshading)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GOURAUDSHADING, OnUpdateViewGouraudshading)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_VIEW_WIREFRAME_WITH_RAYTRACE, &CRayTraceView::OnViewWireframeWithRaytrace)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTraceView クラスの構築/消滅

CRayTraceView::CRayTraceView()
{
	m_StartX = m_StartY = m_NowX = m_NowY = 0;
	m_NowSize = START_SQUARE;
	m_View.left   = -10;
	m_View.right  = 10;
	m_View.top    = -10;
	m_View.bottom = 10;
	m_ViewMode = eD3DFlatShading; //eWireFrame;
	m_Alt = FALSE;
	m_AltStart.x = m_AltStart.y = 0;
	m_SelectedNode = NULL;
	m_Manipulator.Type = eSELECT;
	m_Manipulator.Axis = eNONE;
	m_pd3dDevice = NULL;
	m_vEyePt = D3DXVECTOR3(0.0f, 5.0f, -30.0f);
	m_vLookatPt = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
}

CRayTraceView::~CRayTraceView()
{
	m_MemoryDC.DeleteDC();
	m_Viewport.DetachRoot();
}

BOOL CRayTraceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	cs.style |= WS_VISIBLE;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRayTraceView クラスの描画

void CRayTraceView::OnDraw(CDC* pDC)
{
	switch (m_ViewMode) {
	case eRayTrace:
	case eWireFrameWithRayTrace:
		pDC->BitBlt(0, 0, m_ClientSize.cx, m_ClientSize.cy, &m_MemoryDC, 0, 0, SRCCOPY);
		if (m_ViewMode == eRayTrace)
			break;
	case eWireFrame:
		pDC->SelectStockObject(NULL_BRUSH);
		m_Viewport.Draw_Outline(pDC, *this, matrix(4,4));
		break;
	case eD3DWireFrame:
	case eD3DFlatShading:
	case eD3DGouraudShading:
		if (!m_pd3dDevice)
			UpdateDevice();

		// Clear the backbuffer to a blue color
		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
							D3DCOLOR_XRGB(127,127,127), 1.0f, 0);

		// Begin the scene
		if (SUCCEEDED(m_pd3dDevice->BeginScene())) {
			
			// Set up a material. The material here just has the diffuse and ambient
			// colors set to yellow. Note that only one material can be used at a time.
			D3DMATERIAL9 mtrl;
			ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
			mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
			m_pd3dDevice->SetMaterial( &mtrl );

			// Set up a white, directional light, with an oscillating direction.
			// Note that many lights may be active at a time (but each one slows down
			// the rendering of our scene). However, here we are just using one. Also,
			// we need to set the D3DRS_LIGHTING renderstate to enable lighting
			D3DXVECTOR3 vecDir;
			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(D3DLIGHT9));
			light.Type       = D3DLIGHT_DIRECTIONAL;
			light.Diffuse.r  = 1.0f;
			light.Diffuse.g  = 1.0f;
			light.Diffuse.b  = 1.0f;
			vecDir = D3DXVECTOR3(cosf(timeGetTime()/350.0f),
								 -1.0f,
								 sinf(timeGetTime()/350.0f));
			D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
			light.Range       = 1000.0f;
			m_pd3dDevice->SetLight(0, &light);
			m_pd3dDevice->LightEnable(0, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			// Finally, turn on some ambient light.
			m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);

			// Set up our view matrix. A view matrix can be defined given an eye point,
			// a point to lookat, and a direction for which way is up. Here, we set the
			// eye five units back along the z-axis and up three units, look at the
			// origin, and define "up" to be in the y-direction.
			D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
			D3DXMATRIXA16 matView;
			D3DXMatrixLookAtLH(&matView, &m_vEyePt, &m_vLookatPt, &vUpVec);
			m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

			// For the projection matrix, we set up a perspective transform (which
			// transforms geometry from 3D view space to 2D viewport space, with
			// a perspective divide making objects smaller in the distance). To build
			// a perpsective transform, we need the field of view (1/4 pi is common),
			// the aspect ratio, and the near and far clipping planes (which define at
			// what distances geometry should be no longer be rendered).
			D3DXMATRIXA16 matProj;
			D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f);
			m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

			// For our world matrix, we will just leave it as the identity
			D3DXMATRIXA16 matWorld;
			D3DXMatrixIdentity(&matWorld);
			//D3DXMatrixRotationX(&matWorld, timeGetTime()/500.0f);
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

			// Rendering of scene objects can happen here
			POSITION pos = m_lstGeometry.GetHeadPosition();
			while (pos) {
				Geometry geometry = m_lstGeometry.GetAt(pos);
				if (!geometry.Draw(m_pd3dDevice))
					MessageBox("Error");
				m_lstGeometry.GetNext(pos);
			}

			// End the scene
			m_pd3dDevice->EndScene();
		}

		// Present the backbuffer contents to the display
		m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRayTraceView クラスの印刷

BOOL CRayTraceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CRayTraceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CRayTraceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

/////////////////////////////////////////////////////////////////////////////
// CRayTraceView クラスの診断

#ifdef _DEBUG
void CRayTraceView::AssertValid() const
{
	CView::AssertValid();
}

void CRayTraceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRayTraceDoc* CRayTraceView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRayTraceDoc)));
	return (CRayTraceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRayTraceView クラスのメッセージ ハンドラ

int CRayTraceView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!(m_TimerID = SetTimer(0,1,NULL)))
		MessageBox("SetTimer is returned to 0!");

	CRayTraceDoc	*pDoc = GetDocument();

	m_Viewport.SetDocument(pDoc);
	m_Viewport.AttachRoot(&(pDoc->m_Root));

	return 0;
}
	
void CRayTraceView::GetVectorFromPoint(sp& k, sp& l, int px, int py)
{
	double rx = (m_View.right - m_View.left) * px / m_ClientSize.cx + m_View.left;
	double ry = (m_View.bottom - m_View.top) * py / m_ClientSize.cx + m_View.top;

	k = sp(0.01 * rx / (m_View.right - m_View.left), 0.01 * ry / (m_View.bottom - m_View.top), 0.01);
	l = sp(rx, ry, -20);

	matrix m = m_Viewport.getMatrix().Inv();
	k = m * (k + l) - m * l;
	l = m * l;
}

void CRayTraceView::OnTimer(UINT nIDEvent) 
{
	CRayTraceDoc	*pDoc = GetDocument();

	for (int i = 0; i < 1000; i ++) {
		if (m_Job == CONTINUED) {
			sp k, l;
			GetVectorFromPoint(k, l, m_NowX, m_NowY);
			sp	   c  = pDoc->m_Root.GetColor(k, l, NULL);
			m_MemoryDC.FillSolidRect(CRect(m_NowX, m_NowY, m_NowX+m_NowSize, m_NowY+m_NowSize), RGB(c.x, c.y, c.z));
			m_Job = Go_ahead(m_NowX, m_NowY, m_NowSize, m_StartX, m_StartY, m_ClientSize, START_SQUARE);
		}
	}
	if (m_ViewMode == eRayTrace || m_ViewMode == eWireFrameWithRayTrace)
		Invalidate(FALSE);
}

int CRayTraceView::Go_ahead(int& X, int& Y, int& S, int& X0, int& Y0, CSize& cs, int MAX )
{
	int	flag = 0; 

	X += MAX;
	if (X > cs.cx) {
		X = X0, Y += MAX;
		if ( Y > cs.cy )
			if ((X0 == (MAX-1)) && (Y0 == (MAX-1)))
   				return FINISHED;
			else {
				while (flag == 0) {
					if (X0 % S == 0 && Y0 % S == 0) {
						flag = 1, X0 += S;
      					if ( S < MAX && X0 % (2 * S) == 0 && Y0 % (2 * S) == 0 )
							X0 += S;
						if (X0 > MAX) {
       						X0 = 0 , Y0 += S ;
       						if (Y0 > MAX)
        						S /= 2, X0 = S, Y0 = 0;
						}
					} else
						S /= 2;
    			}
				X = X0, Y = Y0;
			}
	}
	return CONTINUED;
}

void CRayTraceView::OnSize(UINT nType, int cx, int cy) 
{
	m_ClientSize.cx = cx;
	m_ClientSize.cy = cy;
	
	if (!m_Iconized && nType != SIZE_MINIMIZED)	{
		m_MemoryDC.DeleteDC();
		m_MemoryDC.CreateCompatibleDC(NULL);
		CBitmap	b;
		b.CreateCompatibleBitmap(&CClientDC(NULL), cx, cy);
		m_MemoryDC.SelectObject(&b);
		m_StartX = m_StartY = m_NowX = m_NowY = 0;
		m_NowSize = START_SQUARE;
		m_Job = CONTINUED;
	}

	if (nType == SIZE_MINIMIZED) {
		if (!m_Iconized)
			m_Iconized = TRUE;
	} else {
		if (m_Iconized)
			m_Iconized = FALSE;
	}

	switch (m_ViewMode) {
	case eD3DWireFrame:
	case eD3DFlatShading:
	case eD3DGouraudShading:
		UpdateDevice();
	}
}

void CRayTraceView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_SelectedNode && m_Viewport.SetManipulatorAxis(*this, point, matrix(4,4))) {
		Invalidate();
		return;
	}

	Info	info;
	CRayTraceDoc	*pDoc = GetDocument();
	sp k, l;
	GetVectorFromPoint(k, l, point.x, point.y);

	if (pDoc->m_Root.GetInfo2(k, l, info)) {
		m_SelectedNode = (Node*)info.pNode;
	} else {
		m_SelectedNode = NULL;
	}

	m_AltStart = point;
}

void CRayTraceView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRayTraceDoc	*pDoc = GetDocument();
	Info	info;
	sp k, l;
	GetVectorFromPoint(k, l, point.x, point.y);

	if (pDoc->m_Root.GetInfo2(k, l, info)) {
		m_SelectedNode = (Node*)info.pNode;
	}

	Invalidate();
}

void CRayTraceView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_AltStart = point;
}

void CRayTraceView::UpdateDevice()
{
    if (m_pd3dDevice) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}

	extern LPDIRECT3D9	g_pD3D;
    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.BackBufferWidth = m_ClientSize.cx;
	d3dpp.BackBufferHeight = m_ClientSize.cy;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetSafeHwnd(),
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &m_pd3dDevice))) {
		AfxMessageBox("Failed to CreateDevice");
		return;
	}

	// Turn off culling
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn on the zbuffer
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	while (!m_lstGeometry.IsEmpty())
		m_lstGeometry.RemoveTail().Clear();

	// DirectX の視点移動は高速化の余地があり、その場合、以下の m_Viewport を GetDocument()->m_Root に変更する等...
	m_Viewport.AddGeometry(m_pd3dDevice, m_lstGeometry, *this, matrix());
}

void CRayTraceView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch (m_ViewMode)
	{
	case eD3DWireFrame:
	case eD3DFlatShading:
	case eD3DGouraudShading:
		UpdateDevice();
	case eWireFrame:
	case eWireFrameWithRayTrace:
		Invalidate();
		if (m_ViewMode == eWireFrame)
			break;
	case eRayTrace:
		m_StartX = m_StartY = m_NowX = m_NowY = 0;
		m_NowSize = START_SQUARE;
		m_Job = CONTINUED;
		break;
	}
}

void CRayTraceView::OnViewRaytrace()
{
	m_ViewMode = eRayTrace;
	Invalidate();
}

void CRayTraceView::OnViewWireframe()
{
	m_ViewMode = eWireFrame;
	Invalidate();
}

void CRayTraceView::OnViewWireframeWithRaytrace()
{
	m_ViewMode = eWireFrameWithRayTrace;
	Invalidate();
}

void CRayTraceView::OnViewD3dwireframe()
{
	m_ViewMode = eD3DWireFrame;
	UpdateDevice();
	Invalidate();
}

void CRayTraceView::OnViewFlatshading()
{
	m_ViewMode = eD3DFlatShading;
	UpdateDevice();
	Invalidate();
}

void CRayTraceView::OnViewGouraudshading()
{
	m_ViewMode = eD3DGouraudShading;
	UpdateDevice();
	Invalidate();
}

void CRayTraceView::OnUpdateViewRaytrace(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eRayTrace); }
void CRayTraceView::OnUpdateViewWireframe(CCmdUI* pCmdUI) {	pCmdUI->SetCheck(m_ViewMode == eWireFrame); }
void CRayTraceView::OnUpdateViewD3dwireframe(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eD3DWireFrame); }
void CRayTraceView::OnUpdateViewFlatshading(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eD3DFlatShading); }
void CRayTraceView::OnUpdateViewGouraudshading(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eD3DGouraudShading); }

BOOL CRayTraceView::OnEraseBkgnd(CDC* pDC) 
{
    CBrush * pOldBrush = (CBrush*)pDC->SelectStockObject( GRAY_BRUSH );
    CRect rect;
    pDC->GetClipBox( &rect );
    pDC->PatBlt( rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject( pOldBrush );

    return TRUE;
}

void CRayTraceView::OnMouseMove(UINT nFlags, CPoint point) 
{
	POINT	d;
	d.x = m_AltStart.x - point.x;
	d.y = m_AltStart.y - point.y;
	m_AltStart = point;

	Node* p = m_SelectedNode ? m_SelectedNode : &m_Viewport;
	eAxis axis;
	eType type;
		
	if (m_Alt) {
		p = &m_Viewport;
		axis = eNONE;
		switch (nFlags) {
		case MK_LBUTTON: type = eROTATE; break;
		case MK_MBUTTON: type = eMOVE;	break;
		case MK_RBUTTON: type = eSCALE;	break;
		default: return;
		}
	} else if (m_SelectedNode && (nFlags == MK_LBUTTON || nFlags == MK_MBUTTON)) {
		p = m_SelectedNode;
		axis = m_Manipulator.Axis;
		type = m_Manipulator.Type;
	} else {
		m_AltStart = point;
		return;
	}
	
	switch(type){
	case eMOVE:
		if (m_SelectedNode)
			p->Move(axis, d.x);
		else {
			p->Move(d);
		}
		break;
	case eROTATE:
		if (m_SelectedNode)
			p->Rotate(axis, d.x);
		else {
			p->Rotate(d);
		}
		break;
	case eSCALE:
		p->Scale(axis,d.x);
		break;
	case ePIVOT_MOVE:
		if (!m_SelectedNode)
			return;
		p->MovePivot(axis, d.x);
		break;
	default:
		return;
	}

	m_StartX = m_StartY = m_NowX = m_NowY = 0;
	m_NowSize = START_SQUARE;
	m_Job = CONTINUED;

	if (m_SelectedNode) {
		((CRayTraceDoc*)GetDocument())->UpdateAllViews(NULL);
	} else {
		switch (m_ViewMode) {
		case eRayTrace:
		case eWireFrame:
		case eWireFrameWithRayTrace:
			OnUpdate(0, 0, 0);
			break;
		case eD3DWireFrame:
		case eD3DFlatShading:
		case eD3DGouraudShading:
			// ToDo: UpdateDevice を不要に出来たら、以下を削除して高速化可能。
			OnUpdate(0, 0, 0);
			break;
		}
	}

	CView::OnMouseMove(nFlags, point);
}

void CRayTraceView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar) {
	case 81: m_Manipulator.Type = eSELECT;		break;
	case 87: m_Manipulator.Type = eMOVE;		break;
	case 69: m_Manipulator.Type = eROTATE;		break;
	case 82: m_Manipulator.Type = eSCALE;		break;
	case 45: m_Manipulator.Type = ePIVOT_MOVE;	break;
	}

	if (m_SelectedNode)
		Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRayTraceView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 18)
		m_Alt = TRUE;

	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CRayTraceView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 18)
		m_Alt = FALSE;

	CView::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CRayTraceView::DestroyWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	return CView::DestroyWindow();
}

void CRayTraceView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	while (!m_lstGeometry.IsEmpty())
		m_lstGeometry.RemoveTail().Clear();
	
	if (m_pd3dDevice) {
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
}

