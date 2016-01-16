#include "stdafx.h"
#include <float.h>

Boundary Object::sBoundary = Boundary(FLT_MAX); // 

Object::Object(Node* const root, node_type NodeType, const TCHAR* const Name, int num_of_vertexes, const sp Color)
	: Node(root, NodeType, Name, Color), m_pVertexes(0), m_NumberOfVertexes(num_of_vertexes), m_IsVertexEditable(false)
{
	init(num_of_vertexes);
}

Object::Object(const Object& other)
	: Node(other), m_pVertexes(0), m_NumberOfVertexes(other.m_NumberOfVertexes), m_IsVertexEditable(false)
{
	init(m_NumberOfVertexes);
}

void Object::init(int num)
{
	if (m_pVertexes)
		delete m_pVertexes;
	m_NumberOfVertexes = num;
	m_pVertexes = new Vertex[num];
}

Object::~Object()
{
	delete [] m_pVertexes;
}

bool Object::OnVertex()
{
	if (m_IsVertexEditable)
		return false;
	m_IsVertexEditable = true;
	m_Selected = false;
	return true;
}

bool Object::OnObjectMode()
{
	if (!m_IsVertexEditable)
		return false;
	m_IsVertexEditable = false;
	return true;
}


bool Object::IsInside(const sp& L) const
{
	return ( L.y >= 0.0 );
}

bool Object::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (m_IsVertexEditable) {
		for (int i = 0; i < m_NumberOfVertexes; i++)
			if (m_pVertexes[i].GetInfo3(K, L, info, NULL, true))
				return true;
	}

	if (pHint && pHint->pNode == this && fromOutSide)
		return false;

	float	t = (K.y) ? -L.y / K.y : ((L.y> 0) ? FLT_MAX : -FLT_MAX);

	if (t <= 0)
		return false;

	info.Cross = K * t + L;
	info.Vertical = sp(0, 1, 0);
	info.Distance = t * sqrt(K * K);
	info.isEnter = (L.y < 0);
	info.Material = GetPixel(info.Cross.x, info.Cross.z).getMaterial();
	info.pNode = this;

	return -3 < info.Cross.x && info.Cross.x < 3 && -3 < info.Cross.z && info.Cross.z < 3;
}

void Object::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	if (m_IsVertexEditable) {
		for (int i = 0; i < m_NumberOfVertexes; i++)
			m_pVertexes[i].Draw_Outline(pDC, raytraceview, mat);
	}
	Node::Draw_Outline(pDC, raytraceview, mat);
}

bool Object::getManipulatorMatrix(matrix& mat) const
{
	if (!m_IsVertexEditable)
		return Node::getManipulatorMatrix(mat);

	bool ans = false;
	matrix m1;
	int c = 0;
	for (int i = 0; i < m_NumberOfVertexes; i++) {
		matrix m2;
		if (m_pVertexes[i].getManipulatorMatrix(m2)) {
			m1 = m1 * m2;
			c++;
			ans = true;
		}
	}
	if (ans) {
		m1 = expand(1.0f / c) * m1;
		mat = mat * m1;
	}
	return ans;
}

void Object::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD_PTR)this);
}

void Object::Serialize(CArchive& ar)
{
	Node::Serialize(ar);

	if (ar.IsStoring()) {
		ar << m_NumberOfVertexes;
		for (int i = 0; i < m_NumberOfVertexes; i++)
			m_pVertexes[i].Serialize(ar);
	} else {
		int n;
		ar >> n;
		init(n);
		for (int i = 0; i < m_NumberOfVertexes; i++)
			m_pVertexes[i].Serialize(ar);
	}
}

bool Object::ChangeSelection(const CRect* pRect, int cx, int cy, const matrix& mat)
{
	matrix m = mat * m_Matrix;
	if (m_IsVertexEditable) {
		bool ans = false;
		for (int i = 0; i < m_NumberOfVertexes; i++)
			ans |= m_pVertexes[i].ChangeSelection(pRect, cx, cy, m);
		return ans;
	}
	return Node::ChangeSelection(pRect, cx, cy, mat);
}

bool Object::ResetSelection()
{
	bool ans = false;
	for (int i = 0; i < m_NumberOfVertexes; i++)
		ans |= m_pVertexes[i].ResetSelection();
	ans |= Node::ResetSelection();
	return ans;
}

void Object::Move(eAxis axis, float d)
{
	if (m_IsVertexEditable) {
		for (int i = 0; i < m_NumberOfVertexes; i++)
			m_pVertexes[i].Move(axis, d);
	} else {
		Node::Move(axis, d);
	}
}

void Object::Rotate(const sp& o, eAxis axis, float d)
{
	if (m_IsVertexEditable) {
		for (int i = 0; i < m_NumberOfVertexes; i++)
			m_pVertexes[i].Rotate(o, axis, d);
	} else {
		Node::Rotate(o, axis, d);
	}
}

void Object::Scale(const sp& o, eAxis axis, float d)
{
	if (m_IsVertexEditable) {
		for (int i = 0; i < m_NumberOfVertexes; i++)
			m_pVertexes[i].Scale(o, axis, d);
	} else {
		Node::Scale(o, axis, d);
	}
}

void Object::MovePivot(eAxis axis, float d)
{
	if (m_IsVertexEditable) {
		for (int i = 0; i < m_NumberOfVertexes; i++)
			m_pVertexes[i].MovePivot(axis, d);
	} else {
		Node::MovePivot(axis, d);
	}
}
