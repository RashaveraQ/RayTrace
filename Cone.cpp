#include "stdafx.h"

IMPLEMENT_SERIAL(Cone, CObject, 1)

void Cone::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;

	matrix m = Matrix * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

#define COUNT	100
	
	POINT	P[COUNT];

	for (int i = 0; i < COUNT; i++) {
		float th = 6.28f * (float)i / COUNT;
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
			float th = 6.28f * (float)i / COUNT;
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
			float th = 6.28f * (float)(i)/ LINES;
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

BOOL Cone::IsInside(const sp& L) const
{
	return (0 <= L.y && L.y <= 1 && sqrt( L.x * L.x + L.z * L.z ) <= L.y);
}

BOOL Cone::GetInfo(const sp& K, const sp& L, Info& info) const
{
	if (L.y > 1) {

		if (K.y >= 0)
			return FALSE;

		float t = (1 - L.y) / K.y;

		sp p = K*t + L;

		if (p.x * p.x + p.z * p.z <= 1) {
			info.Cross = p;
			info.Vertical = sp(0,1,0);
			info.Distance = t * sqrt(K*K);
			info.isEnter = 1;
			info.Material = GetPixel(.5f*(p.x+1),.5f*(p.z+1)).getMaterial();
			info.pNode = this;
			info.Refractive = m_Refractive;

			return TRUE;
		}
	}

	float	a, b, c, d, t, t1, t2;

	c = K.x * L.y - K.y * L.x, c *= c, d = c;
	c = K.z * L.y - K.y * L.z, c *= c, d += c;
	c = K.x * L.z - K.z * L.x, c *= c, d -= c;

	if ( d < 0 )
		return FALSE;

	d = sqrt( d );

	a = -( K.x * L.x + K.z * L.z - K.y * L.y );
	b = K.x * K.x + K.z * K.z - K.y * K.y;

	t1 = ( a + d ) / b;
	t2 = ( a - d ) / b;

	if (fabs(t1) < 1E-10 || fabs(t2) < 1E-10)
		return FALSE;

	if ( t1 > 0 )
	{
		if ( t2 > 0 )
		{
			if ( t1 < t2 )
				t = ( K.y * t1 + L.y > 0 ) ? t1 : t2;
			else
				t = ( K.y * t2 + L.y > 0 ) ? t2 : t1;
		}
		else
			t = t1;
	}
	else
	{
		if ( t2 > 0 )
			t = t2;
		else
			return FALSE;
	}

	info.isEnter = !IsInside( L );
	sp p = K * t + L;
	info.Cross = info.Vertical = p;

	if (p.y < 0 || p.x*p.x + p.z*p.z > 1) 
		return FALSE;

	info.Vertical.y *= -1;

	info.Distance = t * sqrt( K * K );
	info.Material = m_Material;
	info.pNode = this;

	return TRUE;
}

void Cone::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 17, 16, hParent, hInsertAfter), (DWORD)this);
}

