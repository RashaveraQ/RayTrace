// RayTraceView.cpp : CRayTraceView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "RayTrace.h"

#include "RayTraceDoc.h"
#include "RayTraceView.h"
#include "DoCuda.h"
#include "GetVectorFromPoint.h"
#include "MainFrm.h"

// OpenGL Graphics includes
#include <GL/glew.h>
#if defined(__APPLE__) || defined(MACOSX)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
// Sorry for Apple : unsigned int sampler is not available to you, yet...
// Let's switch to the use of PBO and glTexSubImage
#define USE_TEXSUBIMAGE2D
#else
#include <GL/freeglut.h>
#endif

// CUDA includes
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

// CUDA utilities and system includes
#include <helper_cuda.h>
#include <helper_cuda_gl.h>

#include <helper_functions.h>
#include <rendercheck_gl.h>

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
	ON_COMMAND(ID_VIEW_CUDA_RAYTRACE, &CRayTraceView::OnViewCudaRaytrace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CUDA_RAYTRACE, &CRayTraceView::OnUpdateViewCudaRaytrace)
	ON_WM_TIMER()
	ON_COMMAND(ID_CONTROL_VERTEX, &CRayTraceView::OnControlVertex)
	ON_COMMAND(ID_OBJECT_MODE, &CRayTraceView::OnObjectMode)
	ON_COMMAND(ID_SELECT_ALL, &CRayTraceView::OnSelectAll)
	ON_COMMAND(ID_COMPLETE_TOOL, &CRayTraceView::OnCompleteTool)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME_WITH_RAYTRACE, &CRayTraceView::OnUpdateViewWireframeWithRaytrace)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTraceView クラスの構築/消滅

CRayTraceView::CRayTraceView()
{
	m_StartX = m_StartY = m_NowX = m_NowY = 0;
	m_NowSize = START_SQUARE;
	m_View.left = -10;
	m_View.right = 10;
	m_View.top = -10;
	m_View.bottom = 10;
	m_ViewMode = eWireFrame;
	m_CudaRayTraceView = false;
	m_Alt = FALSE;
	m_AltStart.x = m_AltStart.y = 0;
	m_pActiveNode = NULL;
	m_Manipulator.Type = eSELECT;
	m_Manipulator.Axis = eNONE;
	m_pd3dDevice = NULL;
	m_vEyePt = D3DXVECTOR3(0.0f, 5.0f, -30.0f);
	m_vLookatPt = D3DXVECTOR3(0.0f, 10.0f, 0.0f);

	pbo_dest = 0;
	cuda_pbo_dest_resource = NULL;
	tex_cudaResult = 0;
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

// display image to the screen as textured quad
static void displayImage(GLuint texture, int window_width, int window_height)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, window_width, window_height);


	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.5);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.5);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.5);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0.5);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	SDK_CHECK_ERROR_GL();
}

bool CRayTraceView::setManipulatorAxis(const CPoint& point/*, const matrix& mat*/)
{
	if (m_Manipulator.Type == eSELECT)
		return false;

	const Viewport& viewport = m_Viewport;
	matrix mat;
	if (!viewport.getManipulatorMatrix(mat))
		return false;

	const CSize& size = m_ClientSize;

	sp	p0 = mat * sp(0, 0, 0);
	sp	px, py, pz;

	int r = 100;

	switch (m_Manipulator.Type) {
	case ePIVOT_MOVE:
		r = 50;
	case eMOVE:
	case eSCALE:
		px = p0 + viewport.m_Rotate * sp(0, 0, -2);
		py = p0 + viewport.m_Rotate * sp(0, -2, 0);
		pz = p0 + viewport.m_Rotate * sp(-2, 0, 0);
		break;
	default:
		px = mat * sp(0, 0, -1/* / m_Scale.get_data(3, 3)*/);
		py = mat * sp(0, -1 /*/ m_Scale.get_data(2, 2)*/, 0);
		pz = mat * sp(-1 /*/ m_Scale.get_data(1, 1)*/, 0, 0);
	}

	POINT P0, PX, PY, PZ;
	p0.getPOINT(P0.x, P0.y, size.cx, size.cy);
	px.getPOINT(PX.x, PX.y, size.cx, size.cy);
	py.getPOINT(PY.x, PY.y, size.cx, size.cy);
	pz.getPOINT(PZ.x, PZ.y, size.cx, size.cy);

	switch (m_Manipulator.Type)
	{
		// case eMOVE:
		// point が P0-PX 線上の場合は、rtv.m_Manipulator.Axis = eX;
		// point が P0-PY 線上の場合は、rtv.m_Manipulator.Axis = eY;
		// point が P0-PZ 線上の場合は、rtv.m_Manipulator.Axis = eZ;
		// break;
	case eROTATE:
	{
		eAxis tbl[] = { eX, eZ, eY };
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 50; j++) {
				float th = (i == 2) ? (6.28f * j / 50) : (3.14f * i / 2);
				float ph = (i == 2) ? (3.14f / 2) : (6.28f * j / 50);
				sp p = mat * sp(2 * cos(th)*sin(ph), 2 * cos(ph), 2 * sin(th)*sin(ph));
				float dx = (P0.x + (r - 10) * (p - p0).e().x) - point.x;
				float dy = (P0.y + (r - 10) * (p - p0).e().y) - point.y;
				if (dx*dx + dy*dy < 20) {
					m_Manipulator.Axis = tbl[i];
					return true;
				}
			}
		}
	}

	return false;
	break;
	case eMOVE: // temp
	case ePIVOT_MOVE: // temp
	case eSCALE:
		if (::abs(PX.x - point.x) + ::abs(PX.y - point.y) <= 20)
			m_Manipulator.Axis = eX;
		else if (::abs(PY.x - point.x) + ::abs(PY.y - point.y) <= 20)
			m_Manipulator.Axis = eY;
		else if (::abs(PZ.x - point.x) + ::abs(PZ.y - point.y) <= 20)
			m_Manipulator.Axis = eZ;
		else if (::abs(P0.x - point.x) + ::abs(P0.y - point.y) <= 20)
			m_Manipulator.Axis = eNONE;
		else
			return false;
		break;
	default:
		return false;
	}
	return true;
}

void CRayTraceView::drawManipulator(CDC* pDC) const
{
	const Viewport& viewport = m_Viewport;
	matrix mat;
	if (!viewport.getManipulatorMatrix(mat))
		return;

	static COLORREF s_color = RGB(255, 255, 0);

	CPen	
		x_pen(PS_SOLID, 1, (m_Manipulator.Axis == eX) ? s_color : RGB(255, 0, 0)),
		y_pen(PS_SOLID, 1, (m_Manipulator.Axis == eY) ? s_color : RGB(0, 255, 0)),
		z_pen(PS_SOLID, 1, (m_Manipulator.Axis == eZ) ? s_color : RGB(0, 0, 255)),
		c_pen(PS_SOLID, 1, (m_Manipulator.Axis == eNONE) ? s_color : RGB(0, 255, 255)),
		g_pen(PS_SOLID, 1, RGB(127, 127, 127));

	const CSize& size = m_ClientSize;

	sp	p0 = mat * sp(0, 0, 0);
	sp	px, py, pz;

	int r = 100;
	switch (m_Manipulator.Type) {
	case ePIVOT_MOVE:
		r = 50;
	case eMOVE:
	case eSCALE:
		px = p0 + viewport.m_Rotate * sp(0, 0, -2);
		py = p0 + viewport.m_Rotate * sp(0, -2, 0);
		pz = p0 + viewport.m_Rotate * sp(-2, 0, 0);
		break;
	default:
		px = mat * sp(0, 0, -1);
		py = mat * sp(0, -1, 0);
		pz = mat * sp(-1, 0, 0);
	}

	POINT P0, PX, PY, PZ;
	p0.getPOINT(P0.x, P0.y, size.cx, size.cy);
	px.getPOINT(PX.x, PX.y, size.cx, size.cy);
	py.getPOINT(PY.x, PY.y, size.cx, size.cy);
	pz.getPOINT(PZ.x, PZ.y, size.cx, size.cy);

	// X-Axis
	CPen *old_pen;
	CPen *pens[] = { &x_pen, &z_pen, &y_pen };

	switch (m_Manipulator.Type)
	{
	case eMOVE:
	case ePIVOT_MOVE:
		old_pen = pDC->SelectObject(&z_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PZ);
		pDC->SelectObject(&y_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PY);
		pDC->SelectObject(&x_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PX);
		pDC->SelectObject(&c_pen);
		pDC->Rectangle(P0.x - 10, P0.y - 10, P0.x + 10, P0.y + 10);
		break;
	case eROTATE:
		old_pen =
			pDC->SelectObject(&g_pen);

		int i, j;
		float th, ph;
		POINT	P[50];

		for (i = 0; i < 3; i++) {
			for (j = 0; j < 50; j++) {
				th = (i == 2) ? (6.28f * j / 50) : (3.14f * i / 2);
				ph = (i == 2) ? (3.14f / 2) : (6.28f * j / 50);
				sp p = mat * sp(2 * cosf(th) * sinf(ph), 2 * cosf(ph), 2 * sinf(th) * sinf(ph));
				P[j].x = (long)(P0.x + (r - 10) * (p - p0).e().x);
				P[j].y = (long)(P0.y + (r - 10) * (p - p0).e().y);
			}
			pDC->SelectObject(pens[i]);
			pDC->Polygon(P, 50);
		}
		pDC->SelectObject(&c_pen);
		pDC->Ellipse(P0.x - r, P0.y - r, P0.x + r, P0.y + r);
		break;

	case eSCALE:
		old_pen = pDC->SelectObject(&g_pen);
		pDC->MoveTo(PX);
		pDC->LineTo(P0);
		pDC->LineTo(PY);
		pDC->MoveTo(P0);
		pDC->LineTo(PZ);
		pDC->SelectObject(&x_pen);
		pDC->Ellipse(PX.x - 5, PX.y - 5, PX.x + 5, PX.y + 5);
		pDC->SelectObject(&y_pen);
		pDC->Ellipse(PY.x - 5, PY.y - 5, PY.x + 5, PY.y + 5);
		pDC->SelectObject(&z_pen);
		pDC->Ellipse(PZ.x - 5, PZ.y - 5, PZ.x + 5, PZ.y + 5);
		pDC->SelectObject(&c_pen);
		pDC->Ellipse(P0.x - 5, P0.y - 5, P0.x + 5, P0.y + 5);
		break;
	default:
		return;
	}
	pDC->SelectObject(old_pen);
}

void CRayTraceView::OnDraw(CDC* pDC)
{
	DWORD t1, t2, t3, t4, d1, d2, d3;
	CString str;

	if (m_CudaRayTraceView) {
		t1 = GetTickCount();

		// run the Cuda kernel
		unsigned int *out_data;

		checkCudaErrors(cudaGraphicsMapResources(1, &cuda_pbo_dest_resource, 0));
		size_t num_bytes;
		checkCudaErrors(cudaGraphicsResourceGetMappedPointer((void **)&out_data, &num_bytes, cuda_pbo_dest_resource));
		//printf("CUDA mapped pointer of pbo_out: May access %ld bytes, expected %d\n", num_bytes, size_tex_data);

		t2 = GetTickCount();

		if (!DoCuda_OnDraw(out_data, GetDocument()->m_Root.m_devNode, m_ClientSize.cx, m_ClientSize.cy, &m_View, &m_Viewport.getMatrix().Inv()))
			MessageBox(_T("Failed to DoCuda_OnDraw."));

		t3 = GetTickCount();

		// CUDA generated data in cuda memory or in a mapped PBO made of BGRA 8 bits
		// 2 solutions, here :
		// - use glTexSubImage2D(), there is the potential to loose performance in possible hidden conversion
		// - map the texture and blit the result thanks to CUDA API
		checkCudaErrors(cudaGraphicsUnmapResources(1, &cuda_pbo_dest_resource, 0));
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo_dest);

		glBindTexture(GL_TEXTURE_2D, tex_cudaResult);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_ClientSize.cx, m_ClientSize.cy, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		SDK_CHECK_ERROR_GL();
		glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

		displayImage(tex_cudaResult, m_ClientSize.cx, m_ClientSize.cy);
		glutSwapBuffers();

		t4 = GetTickCount();

		d1 = t2 - t1;
		d2 = t3 - t2;
		d3 = t4 - t3;

		extern CRayTraceApp theApp;
		str.Format(_T("GPU : %d, CPU : %d (= %d + %d)\n"), d2, d1 + d3, d1, d3);
		((CMainFrame*)theApp.m_pMainWnd)->SetStatusText(str);

		pDC->SelectStockObject(NULL_BRUSH);
		m_Viewport.Draw_Outline(pDC, *this, matrix(4, 4));
	}

	switch (m_ViewMode) {
	case eRayTrace:
	case eWireFrameWithRayTrace:
		pDC->BitBlt(0, 0, m_ClientSize.cx, m_ClientSize.cy, &m_MemoryDC, 0, 0, SRCCOPY);
		if (m_ViewMode == eRayTrace)
			break;
	case eWireFrame:
		pDC->SelectStockObject(NULL_BRUSH);
		m_Viewport.Draw_Outline(pDC, *this, matrix(4, 4));
		drawManipulator(pDC);
		break;
	case eD3DWireFrame:
	case eD3DFlatShading:
	case eD3DGouraudShading:
		if (!m_pd3dDevice)
			UpdateDevice();

		// Clear the backbuffer to a blue color
		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(127, 127, 127), 1.0f, 0);

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
			m_pd3dDevice->SetMaterial(&mtrl);

			// Set up a white, directional light, with an oscillating direction.
			// Note that many lights may be active at a time (but each one slows down
			// the rendering of our scene). However, here we are just using one. Also,
			// we need to set the D3DRS_LIGHTING renderstate to enable lighting
			D3DXVECTOR3 vecDir;
			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(D3DLIGHT9));
			light.Type = D3DLIGHT_DIRECTIONAL;
			light.Diffuse.r = 1.0f;
			light.Diffuse.g = 1.0f;
			light.Diffuse.b = 1.0f;
			vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
				-1.0f,
				sinf(timeGetTime() / 350.0f));
			D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
			light.Range = 1000.0f;
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
			D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
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
					MessageBox(_T("Error"));
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

	if (!(m_TimerID = SetTimer(0, 1, NULL)))
		MessageBox(_T("SetTimer is returned to 0!"));

	CRayTraceDoc	*pDoc = GetDocument();

	m_Viewport.SetRoot(&pDoc->m_Root);
	m_Viewport.AttachRoot(&(pDoc->m_Root));

	if (!DoCuda_Init())
		MessageBox(_T("Failed to DoCuda_Init!"));

	return 0;
}

void CRayTraceView::GetVectorFromPoint(sp& k, sp& l, int px, int py)
{
	::GetVectorFromPoint(k, l, px, py, &m_View, m_ClientSize.cx, m_ClientSize.cy, &m_Viewport.getMatrix().Inv());
}

void CRayTraceView::OnTimer(UINT_PTR nIDEvent)
{
	CRayTraceDoc	*pDoc = GetDocument();

	for (int i = 0; i < 1000; i++) {
		if (m_Job == CONTINUED) {
			sp k, l;
			GetVectorFromPoint(k, l, m_NowX, m_NowY);
			sp	   c = pDoc->m_Root.GetColor(k, l, 0, NULL, true);
			m_MemoryDC.FillSolidRect(CRect(m_NowX, m_NowY, m_NowX + m_NowSize, m_NowY + m_NowSize), RGB(c.x, c.y, c.z));
			m_Job = Go_ahead(m_NowX, m_NowY, m_NowSize, m_StartX, m_StartY, m_ClientSize, START_SQUARE);
		}
	}
	if (m_ViewMode == eRayTrace || m_ViewMode == eWireFrameWithRayTrace)
		Invalidate(FALSE);
}

int CRayTraceView::Go_ahead(int& X, int& Y, int& S, int& X0, int& Y0, CSize& cs, int MAX)
{
	int	flag = 0;

	X += MAX;
	if (X > cs.cx) {
		X = X0, Y += MAX;
		if (Y > cs.cy)
			if ((X0 == (MAX - 1)) && (Y0 == (MAX - 1)))
				return FINISHED;
			else {
				while (flag == 0) {
					if (X0 % S == 0 && Y0 % S == 0) {
						flag = 1, X0 += S;
						if (S < MAX && X0 % (2 * S) == 0 && Y0 % (2 * S) == 0)
							X0 += S;
						if (X0 > MAX) {
							X0 = 0, Y0 += S;
							if (Y0 > MAX)
								S /= 2, X0 = S, Y0 = 0;
						}
					}
					else
						S /= 2;
				}
				X = X0, Y = Y0;
			}
	}
	return CONTINUED;
}

static void deleteTexture(GLuint *tex)
{
	glDeleteTextures(1, tex);
	SDK_CHECK_ERROR_GL();
	*tex = 0;
}

static void createPBO(GLuint *pbo, struct cudaGraphicsResource **pbo_resource, unsigned int image_width, unsigned int image_height)
{
	// set up vertex data parameter
	unsigned int num_texels = image_width * image_height;
	unsigned int num_values = num_texels * 4;
	unsigned int size_tex_data = sizeof(GLubyte) * num_values;
	void *data = malloc(size_tex_data);

	// create buffer object
	glGenBuffers(1, pbo);
	glBindBuffer(GL_ARRAY_BUFFER, *pbo);
	glBufferData(GL_ARRAY_BUFFER, size_tex_data, data, GL_DYNAMIC_DRAW);
	free(data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// register this buffer object with CUDA
	checkCudaErrors(cudaGraphicsGLRegisterBuffer(pbo_resource, *pbo, cudaGraphicsMapFlagsNone));

	SDK_CHECK_ERROR_GL();
}

static void deletePBO(GLuint *pbo)
{
	glDeleteBuffers(1, pbo);
	SDK_CHECK_ERROR_GL();
	*pbo = 0;
}

static void createTextureDst(GLuint *tex_cudaResult, unsigned int size_x, unsigned int size_y)
{
	// create a texture
	glGenTextures(1, tex_cudaResult);
	glBindTexture(GL_TEXTURE_2D, *tex_cudaResult);

	// set basic parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	SDK_CHECK_ERROR_GL();
}

static GLuint compileGLSLprogram(const char *vertex_shader_src, const char *fragment_shader_src)
{
	GLuint v, f, p = 0;

	p = glCreateProgram();

	if (vertex_shader_src)
	{
		v = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(v, 1, &vertex_shader_src, NULL);
		glCompileShader(v);

		// check if shader compiled
		GLint compiled = 0;
		glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			//#ifdef NV_REPORT_COMPILE_ERRORS
			char temp[256] = "";
			glGetShaderInfoLog(v, 256, NULL, temp);
			printf("Vtx Compile failed:\n%s\n", temp);
			//#endif
			glDeleteShader(v);
			return 0;
		}
		else
		{
			glAttachShader(p, v);
		}
	}

	if (fragment_shader_src)
	{
		f = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(f, 1, &fragment_shader_src, NULL);
		glCompileShader(f);

		// check if shader compiled
		GLint compiled = 0;
		glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			//#ifdef NV_REPORT_COMPILE_ERRORS
			char temp[256] = "";
			glGetShaderInfoLog(f, 256, NULL, temp);
			printf("frag Compile failed:\n%s\n", temp);
			//#endif
			glDeleteShader(f);
			return 0;
		}
		else
		{
			glAttachShader(p, f);
		}
	}

	glLinkProgram(p);

	int infologLength = 0;
	int charsWritten = 0;

	glGetProgramiv(p, GL_INFO_LOG_LENGTH, (GLint *)&infologLength);

	if (infologLength > 0)
	{
		char *infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(p, infologLength, (GLsizei *)&charsWritten, infoLog);
		printf("Shader compilation error: %s\n", infoLog);
		free(infoLog);
	}

	return p;
}

static const char *glsl_draw_fragshader_src =
	//WARNING: seems like the gl_FragColor doesn't want to output >1 colors...
	//you need version 1.3 so you can define a uvec4 output...
	//but MacOSX complains about not supporting 1.3 !!
	// for now, the mode where we use RGBA8UI may not work properly for Apple : only RGBA16F works (default)
#if defined(__APPLE__) || defined(MACOSX)
	"void main()\n"
	"{"
	"  gl_FragColor = vec4(gl_Color * 255.0);\n"
	"}\n";
#else
	"#version 130\n"
	"out uvec4 FragColor;\n"
	"void main()\n"
	"{"
	"  FragColor = uvec4(gl_Color.xyz * 255.0, 255.0);\n"
	"}\n";
#endif

static int iGLUTWindowHandle = 0;          // handle to the GLUT window
static bool bInitGL_done = false;

////////////////////////////////////////////////////////////////////////////////
//! Initialize GL
////////////////////////////////////////////////////////////////////////////////
static bool initGL(int window_width, int window_height)
{
	int argc = 0;
	char *argv[] = { "RayTrace.exe" };

	// Create GL context
	if (bInitGL_done) {
		glutHideWindow();
		glutDestroyWindow(iGLUTWindowHandle);
	} else
		glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	iGLUTWindowHandle = glutCreateWindow("CUDA OpenGL post-processing");

	// initialize necessary OpenGL extensions
	glewInit();

	if (!glewIsSupported(
		"GL_VERSION_2_0 "
		"GL_ARB_pixel_buffer_object "
		"GL_EXT_framebuffer_object "
		))
	{
		printf("ERROR: Support for necessary OpenGL extensions missing.");
		fflush(stderr);
		return false;
	}

	// default initialization
#ifndef USE_TEXTURE_RGBA8UI
	glClearColor(0.5, 0.5, 0.5, 1.0);
#else
	glClearColorIuiEXT(128, 128, 128, 255);
#endif
	glDisable(GL_DEPTH_TEST);

	// viewport
	glViewport(0, 0, window_width, window_height);

	// projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)window_width / (GLfloat)window_height, 0.1f, 10.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_LIGHT0);
	float red[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);

	SDK_CHECK_ERROR_GL();

	return true;
}

void CRayTraceView::OnSize(UINT nType, int cx, int cy)
{
	m_ClientSize.cx = cx;
	m_ClientSize.cy = cy;
	
	if (!initGL(cx, cy)) {
		MessageBox(_T("Failed to initGL"));
		return;
	}
	bInitGL_done = true;

	if (!DoCuda_Init()) {
		MessageBox(_T("Failed to DoCuda_Init."));
		return;
	}


	if (pbo_dest) {
		checkCudaErrors(cudaGraphicsUnregisterResource(cuda_pbo_dest_resource));
		deletePBO(&pbo_dest);
		pbo_dest = 0;
	}

	if (tex_cudaResult) {
		deleteTexture(&tex_cudaResult);
		tex_cudaResult = 0;
	}
	// create pbo
	createPBO(&pbo_dest, &cuda_pbo_dest_resource, cx, cy);
	// create texture that will receive the result of CUDA
	createTextureDst(&tex_cudaResult, cx, cy);
	// load shader programs
	GLuint shDraw = compileGLSLprogram(NULL, glsl_draw_fragshader_src);
	SDK_CHECK_ERROR_GL();

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
	}
	else {
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

Selectable* CRayTraceView::GetSelectable(const CPoint& point)
{
	sp k, l;
	GetVectorFromPoint(k, l, point.x, point.y);
	return ((CRayTraceDoc*)GetDocument())->m_Root.GetSelectable(k, l);
}

void CRayTraceView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_Alt)
		return;
	if (setManipulatorAxis(point)) {
		Invalidate();
		return;
	}
	m_SelectionRect.SetRect(point, point);
	m_AltStart = point;
}

void CRayTraceView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_Alt)
		return;

	m_SelectionRect.SetRect(m_SelectionRect.TopLeft(), point);
	m_SelectionRect.NormalizeRect();
	m_SelectionRect.InflateRect(3, 3);

	// シフトキーを押していない場合は、全ての選択を解除する。
	if (!(nFlags & MK_SHIFT) && m_Viewport.ResetSelection())
		Invalidate();

	// 選択を切り替える
	if (m_Viewport.ChangeSelection(&m_SelectionRect, m_ClientSize.cx, m_ClientSize.cy, matrix(4,4)))
		Invalidate();
	
	//m_SelectedNode = GetSelectable(point);
}

void CRayTraceView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_AltStart = point;

	if (m_Alt || (nFlags & MK_SHIFT))
		return;

	CMenu cMenu;
	cMenu.LoadMenu(IDR_POPUP_MENU);

	m_pActiveNode = GetSelectable(point);
	CMenu* popup = cMenu.GetSubMenu((!m_pActiveNode) ? 0 : (dynamic_cast<NurbsPrimitive*>(m_pActiveNode)) ? 1 : 2);
	ClientToScreen(&point);
	popup->TrackPopupMenu(0, point.x, point.y, this);
	cMenu.DestroyMenu();
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
		AfxMessageBox(_T("Failed to CreateDevice"));
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

void CRayTraceView::OnViewCudaRaytrace()
{
	m_CudaRayTraceView = !m_CudaRayTraceView;
}

void CRayTraceView::OnUpdateViewRaytrace(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eRayTrace); }
void CRayTraceView::OnUpdateViewWireframe(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eWireFrame); }
void CRayTraceView::OnUpdateViewD3dwireframe(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eD3DWireFrame); }
void CRayTraceView::OnUpdateViewFlatshading(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eD3DFlatShading); }
void CRayTraceView::OnUpdateViewGouraudshading(CCmdUI* pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eD3DGouraudShading); }
void CRayTraceView::OnUpdateViewCudaRaytrace(CCmdUI *pCmdUI) { pCmdUI->SetCheck(m_CudaRayTraceView); }
void CRayTraceView::OnUpdateViewWireframeWithRaytrace(CCmdUI *pCmdUI) { pCmdUI->SetCheck(m_ViewMode == eWireFrameWithRayTrace); }

BOOL CRayTraceView::OnEraseBkgnd(CDC* pDC)
{
	CBrush * pOldBrush = (CBrush*)pDC->SelectStockObject(GRAY_BRUSH);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);

	return TRUE;
}

void CRayTraceView::OnMouseMove(UINT nFlags, CPoint point)
{
	POINT	d;
	d.x = m_AltStart.x - point.x;
	d.y = m_AltStart.y - point.y;
	m_AltStart = point;

	eAxis axis;
	eType type;

	if (m_Alt) {
		axis = eNONE;
		switch (nFlags) {
		case MK_LBUTTON: type = eROTATE;break;
		case MK_MBUTTON: type = eMOVE;	break;
		case MK_RBUTTON: type = eSCALE;	break;
		default: return;
		}
	}
	else if (nFlags == MK_LBUTTON || nFlags == MK_MBUTTON) {
		axis = m_Manipulator.Axis;
		type = m_Manipulator.Type;
	} else {
		m_AltStart = point;
		return;
	}

	switch (type){
	case eMOVE:
		if (m_Alt)
			m_Viewport.Move2(d);
		else
			m_Viewport.Move(axis, (float)d.x);
		break;
	case eROTATE:
		if (m_Alt)
			m_Viewport.Rotate2(d);
		else
			m_Viewport.Rotate(axis, (float)d.x);
		break;
	case eSCALE:
		m_Viewport.Scale(axis, (float)d.x);
		break;
	case ePIVOT_MOVE:
		m_Viewport.MovePivot(axis, (float)d.x);
		break;
	default:
		return;
	}

	m_StartX = m_StartY = m_NowX = m_NowY = 0;
	m_NowSize = START_SQUARE;
	m_Job = CONTINUED;

	if (m_pActiveNode) {
		((CRayTraceDoc*)GetDocument())->UpdateAllViews(NULL);
	} else {
		if (m_CudaRayTraceView)
			OnUpdate(0, 0, 0);
		else
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
	switch (nChar) {
	case 81: m_Manipulator.Type = eSELECT;		break;
	case 87: m_Manipulator.Type = eMOVE;		break;
	case 69: m_Manipulator.Type = eROTATE;		break;
	case 82: m_Manipulator.Type = eSCALE;		break;
	case 45: m_Manipulator.Type = ePIVOT_MOVE;	break;	// Insert Key
	}

	if (m_pActiveNode)
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

void CRayTraceView::OnControlVertex()
{
	if (((NurbsPrimitive*)m_pActiveNode)->OnControlVertex())
		Invalidate();
}

void CRayTraceView::OnObjectMode()
{
	if (m_pActiveNode->OnObjectMode())
		Invalidate();
}

void CRayTraceView::OnSelectAll()
{
	// TODO: Add your command handler code here
}

void CRayTraceView::OnCompleteTool()
{
	// TODO: Add your command handler code here
}


