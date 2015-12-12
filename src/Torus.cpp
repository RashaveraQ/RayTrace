#include "stdafx.h"
#include "RayTrace.h"
#include "DlgTorus.h"

IMPLEMENT_SERIAL(Torus, CObject, 1)

#define A 10
#define B 50

Boundary Torus::sBoundary = Boundary(1);

Torus::Torus(Node* const root, const char* const Name, const sp Color)
	: Node(root, TORUS, Name, Color), m_R(0.7f), m_r(0.3f)
{

}

bool Torus::newDeviceNode(DevNode** ppDevNode)
{
	bool newDevTorus(DevNode**);
	return newDevTorus(ppDevNode);
}

void Torus::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const
{
	const CSize& size = raytraceview.m_ClientSize;
	const Node* pNode = raytraceview.m_SelectedNode;

	matrix m = Matrix * m_Matrix;
	pDC->SelectStockObject((pNode == this) ? WHITE_PEN : BLACK_PEN);

	POINT	P[B];

	int i, j;
	float th, ph;

	for (i = 0; i < A; i++) {
		for (j = 0; j < B; j++) {
			th = 6.28f * i / A;
			ph = 6.28f * j / B;
			sp p = m * sp((m_R + m_r * cosf(ph)) * cosf(th), (m_R + m_r * cosf(ph)) * sinf(th), m_r * sinf(ph));
			P[j] = p.getPOINT(size);
		}
		pDC->Polygon(P, B);
	}

	for (i = 0; i < A; i++) {
		for (j = 0; j < B; j++) {
			ph = 6.28f * i / A;
			th = 6.28f * j / B;
			sp p = m * sp((m_R + m_r * cosf(ph)) * cosf(th), (m_R + m_r * cosf(ph)) * sinf(th), m_r * sinf(ph));
			P[j] = p.getPOINT(size);
		}
		pDC->Polygon(P, B);
	}
	Node::Draw_Outline(pDC, raytraceview, m);
}

void Torus::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	// D3DXCreateTorus の使用を検討すること
	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	matrix m = Matrix * m_Matrix;

	switch (rtv.m_ViewMode) {
	case CRayTraceView::eD3DWireFrame:
		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 2 * A*B))
			return;


		int i, j;
		float th, ph;

		for (i = 0; i < A; i++) {
			for (j = 0; j < B; j++) {
				th = 6.28f * i / A;
				ph = 6.28f * j / B;
				sp p = m * sp((m_R + m_r * cosf(ph)) * cosf(th), (m_R + m_r * cosf(ph)) * sinf(th), m_r * sinf(ph));
				pVertices[B*i + j].position = D3DXVECTOR3(p.x, p.y, p.z);
				pVertices[B*i + j].normal = D3DXVECTOR3(p.x, p.y, p.z);
			}
		}

		for (i = 0; i < A; i++) {
			for (j = 0; j < B; j++) {
				ph = 6.28f * i / A;
				th = 6.28f * j / B;
				sp p = m * sp((m_R + m_r * cosf(ph)) * cosf(th), (m_R + m_r * cosf(ph)) * sinf(th), m_r * sinf(ph));
				pVertices[A*B + B*i + j].position = D3DXVECTOR3(p.x, p.y, p.z);
				pVertices[A*B + B*i + j].normal = D3DXVECTOR3(p.x, p.y, p.z);
			}
		}
		pVB->Unlock();

		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, 2 * A*B - 1));
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

bool Torus::IsInside(const sp& L) const
{
	float	d;

	d = m_R - sqrt(L.x * L.x + L.y * L.y);
	d *= d;

	return (m_r * m_r <= d + L.z * L.z);
}

bool Torus::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	int Solve_Polynomial(int d, float *k, float min, float max, float *r);

	float	R2 = m_R * m_R;
	float	a = K * K;
	float	b = 2 * (K * L);
	float	c = L * L + R2 - m_r * m_r;

	float	k[20], r[50];
	int		n;

	k[4] = a * a;
	k[3] = 2 * a * b;
	k[2] = 2 * a * c + b * b - 4 * R2 * (K.x * K.x + K.y * K.y);
	k[1] = 2 * b * c - 8 * R2 * (K.x * L.x + K.y * L.y);
	k[0] = c * c - 4 * R2 * (L.x * L.x + L.y * L.y);

	n = Solve_Polynomial(4, k, 0, 20000, r);

	if (n == 0)
		return FALSE;

	if (pHint && pHint->pNode == this) {
		if (fromOutSide && n % 2 == 1) {
			if (n == 1)
				return FALSE;
			r[0] = r[1];
		}
		if (!fromOutSide && n % 2 == 0) {
			r[0] = r[1];
		}
	}

	sp		p;
	float	th;

	info.isEnter = IsInside(L) == TRUE ? 1 : 0;
	info.Cross = p = K * r[0] + L;

	if (p.x == 0.0) {
		info.Vertical = sp(0, p.y - ((p.y > 0) ? 1 : -1) * m_R, p.z);
	}
	else {
		th = atan2(p.y, p.x);
		info.Vertical = sp(p.x - m_R * cos(th), p.y - m_R * sin(th), p.z);
	}

	info.Distance = r[0] * sqrt(K * K);

	float x, y, z, phy;

	x = info.Vertical.x;
	y = info.Vertical.y;
	z = info.Vertical.z;

	th = acosf(y) / (2 * M_PI); if (x < 0) th = 1 - th;
	phy = acosf(z / sqrtf(x * x + z * z)) / (2 * M_PI);
	if (x < 0)
		phy = 1 - phy;

	info.Material = GetPixel(phy, th).getMaterial();
	info.pNode = this;

	return TRUE;
}

void Torus::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 21, 20, hParent, hInsertAfter), (DWORD)this);
}

void Torus::Serialize(CArchive& ar)
{
	Node::Serialize(ar);

	if (ar.IsStoring()) {
		ar << m_r;
		ar << m_R;
	}
	else {
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