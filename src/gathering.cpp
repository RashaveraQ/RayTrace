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
			case eSPHERE:		m_Node[i] = new Sphere(m_Root); 	break;
			case ePLANE:		m_Node[i] = new Plane(m_Root);		break;
			case ePLUS:			m_Node[i] = new Plus(m_Root);		break;
			case eMINUS:		m_Node[i] = new Minus(m_Root);		break;
			case eMULTIPLE:		m_Node[i] = new Multiple(m_Root);	break;
			case eCONE:			m_Node[i] = new Cone(m_Root);		break;
			case eCYLINDER:		m_Node[i] = new Cylinder(m_Root);	break;
			case eTORUS:		m_Node[i] = new Torus(m_Root);		break;
			case ePOLYGON:		m_Node[i] = new Polygon2(m_Root);	break;
			case eCUBE:			m_Node[i] = new Cube(m_Root);		break;
			case eNurbsPlane:	m_Node[i] = new NurbsPlane(m_Root); break;
			}
			m_Node[i]->Serialize(ar);

			if (!DoCuda_AddNode((DevGathering**)m_devNode, m_Node[i]->m_devNode)) {
				MessageBox(0, _T("Failed to DoCuda_AddNode"), _T("Error"), MB_OK);
			}
		}
	}
}

void Gathering::Draw_Outline(CDC* pDC, CRayTraceView& rtv, const matrix& mat) const
{
	matrix m = mat * m_Matrix;
	for (int i = 0; i < m_Member; i++)
		m_Node[i]->Draw_Outline(pDC, rtv, m);
	Node::Draw_Outline(pDC, rtv, m);
}

bool Gathering::getManipulatorMatrix(matrix& mat) const
{
	matrix m1;
	bool ans = false;
	int c = 0;
	for (int i = 0; i < m_Member; i++) {
		matrix m2;
		if (m_Node[i]->getManipulatorMatrix(m2)) {
			m1 = m1 * m2;
			c++;
			ans = true;
		}
	}

	if (ans) {
		m1 = m1 * expand(1.0f / c);
		mat = mat * m_Matrix * m1;
	}
	return ans;
}

void Gathering::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& mat) const
{
	matrix m = mat * m_Matrix;
	for (int i = 0; i < m_Member; i++)
		m_Node[i]->AddGeometry(pd3dDevice, lstGeometry, rtv, m);
	Node::AddGeometry(pd3dDevice, lstGeometry, rtv, m);
}

bool Gathering::SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& mat) const
{
	matrix m = m_Matrix * mat;
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
			if (!DoCuda_DeleteNode((DevGathering**)m_devNode, pNode->m_devNode)) {
				MessageBox(0, _T("Failed to DoCuda_DeleteNode"), _T("Error"), MB_OK);
			}
			pNode->m_devNode = 0;
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

	if (!DoCuda_AddNode((DevGathering**)m_devNode, pNode->m_devNode)) {
		MessageBox(0, _T("Failed to DoCuda_AddNode"), _T("Error"), MB_OK);
	}

	return TRUE;
}

void Gathering::SetRoot(Node* const root)
{
	m_Root = root;
	if (!DoCuda_SetRoot(m_devNode, root->m_devNode)) {
		MessageBox(0, _T("Failed to DoCuda_SetRoot()."), _T("Error"), MB_OK);
	}

	for (int i = 0; i < m_Member; i++)
		m_Node[i]->SetRoot(root);
}

bool Gathering::ChangeSelection(const CRect* pRect, int cx, int cy, const matrix& mat)
{
	matrix m = mat * m_Matrix;
	bool ans = false;
	for (int i = 0; i < m_Member; i++)
		ans |= m_Node[i]->ChangeSelection(pRect, cx, cy, m);
	return ans;
}

bool Gathering::ResetSelection()
{
	bool ans = false;
	for (int i = 0; i < m_Member; i++)
		ans |= m_Node[i]->ResetSelection();
	return ans;
}
