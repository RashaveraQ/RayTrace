#include "stdafx.h"

IMPLEMENT_SERIAL(Cone, CObject, 1)

void Cone::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const BaseNode* pNode = raytraceview.m_SelectedNode;

	matrix m = Matrix * m_Matrix;
	pDC->SelectStockObject((pNode == (BaseCone*)this) ? WHITE_PEN : BLACK_PEN);

#define COUNT	100
	
	POINT	P[COUNT];

	for (int i = 0; i < COUNT; i++) {
		double th = 6.28 * (double)i / COUNT;
		P[i] = sp(m * sp(cos(th), 1, sin(th))).getPOINT(size);
	}
	pDC->Polygon(P, COUNT);

#define LINES	8

	POINT	O = sp(m * sp(0,0,0)).getPOINT(size);
	for (int i = 0; i < LINES; i++) {
		pDC->MoveTo(O);
		pDC->LineTo(P[i*COUNT/LINES]);
	}
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Cone::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	matrix m = Matrix * m_Matrix;

	switch (rtv.m_ViewMode) {
	case CRayTraceView::eD3DWireFrame:

		LPDIRECT3DVERTEXBUFFER9 pVB;
		CUSTOMVERTEX*	pVertices;

		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, COUNT))
			return;

		int i;
		for (i = 0; i < COUNT; i++) {
			double th = 6.28 * (double)i / COUNT;
			sp p = m * sp(cos(th), 0, sin(th));
			pVertices[i].position = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			pVertices[i].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
		}
		pVB->Unlock();
		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, COUNT-1));

		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 2 * LINES))
			return;
		{
			sp p = m * sp(0,1,0);
			pVertices[0].position = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			pVertices[0].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
		}
		for (int i = 0; i < LINES; i++) {
			pVertices[2*i] = pVertices[0];
			double th = 6.28 * (double)(i)/ LINES;
			sp p = m * sp(cos(th), 0, sin(th));
			pVertices[2*i+1].position = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			pVertices[2*i+1].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
		}
		pVB->Unlock();
		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINELIST, LINES));

		break;

	case CRayTraceView::eD3DFlatShading:
	case CRayTraceView::eD3DGouraudShading:

		LPD3DXMESH pMesh;

		if (FAILED(D3DXCreateCylinder(pd3dDevice, 0, 1, 2, 50, 50, &pMesh, NULL)))
			return;
		
		lstGeometry.AddTail(Geometry(this, pMesh, m));
		break;
	}
	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

BOOL Cone::IsInside(const sp* L)
{
	return (0 <= L->y && L->y <= 1 && sqrt( L->x * L->x + L->z * L->z ) <= L->y);
}


void Cone::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 17, 16, hParent, hInsertAfter), (DWORD)this);
}

