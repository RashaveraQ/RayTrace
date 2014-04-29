#include "stdafx.h"

IMPLEMENT_SERIAL(Cube, CObject, 1)

BOOL Cube::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint) const
{
	double t;

	info.isEnter = !IsInside(L);

	for(;;) {
		if (K.x > 0 && L.x < -1) {
			t = (-1 - L.x) / K.x;
			sp p = K * t + L;
			if (-1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1) {
				info.Vertical = sp(-1,0,0);
				break;
			}
		}
		
		if(K.x < 0 && L.x > 1) {
			t = (1 - L.x) / K.x;
			sp p = K * t + L;
			if (-1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1) {
				info.Vertical = sp(1,0,0);
				break;
			}
		}

		if (K.y > 0 && L.y < -1) {
			t = (-1 - L.y) / K.y;
			sp p = K * t + L;
			if (-1 <= p.x && p.x <= 1 && -1 <= p.z && p.z <= 1) {
				info.Vertical = sp(0,-1,0);
				break;
			}
		}
		
		if(K.y < 0 && L.y > 1) {
			t = (1 - L.y) / K.y;
			sp p = K * t + L;
			if (-1 <= p.x && p.x <= 1 && -1 <= p.z && p.z <= 1) {
				info.Vertical = sp(0,1,0);
				break;
			}
		}

		if (K.z > 0 && L.z < -1) {
			t = (-1 - L.z) / K.z;
			sp p = K * t + L;
			if (-1 <= p.y && p.y <= 1 && -1 <= p.x && p.x <= 1) {
				info.Vertical = sp(0,0,-1);
				break;
			}
		}
		
		if(K.z < 0 && L.z > 1) {
			t = (1 - L.z) / K.z;
			sp p = K * t + L;
			if (-1 <= p.y && p.y <= 1 && -1 <= p.x && p.x <= 1) {
				info.Vertical = sp(0,0,1);
				break;
			}
		}

		return FALSE;
	}

	info.Cross = K * t + L;
	info.Distance = t * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;
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

BOOL Cube::IsInside(const sp& L) const
{
	return (-1.0 <= L.x && L.x <= 1.0 && -1.0 <= L.y && L.y <= 1.0 && -1.0 <= L.z && L.z <= 1.0);
}

void Cube::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 8, 9, hParent, hInsertAfter), (DWORD)this);
}
