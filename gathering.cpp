#include "stdafx.h"

Gathering::Gathering(const CRayTraceDoc* const pDoc, node_type NodeType, const char* const Name)
	: Node(pDoc, NodeType, Name), m_Member(0)
{
}

Gathering::Gathering(const Gathering& other) : Node(other), m_Member(other.m_Member)
{
	for (short i = 0; i < m_Member; i++)
		m_Node[i] = (class Node*)other.m_Node[i]->MakeCopy();
}

Gathering::~Gathering()
{
	for (short i = 0; i < m_Member; i++)
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
			case SPHERE:	m_Node[i] = new Sphere(m_pDoc); 	break;
			case PLANE:		m_Node[i] = new Plane(m_pDoc);		break;
			case PLUS:		m_Node[i] = new Plus(m_pDoc);		break;
			case MINUS:		m_Node[i] = new Minus(m_pDoc);		break;
			case MULTIPLE:	m_Node[i] = new Multiple(m_pDoc);	break;
			case CONE:		m_Node[i] = new Cone(m_pDoc);		break;
			case CYLINDER:	m_Node[i] = new Cylinder(m_pDoc);	break;
			case TORUS:		m_Node[i] = new Torus(m_pDoc);		break;
			case POLYGON:	m_Node[i] = new Polygon2(m_pDoc);	break;
			case CUBE:		m_Node[i] = new Cube(m_pDoc);		break;
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
	matrix m = Matrix * m_Matrix;
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

BOOL Gathering::Delete(Node* pNode)
{
	short	i, j;

	for (i = 0; i < m_Member; i++) {
		if (m_Node[i] == pNode) {
			delete pNode;
			m_Member--;
			for (j = i; j < m_Member; j++)
				m_Node[j] = m_Node[j+1];
			return TRUE;
		} else if (m_Node[i]->Delete(pNode))
			return TRUE;
	}
	return FALSE;
}

BOOL Gathering::AddNode(CTreeCtrl& c, HTREEITEM hitem, Node* pNode)
{
	if (m_Member >= 1000)
		return FALSE;
	
	cudaError_t err;

	BaseNode* p = pNode->getDeviceData();
	if (cudaSuccess != (err = cudaMemcpy(
			p + sizeof(BaseNode) + sizeof(short) + m_Member * sizeof(BaseNode*),
			&p, sizeof(BaseNode*), cudaMemcpyHostToDevice))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy at Gathering::AddNode()", MB_OK);
	}

	m_Node[m_Member++] = pNode;

	pNode->InsertItem(c, hitem);

	//pNode->updateMatrix(this);
	
	return TRUE;
}

void Gathering::SetDocument(const CRayTraceDoc* const pDoc)
{
	m_pDoc = (CRayTraceDoc*)pDoc;

	for (int i = 0; i < m_Member; i++)
		m_Node[i]->SetDocument(pDoc);
}

void Gathering::updateDeviceData()
{
	cudaError_t err;

	if (m_DeviceData) {
	    if (cudaSuccess != (err = cudaFree(m_DeviceData))) {
			MessageBox(0, cudaGetErrorString(err), "cudaFree at Gathering::updateDeviceData()", MB_OK);
		}
	}

	if (cudaSuccess != (err = cudaMalloc((void**)&m_DeviceData, sizeof(Gathering)))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMalloc at Gathering::updateDeviceData()", MB_OK);
	}

	if (cudaSuccess != (err = cudaMemcpy(m_DeviceData, this, sizeof(Gathering), cudaMemcpyHostToDevice))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy at Gathering::updateDeviceData()", MB_OK);
	}
}