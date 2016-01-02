#include "stdafx.h"

IMPLEMENT_SERIAL(Cube, CObject, 1)

Boundary Cube::sBoundary = Boundary(sqrt(3.0f));

bool Cube::newDeviceNode()
{
	bool newDevCube(DevNode***, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevCube(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

Cube::Cube(Node* const root, const TCHAR* const Name, const sp Color)
	: Node(root, eCUBE, Name, Color)
{
	if (!newDeviceNode())
		exit(1);
}

Cube::Cube(const Cube& other)
	: Node(other)
{
	if (!newDeviceNode())
		exit(1);
}

bool Cube::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	float t[6];
	sp	   v[6];
	int i = 0;
	
	info.isEnter = (pHint && pHint->pNode == this) ? fromOutSide : !IsInside(L);

	t[i] = (-1 - L.x) / K.x;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(-1,0,0);
			i++;
		}
	}

	t[i] = (1 - L.x) / K.x;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(1, 0, 0);
			i++;
		}
	}

	t[i] = (-1 - L.y) / K.y;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.x && p.x <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(0, -1, 0);
			i++;
		}
	}

	t[i] = (1 - L.y) / K.y;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.x && p.x <= 1 && -1 <= p.z && p.z <= 1) {
			v[i] = sp(0, 1, 0);
			i++;
		}
	}

	t[i] = (-1 - L.z) / K.z;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.x && p.x <= 1) {
			v[i] = sp(0, 0, -1);
			i++;
		}
	}

	t[i] = (1 - L.z) / K.z;
	if (t[i] > 0) {
		sp p = K * t[i] + L;
		if (-1 <= p.y && p.y <= 1 && -1 <= p.x && p.x <= 1) {
			v[i] = sp(0, 0, 1);
			i++;
		}
	}

	switch (i) {
	case 0:
		return FALSE;
	case 1:
		break;
	default:
		if (pHint && pHint->pNode == this) {
			if (t[0] < t[1]) {
				t[0] = t[1];
				v[0] = v[1];
			}
		} else if (t[0] > t[1]) {
			t[0] = t[1];
			v[0] = v[1];
		}
		break;
	}

	info.Cross = K * t[0] + L;
	info.Vertical = info.isEnter ? v[0] : -v[0];
	info.Distance = t[0] * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;
	return TRUE;
}

void Cube::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;
	matrix m = mat * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

	sp p[] = {sp(1,1,1), sp(1,1,-1), sp(1,-1,1), sp(1,-1,-1), sp(-1,1,1), sp(-1,1,-1), sp(-1,-1,1), sp(-1,-1,-1)};

	POINT	P[8];

	for (int i = 0; i < 8; i++ ) {
		sp(m * p[i]).getPOINT(P[i].x, P[i].y, size.cx, size.cy);
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

void Cube::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& mat) const
{
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	matrix m = mat * m_Matrix;

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

		if (FAILED(D3DXCreateBox(pd3dDevice, 2, 2, 2, &pMesh, NULL)))
			return;
		
		lstGeometry.AddTail(Geometry(this, pMesh, m));
		break;
	}

	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

bool Cube::IsInside(const sp& L) const
{
	return (-1.0 <= L.x && L.x <= 1.0 && -1.0 <= L.y && L.y <= 1.0 && -1.0 <= L.z && L.z <= 1.0);
}

void Cube::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 8, 9, hParent, hInsertAfter), (DWORD_PTR)this);
}
