#include "stdafx.h"
#include "RayTrace.h"
#include "DlgPolygon.h"

IMPLEMENT_SERIAL(Polygon2, CObject, 1)

Polygon2::Polygon2( const Polygon2& other ) : Node( other )
{
	int	i;

	m_N = other.m_N;
	m_P = new sp[m_N];
	for ( i = 0; i < m_N; i++ )
		m_P[i] = other.m_P[i];
}

BOOL Polygon2::IsInside(const sp* L)
{
	if ( L->z < 0.0 )
		return FALSE;

	BOOL	r = FALSE;
	int		i,j;

	for (i = 0, j = m_N-1; i < m_N; j = i++)
	{
		if ( ( 
				( ( m_P[i].y <= L->y ) && ( L->y < m_P[j].y ) )
				||
				( ( m_P[j].y <= L->y ) && ( L->y < m_P[i].y ) )
			 )
			 && 
			 (
				L->x < ( m_P[j].x - m_P[i].x ) * ( L->y - m_P[i].y ) / ( m_P[j].y - m_P[i].y ) + m_P[i].x
			 )
		)
		r = !r;
	}
	return r;
}


void Polygon2::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 23, 22, hParent, hInsertAfter), (DWORD)this);
}

void	Polygon2::Serialize(CArchive& ar)
{
	int		i;

	Node::Serialize(ar);

	if ( ar.IsStoring()) {
		ar << m_N;
		for ( i = 0; i < m_N; i++ )
			ar << m_P[i].x << m_P[i].y << m_P[i].z;
	} else {
		ar >> m_N;
		m_P = new sp[m_N];
		for ( i = 0; i < m_N; i++ )
			ar >> m_P[i].x >> m_P[i].y >> m_P[i].z;
	}
}

BOOL	Polygon2::Edit()
{
	CDlgPolygon	dlg_polygon;

	if ( dlg_polygon.DoModal() != IDOK )
		return FALSE;

	return TRUE;
}