#include "stdafx.h"
#include "RayTrace.h"
#include "DlgTorus.h"

IMPLEMENT_SERIAL(Torus, CObject, 1)

#define A 10
#define B 50

Torus::Torus(const CRayTraceDoc* const pDoc, const char* const Name, const sp Color)
	: BaseTorus(0.7, 0.3), Node(pDoc, TORUS, Name, Color)
{
}

void Torus::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;

	matrix m = Matrix * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

	POINT	P[B];

	int i, j;
	double th, ph;

	for (i = 0; i < A; i++) {
		for (j = 0; j < B; j++) {
			th = 6.28 * (double)i / A;
			ph = 6.28 * (double)j / B;
			sp p = m * sp((m_R+m_r*cos(ph))*cos(th), (m_R+m_r*cos(ph))*sin(th), m_r*sin(ph));
			P[j] = p.getPOINT(size);
		}
		pDC->Polygon(P, B);
	}
	
	for (i = 0; i < A; i++) {
		for (j = 0; j < B; j++) {
			ph = 6.28 * (double)i / A;
			th = 6.28 * (double)j / B;
			sp p = m * sp((m_R+m_r*cos(ph))*cos(th), (m_R+m_r*cos(ph))*sin(th), m_r*sin(ph));
			P[j] = p.getPOINT(size);
		}
		pDC->Polygon(P, B);
	}
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Torus::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	// D3DXCreateTorus ‚ÌŽg—p‚ðŒŸ“¢‚·‚é‚±‚Æ
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	matrix m = Matrix * m_Matrix;

	switch (rtv.m_ViewMode) {
	case CRayTraceView::eD3DWireFrame:
		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 2*A*B))
			return;


		int i, j;
		double th, ph;

		for (i = 0; i < A; i++) {
			for (j = 0; j < B; j++) {
				th = 6.28 * (double)i / A;
				ph = 6.28 * (double)j / B;
				sp p = m * sp((m_R+m_r*cos(ph))*cos(th), (m_R+m_r*cos(ph))*sin(th), m_r*sin(ph));
				pVertices[B*i+j].position = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
				pVertices[B*i+j].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			}
		}
		
		for (i = 0; i < A; i++) {
			for (j = 0; j < B; j++) {
				ph = 6.28 * (double)i / A;
				th = 6.28 * (double)j / B;
				sp p = m * sp((m_R+m_r*cos(ph))*cos(th), (m_R+m_r*cos(ph))*sin(th), m_r*sin(ph));
				pVertices[A*B+B*i+j].position = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
				pVertices[A*B+B*i+j].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			}
		}
		pVB->Unlock();

		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, 2*A*B-1));
		break;

	case CRayTraceView::eD3DFlatShading:
	case CRayTraceView::eD3DGouraudShading:

		LPD3DXMESH pMesh;

		if (FAILED(D3DXCreateTorus(pd3dDevice, (float)m_r, (float)m_R, 50, 50, &pMesh, NULL)))
			return;
		
		lstGeometry.AddTail(Geometry(this, pMesh, m));
		break;
	}
	
	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

BOOL Torus::IsInside(const sp* L)
{
	double	d;

	d = m_R - sqrt(L->x * L->x + L->y * L->y);
	d *= d;

	return (m_r * m_r <= d + L->z * L->z);
}


void Torus::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 21, 20, hParent, hInsertAfter), (DWORD)this);
}

void Torus::Serialize(CArchive& ar)
{
	Node::Serialize(ar);

	if ( ar.IsStoring()) {
		ar << m_r;
		ar << m_R;
	} else {
		ar >> m_r;
		ar >> m_R;
	}
}

BOOL Torus::Edit()
{
	CDlgTorus	dlg_torus;

	dlg_torus.m_r = m_r;
	dlg_torus.m_R = m_R;

	if (dlg_torus.DoModal() != IDOK)
		return FALSE;

	m_r = dlg_torus.m_r;
	m_R = dlg_torus.m_R;

	return TRUE;
}