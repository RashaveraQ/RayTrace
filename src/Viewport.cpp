// Viewport.cpp: Viewport クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "raytrace.h"
#include "Viewport.h"
#include "DoCuda.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Viewport::Viewport()
{
	Rotate(eZ, 10);
}

Viewport::~Viewport()
{
}

void Viewport::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;

	static CPen	gray_pen(PS_SOLID, 1, RGB(100, 100, 100));

	CPen *old_pen = pDC->SelectObject(&gray_pen);

	matrix m = Matrix * m_Matrix;

	for (int i = 0; i < 2; i++) {
		for (float x = -12; x <= 12; x++) {

			POINT	P1 = sp(m * (i ? sp(x, 0, -12) : sp(-12, 0, x))).getPOINT(size),
				P2 = sp(m * (i ? sp(x, 0, 12) : sp(12, 0, x))).getPOINT(size);

			if (x == 0)
				pDC->SelectStockObject(BLACK_PEN);

			pDC->MoveTo(P1);
			pDC->LineTo(P2);

			if (x == 0)
				pDC->SelectObject((class CPen*)&gray_pen);
		}
	}

	Plus::Draw_Outline(pDC, raytraceview, Matrix);

	pDC->SelectObject(old_pen);
}

void Viewport::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	InitVertexBuffer(pd3dDevice, pVB, pVertices, 100);

	matrix m = Matrix * m_Matrix;

	int j = 0;
	for (int i = 0; i < 2; i++) {
		for (float x = -12; x <= 12; x++) {
			sp p1 = m * (i ? sp(x, 0, -12) : sp(-12, 0, x));
			sp p2 = m * (i ? sp(x, 0, 12) : sp(12, 0, x));
			pVertices[j].position = D3DXVECTOR3((float)p1.x, (float)p1.y, (float)p1.z);
			pVertices[j].normal = D3DXVECTOR3(0, 1, 0);
			pVertices[++j].position = D3DXVECTOR3((float)p2.x, (float)p2.y, (float)p2.z);
			pVertices[j++].normal = D3DXVECTOR3(0, 1, 0);
		}
	}
	pVB->Unlock();
	lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINELIST, 50));

	Plus::AddGeometry(pd3dDevice, lstGeometry, rtv, Matrix);
}

void Viewport::AttachRoot(const Node* pRoot)
{
	m_Node[m_Member++] = (class Node*)pRoot;
	if (!DoCuda_AttachRoot((DevPlus**)m_devNode, pRoot->m_devNode)) {
		MessageBox(0, "Failed to DoCuda_AttachRoot", "Error", MB_OK);
	}
}

void Viewport::DetachRoot()
{
	m_Member = 0;
}

BOOL Viewport::AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target)
{
	return FALSE;
}