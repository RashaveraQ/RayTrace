﻿#include "stdafx.h"
#include "RayTraceView.h"

IMPLEMENT_SERIAL(Sphere, CObject, 1)

Boundary Sphere::sBoundary = Boundary(1);

bool Sphere::newDeviceNode()
{
	bool newDevSphere(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevSphere(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

Sphere::Sphere(Node* const root, const TCHAR* const Name, const sp Color)
	: Node(root, eSPHERE, Name, Color)
{
	if (!newDeviceNode())
		exit(1);
}

Sphere::Sphere(const Sphere& other)
	: Node(other)
{
	if (!newDeviceNode())
		exit(1);
}

void Sphere::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	const CSize& size = raytraceview.m_ClientSize;
	matrix m = mat * m_Matrix;

	pDC->SelectStockObject(m_Selected ? WHITE_PEN : BLACK_PEN);

	POINT	P[50];

	int i, j;
	float th, ph;

	sp p0 = m * sp(0, 0, 0);

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 50; j++) {
			th = 3.14f * i / 5;
			ph = 6.28f * j / 50;
			sp p = m * sp(cosf(th) * sinf(ph), cosf(ph), sinf(th) * sinf(ph));
			p.getPOINT(P[j].x, P[j].y, size.cx, size.cy);
		}
		pDC->Polygon(P, 50);
	}

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 50; j++) {
			ph = 3.14f * i / 5;
			th = 6.28f * j / 50;
			sp p = m * sp(cosf(th) * sinf(ph), cosf(ph), sinf(th) * sinf(ph));
			p.getPOINT(P[j].x, P[j].y, size.cx, size.cy);
		}
		pDC->Polygon(P, 50);
	}
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Sphere::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& mat) const
{
	// D3DXCreateSphere の使用を検討すること
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	matrix m = mat * m_Matrix;

	switch (rtv.m_ViewMode) {
	case CRayTraceView::eD3DWireFrame:
		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 500))
			return;

		int i;
		for (i = 0; i < 5; i++) {
			for (int j = 0; j < 50; j++) {
				float th = 3.14f * i / 5;
				float ph = 6.28f * j / 50;
				sp p = m * sp(cosf(th)*sinf(ph), cosf(ph), sinf(th) * sinf(ph));
				pVertices[50 * i + j].position = pVertices[50 * i + j].normal = D3DXVECTOR3(p.x, p.y, p.z);
			}
		}

		for (i = 0; i < 5; i++) {
			for (int j = 0; j < 50; j++) {
				float ph = 3.14f * i / 5;
				float th = 6.28f * j / 50;
				sp p = m * sp(cosf(th) * sinf(ph), cosf(ph), sinf(th) * sinf(ph));
				pVertices[250 + 50 * i + j].position = pVertices[250 + 50 * i + j].normal = D3DXVECTOR3(p.x, p.y, p.z);
			}
		}
		pVB->Unlock();

		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, 500 - 1));
		break;
	case CRayTraceView::eD3DFlatShading:
	case CRayTraceView::eD3DGouraudShading:

		LPD3DXMESH pMesh;

		if (FAILED(D3DXCreateSphere(pd3dDevice, 1, 50, 50, &pMesh, NULL)))
			return;

		//pMesh->LockVertexBuffer(D3DLOCK_NOOVERWRITE,
		/*

		if (FAILED(pMesh->GetVertexBuffer(&pVB)))
		return;

		if (FAILED(pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return;
		for (i = 0; i < 20 * 20; i++) {
		sp p = m * sp(pVertices[i].position.x, pVertices[i].position.y, pVertices[i].position.z);
		pVertices[i].position = pVertices[i].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
		}
		pVB->Unlock();
		*/

		lstGeometry.AddTail(Geometry(this, pMesh, m));

		break;
	}

	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

bool Sphere::IsInside(const sp& L) const
{
	return (sqrt(L * L) <= 1.0);
}

bool Sphere::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	float	a = K * K;
	float	b = K * L;
	float	c = L * L - 1.0f;

	float	bb_ac = b*b - a*c;

	if (bb_ac < 0)
		return FALSE;

	float	t;
	float	t1 = (-b + sqrt(bb_ac)) / a;
	float	t2 = (-b - sqrt(bb_ac)) / a;

	info.isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			if (pHint && pHint->pNode == this && !fromOutSide) {
				t = (t1 < t2) ? t2 : t1;
			}
			else {
				t = (t1 < t2) ? t1 : t2;
				info.isEnter = 1;
			}
		}
		else {
			t = t1;
		}
	}
	else {
		if (t2 > 0) {
			t = t2;
		}
		else
			return FALSE;
	}

	info.Cross = K * t + L;
	info.Vertical = info.isEnter ? info.Cross : -info.Cross;
	info.Distance = t * sqrt(K * K);

	float x, y, z, th, phy;

	x = info.Vertical.x;
	y = info.Vertical.y;
	z = info.Vertical.z;

	th = atan2f(y, sqrtf(x*x + z*z)) / M_PI + .5f;
	phy = atan2f(x, -z) / (2 * M_PI) + .5f;

	info.Material = GetPixel(phy, th).getMaterial();
	info.pNode = this;

	return TRUE;
}

void Sphere::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 11, 10, hParent, hInsertAfter), (DWORD_PTR)this);
}

