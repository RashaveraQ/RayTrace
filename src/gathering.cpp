#include "stdafx.h"
#include "DoCuda.h"

Gathering::Gathering( const Gathering& other ) : Node( other ), m_Member( other.m_Member )
{
	for ( int i = 0; i < m_Member; i++ )
		m_Node[i] = (class Node*)other.m_Node[i]->MakeCopy();
}

Gathering::~Gathering()
{
	for ( short i = 0; i < m_Member; i++ )
		delete m_Node[i];
}

void	Gathering::Serialize(CArchive& ar)
{
	short	i;
	Node::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_Member;
		for ( i = 0; i < m_Member; i++ )
			m_Node[i]->Serialize(ar);
	}
	else
	{
		WORD	id;
		ar >> m_Member;
		for ( i = 0; i < m_Member; i++ )
		{
			ar >> id;
			switch ( id )
			{
			case SPHERE:	m_Node[i] = new Sphere(m_Root); 	break;
			case PLANE:		m_Node[i] = new Plane(m_Root);		break;
			case PLUS:		m_Node[i] = new Plus(m_Root);		break;
			case MINUS:		m_Node[i] = new Minus(m_Root);		break;
			case MULTIPLE:	m_Node[i] = new Multiple(m_Root);	break;
			case CONE:		m_Node[i] = new Cone(m_Root);		break;
			case CYLINDER:	m_Node[i] = new Cylinder(m_Root);	break;
			case TORUS:		m_Node[i] = new Torus(m_Root);		break;
			case POLYGON:	m_Node[i] = new Polygon2(m_Root);	break;
			case CUBE:		m_Node[i] = new Cube(m_Root);		break;
			}
			m_Node[i]->Serialize(ar);
		}
	}
}

void Gathering::Draw_Outline(CDC* pDC, CRayTraceView& rtv, const matrix& Matrix) const
{
	matrix m = Matrix * m_Matrix;
	
	Node *pNodeBackup = rtv.m_SelectedNode;
	if (pNodeBackup == (Node*)this) {
		for (int i = 0; i < m_Member; i++) {
			rtv.m_SelectedNode = m_Node[i];
			m_Node[i]->Draw_Outline(pDC, rtv, m);
		}
		rtv.m_SelectedNode = pNodeBackup;
	} else {
		for (int i = 0; i < m_Member; i++)
			m_Node[i]->Draw_Outline(pDC, rtv, m);
	}
	Node::Draw_Outline(pDC, rtv, m);
}

void Gathering::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const
{
	matrix m = Matrix * m_Matrix;
	for (int i = 0; i < m_Member; i++)
		m_Node[i]->AddGeometry(pd3dDevice, lstGeometry, rtv, m);
	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

bool Gathering::SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& Matrix) const
{
	matrix m = m_Matrix * Matrix;
	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->SetManipulatorAxis(rtv, point, m))
			return true;
	}
	return false;
}

void Gathering::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	for (short i = 0; i < m_Member; i++)
		m_Node[i]->InsertItem(c, hParent);
	c.Expand(hParent, TVE_EXPAND);
}

bool Gathering::Delete(Node* pNode)
{
	short	i, j;

	for (i = 0; i < m_Member; i++) {
		if (m_Node[i] == pNode) {
			delete pNode;
			m_Member--;
			for (j = i; j < m_Member; j++)
				m_Node[j] = m_Node[j+1];
			return true;
		} else if (m_Node[i]->Delete(pNode))
			return true;
	}
	return false;
}

BOOL Gathering::AddNode(CTreeCtrl& c, HTREEITEM hitem, Node* pNode)
{
	if (m_Member >= 1000)
		return FALSE;

	m_Node[m_Member++] = pNode;

	pNode->InsertItem(c, hitem);

	//pNode->updateMatrix(this);

	pNode->SetParent(this);

	return TRUE;
}

void Gathering::SetRoot(Node* const root)
{
	m_Root = root;
	if (!DoCuda_SetRoot(m_devNode, root->m_devNode)) {
		MessageBox(0, "Failed to DoCuda_SetRoot().", "Error", MB_OK);
	}

	for (int i = 0; i < m_Member; i++)
		m_Node[i]->SetRoot(root);
}
