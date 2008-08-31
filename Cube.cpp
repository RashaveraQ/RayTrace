#include "stdafx.h"

IMPLEMENT_SERIAL(Cube, CObject, 1)

BOOL Cube::GetInfo(const sp* K, const sp* L, Info* info)
{
	info->isEnter = !IsInside(L);

	if (info->isEnter)
	{
		//外側から外に向かう場合、
		if ((L->x < -1.0 && K->x <= 0.0) || (1.0 < L->x && 0.0 <= K->x) ||
			(L->y < -1.0 && K->y <= 0.0) || (1.0 < L->y && 0.0 <= K->y) ||
			(L->z < -1.0 && K->z <= 0.0) || (1.0 < L->z && 0.0 <= K->z))
			 return FALSE;
	}

	int		i, j, k;
	double	T[6], t1, t;
	sp		c;

	// 全ての面までの距離を求める。
	T[0] = (1.0 - L->x) / K->x; T[1] = - (1.0 + L->x) / K->x;
	T[2] = (1.0 - L->y) / K->y; T[3] = - (1.0 + L->y) / K->y;
	T[4] = (1.0 - L->z) / K->z; T[5] = - (1.0 + L->z) / K->z;

	// t1 に最大値を代入する。
	for (i = 1, t1 = T[0]; i < 6; i++)
		if (t1 < T[i])
			t1 = T[i];

	// 負の場合、t1 を代入する。
	for (i = 0; i < 6; i++)
		if (T[i] < 0.0)
			T[i] = t1;

	for (k = 0; k < 3; k++)
	{
		for (i = 1, j = 0, t = T[0]; i < 6; i++)
			if (t > T[i])
				t = T[i], j = i;

		if (!info->isEnter)
			break;

		c = (*K) * t + (*L);

		if (-1.0 <= c.x && c.x <= 1.0 && -1.0 <= c.y && c.y <= 1.0 && -1.0 <= c.z && c.z <= 1.0)
			break;

		T[j] = t1;
	}

	if (k == 3)
		return FALSE;

	info->Cross = (*K) * t + (*L);
	info->Distance = t * sqrt((*K) * (*K));
	info->Material = m_Material;

	switch (j)
	{
	case 0:
		info->Vertical = sp(1, 0, 0);
		break;
	case 1:
		info->Vertical = sp(-1, 0, 0);
		break;
	case 2:
		info->Vertical = sp(0, 1, 0);
		break;
	case 3:
		info->Vertical = sp(0,-1, 0);
		break;
	case 4:
		info->Vertical = sp(0, 0, 1);
		break;
	case 5:
		info->Vertical = sp(0, 0,-1);
		break;
	}
	info->pNode = this;

	return TRUE;
}

void Cube::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;
	matrix m = Matrix * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

	sp p[] = {sp(1,1,1), sp(1,1,-1), sp(1,-1,1), sp(1,-1,-1), sp(-1,1,1), sp(-1,1,-1), sp(-1,-1,1), sp(-1,-1,-1)};

	POINT	P[8];

	for (int i = 0; i < 8; i++ ) {
		P[i] = sp(m * p[i]).getPOINT(size);
	}

	pDC->MoveTo(P[7]);
	pDC->LineTo(P[3]);
	pDC->LineTo(P[1]);
	pDC->LineTo(P[5]);
	pDC->LineTo(P[4]);
	pDC->LineTo(P[0]);
	pDC->LineTo(P[2]);
	pDC->LineTo(P[6]);
	pDC->LineTo(P[7]);
	pDC->LineTo(P[5]);
	pDC->MoveTo(P[4]);
	pDC->LineTo(P[6]);
	pDC->MoveTo(P[2]);
	pDC->LineTo(P[3]);
	pDC->MoveTo(P[0]);
	pDC->LineTo(P[1]);
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Cube::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	matrix m = Matrix * m_Matrix;

	switch (rtv.m_ViewMode) {
	case CRayTraceView::eD3DWireFrame:
		{
			sp q[] = {
				sp(1,1,1), sp(1,1,-1), sp(-1,1,-1), sp(-1,-1,-1), sp(-1,-1,1),
				sp(1,-1,1),	sp(1,-1,-1), sp(1,1,-1), sp(-1,1,-1), sp(-1,1,1), 
				sp(-1,-1,1), sp(-1,-1,-1), sp(1,-1,-1), sp(1,1,-1), sp(1,1,1), 
				sp(1,-1,1),	sp(1,1,1), sp(-1,1,1),
			};

			if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 18))
				return;

			for (int i = 0; i < 18; i++) {
				sp p = m * q[i];
				pVertices[i].position = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
				pVertices[i].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
			}
			pVB->Unlock();
			lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, 17));
		}
		break;
	case CRayTraceView::eD3DFlatShading:
	case CRayTraceView::eD3DGouraudShading:
		LPD3DXMESH pMesh;

		if (FAILED(D3DXCreateBox(pd3dDevice, 1, 1, 1, &pMesh, NULL)))
			return;
		
		lstGeometry.AddTail(Geometry(this, pMesh, m));
		break;
	}

	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

BOOL Cube::IsInside(const sp* L)
{
	return (-1.0 <= L->x && L->x <= 1.0 && -1.0 <= L->y && L->y <= 1.0 && -1.0 <= L->z && L->z <= 1.0);
}

void Cube::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 8, 9, hParent, hInsertAfter), (DWORD)this);
}
