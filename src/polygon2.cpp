#include "stdafx.h"
#include "RayTrace.h"
#include "DlgPolygon.h"

IMPLEMENT_SERIAL(Polygon2, CObject, 1)

Boundary Polygon2::sBoundary = Boundary(1);	// 暫定

bool Polygon2::newDeviceNode()
{
	bool newDevPolygon2(DevNode*** out, DevNode** const root, const char* const Name, const D3DMATERIAL9 Material);
	return newDevPolygon2(&m_devNode, m_Root->m_devNode, m_Name, m_Material);
}

Polygon2::Polygon2(Node* const root, const char* const Name, const sp Color)
	: Node(root, POLYGON, Name, Color), m_N(0), m_P(0)
{
	if (!newDeviceNode())
		exit(1);
}

Polygon2::Polygon2(const Polygon2& other) : Node(other)
{
	int	i;

	m_N = other.m_N;
	m_P = new sp[m_N];
	for (i = 0; i < m_N; i++)
		m_P[i] = other.m_P[i];

	if (!newDeviceNode())
		exit(1);
}

bool Polygon2::IsInside(const sp& L) const
{
	if (L.z < 0.0)
		return false;

	bool	r = false;
	int		i, j;

	for (i = 0, j = m_N - 1; i < m_N; j = i++)
	{
		if ((
			((m_P[i].y <= L.y) && (L.y < m_P[j].y))
			||
			((m_P[j].y <= L.y) && (L.y < m_P[i].y))
			)
			&&
			(
			L.x < (m_P[j].x - m_P[i].x) * (L.y - m_P[i].y) / (m_P[j].y - m_P[i].y) + m_P[i].x
			)
			)
			r = !r;
	}
	return r;
}

bool Polygon2::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
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
	if (t1 > 0)
	{
		if (t2 > 0)
		{
			info.isEnter = 1;
			t = (t1 < t2) ? t1 : t2;
		}
		else
			t = t1;
	}
	else
	{
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	info.Cross = info.Vertical = K * t + L;
	info.Distance = t * sqrt(K * K);
	info.Material = m_Material;
	info.pNode = this;

	return TRUE;
}

void Polygon2::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 23, 22, hParent, hInsertAfter), (DWORD)this);
}

void	Polygon2::Serialize(CArchive& ar)
{
	int		i;

	Node::Serialize(ar);

	if (ar.IsStoring()) {
		ar << m_N;
		for (i = 0; i < m_N; i++)
			ar << m_P[i].x << m_P[i].y << m_P[i].z;
	}
	else {
		ar >> m_N;
		m_P = new sp[m_N];
		for (i = 0; i < m_N; i++)
			ar >> m_P[i].x >> m_P[i].y >> m_P[i].z;
	}
}

BOOL	Polygon2::Edit()
{
	CDlgPolygon	dlg_polygon;

	if (dlg_polygon.DoModal() != IDOK)
		return FALSE;

	return TRUE;
}