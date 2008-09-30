#include "stdafx.h"

IMPLEMENT_SERIAL(Cylinder, CObject, 1)

void Cylinder::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;

	matrix m = Matrix * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

#define COUNT	100

	POINT	P[2][COUNT];

	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < COUNT; i++) {
			double th = 6.28 * (double)i / COUNT;
			sp	p = m * sp(cos(th), j == 0 ? -1 : 1, sin(th));
			P[j][i] = p.getPOINT(size);
		}
		pDC->Polygon(P[j], COUNT);
	}

#define LINES	8

	POINT O1 = sp(m * sp(0,-1,0)).getPOINT(size);
	POINT O2 = sp(m * sp(0, 1,0)).getPOINT(size);

	for (int i = 0; i < LINES; i++) {
		pDC->MoveTo(O1);
		pDC->LineTo(P[0][i*COUNT/LINES]);
		pDC->LineTo(P[1][i*COUNT/LINES]);
		pDC->LineTo(O2);
	}
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Cylinder::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	matrix m = Matrix * m_Matrix;

	switch (rtv.m_ViewMode) {
	case CRayTraceView::eD3DWireFrame:

		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 2*COUNT+4*LINES))
			return;

		int i,j;

		for (j = 0; j < 2; j++) {
			for (i = 0; i < COUNT; i++) {
				double th = 6.28 * (double)i / COUNT;
				sp	p = m * sp(cos(th), j == 0 ? -1 : 1, sin(th));
				pVertices[COUNT*j+i].position = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
				pVertices[COUNT*j+i].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			}
		}

		{
			sp o1= sp(m * sp(0,-1,0));
			sp o2= sp(m * sp(0,1,0));
			for (i = 0; i < LINES; i++) {
				pVertices[2*COUNT+4*i].position = D3DXVECTOR3((float)o1.x, (float)o1.y, (float)o1.z);
				pVertices[2*COUNT+4*i].normal = D3DXVECTOR3((float)o1.x, (float)o1.y, (float)o1.z);
				pVertices[2*COUNT+4*i+1] = pVertices[i*COUNT/LINES];
				pVertices[2*COUNT+4*i+2] = pVertices[COUNT+i*COUNT/LINES];
				pVertices[2*COUNT+4*i+3].position = D3DXVECTOR3((float)o2.x, (float)o2.y, (float)o2.z);
				pVertices[2*COUNT+4*i+3].normal = D3DXVECTOR3((float)o2.x, (float)o2.y, (float)o2.z);
			}
		}
		pVB->Unlock();

		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, 2*COUNT+4*LINES-1));
		break;
	case CRayTraceView::eD3DFlatShading:
	case CRayTraceView::eD3DGouraudShading:

		LPD3DXMESH pMesh;

		if (FAILED(D3DXCreateCylinder(pd3dDevice, 1, 1, 1, 50, 50, &pMesh, NULL)))
			return;
		
		lstGeometry.AddTail(Geometry(this, pMesh, m));
		break;
	}
	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

BOOL Cylinder::IsInside(const sp& L) const
{
	return (-1 <= L.y && L.y <= 1 && sqrt(L.x*L.x+L.z*L.z) <= 1.0);
}

BOOL Cylinder::GetInfo(const sp& K, const sp& L, Info& info) const
{
	if (L.y < -1)
	{
		if (K.y <= 0)
			return FALSE;

		double t = -(1 + L.y)/K.y;

		sp	p = K*t+L;

		if (p.x * p.x + p.z * p.z <= 1) {
			info.Cross = p;
			info.Vertical = sp(0,-1,0);
			info.Distance = t * sqrt(K*K);
			info.isEnter = 1;
			info.Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info.pNode = this;
			return TRUE;
		}
	}

	if (L.y > 1)
	{
		if (K.y >= 0)
			return FALSE;

		double t = (1 - L.y) / K.y;

		sp	p = K*t+L;

		if (p.x * p.x + p.z * p.z <= 1) {
			info.Cross = p;
			info.Vertical = sp(0,1,0);
			info.Distance = t * sqrt(K*K);
			info.isEnter = 1;
			info.Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info.pNode = this;

			return TRUE;
		}
	}

	double	a, b, c, d, t, t1, t2;

	c = K.x * L.z - K.z * L.x;
	c *= c;
	a = K.x * K.x + K.z * K.z;
	d = a - c;

	if (d < 0)
		return FALSE;

	d = sqrt(d);
	b = -(K.x * L.x + K.z * L.z);

	t1 = (b + d)/a;
	t2 = (b - d)/a;

	info.isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
			info.isEnter = 1;
		} else
			t = t1;
	} else {
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	sp p = K * t + L;

	if (p.y < -1 || 1 < p.y)
		return FALSE;

	info.isEnter = !IsInside(L);
	info.Cross = info.Vertical = p;
	info.Vertical.y = 0;
	info.Distance = t * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;

	return TRUE;
}

void Cylinder::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 19, 18, hParent, hInsertAfter), (DWORD)this);
}
