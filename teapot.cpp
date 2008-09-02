#include "stdafx.h"
#include "RayTraceView.h"

IMPLEMENT_SERIAL(Teapot, CObject, 1)

Teapot::Teapot(const CRayTraceDoc* const pDoc, const char* const Name, const sp Color)
	: Node(pDoc, TEAPOT, Name, Color)
{

}

void Teapot::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;

	matrix m = Matrix * m_Matrix;

	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

	POINT	P[50];

	int i, j;
	double th, ph;

	sp p0 = m * sp(0,0,0);

	for (i = 0; i < 5; i++ ) {
		for (j = 0; j < 50; j++ ) {
			th = 3.14 * (double)i / 5;
			ph = 6.28 * (double)j / 50;
			sp p = m * sp(cos(th)*sin(ph), cos(ph), sin(th)*sin(ph));
			P[j] = p.getPOINT(size);
		}
		pDC->Polygon(P, 50);
	}
	
	for (i = 0; i < 5; i++ ) {
		for (j = 0; j < 50; j++ ) {
			ph = 3.14 * (double)i / 5;
			th = 6.28 * (double)j / 50;
			sp p = m * sp(cos(th)*sin(ph), cos(ph), sin(th)*sin(ph));
			P[j] = p.getPOINT(size);
		}
		pDC->Polygon(P, 50);
	}
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Teapot::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	// D3DXCreateTeapot ‚ÌŽg—p‚ðŒŸ“¢‚·‚é‚±‚Æ
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;
	
	matrix m = Matrix * m_Matrix;

	switch (rtv.m_ViewMode) {
	case CRayTraceView::eD3DWireFrame:
		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 500))
			return;

		int i;
		for (i = 0; i < 5; i++) {
			for (int j = 0; j < 50; j++) {
				double th = 3.14 * (double)i / 5;
				double ph = 6.28 * (double)j / 50;
				sp p = m * sp(cos(th)*sin(ph), cos(ph), sin(th)*sin(ph));
				pVertices[50*i+j].position = pVertices[50*i+j].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			}
		}
		
		for (i = 0; i < 5; i++) {
			for (int j = 0; j < 50; j++) {
				double ph = 3.14 * (double)i / 5;
				double th = 6.28 * (double)j / 50;
				sp p = m * sp(cos(th)*sin(ph), cos(ph), sin(th)*sin(ph));
				pVertices[250+50*i+j].position = pVertices[250+50*i+j].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			}
		}
		pVB->Unlock();

		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, 500-1));
		break;
	case CRayTraceView::eD3DFlatShading:
	case CRayTraceView::eD3DGouraudShading:

		LPD3DXMESH pMesh;

		if (FAILED(D3DXCreateTeapot(pd3dDevice, &pMesh, NULL)))
			return;

		lstGeometry.AddTail(Geometry(this, pMesh, m));

		break;
	}

	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

BOOL Teapot::IsInside(const sp* L)
{
	return (sqrt((*L) * (*L)) <= 1.0);
}


void Teapot::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 25, 24, hParent, hInsertAfter), (DWORD)this);
}

