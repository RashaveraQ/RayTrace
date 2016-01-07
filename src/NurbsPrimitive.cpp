#include "stdafx.h"
#include <float.h>

Boundary NurbsPrimitive::sBoundary = Boundary(FLT_MAX); // 

bool NurbsPrimitive::newDeviceNode()
{
	bool newDevNurbsPrimitive(DevNode*** out, DevNode** const root, const D3DMATERIAL9 Material);
	return newDevNurbsPrimitive(&m_devNode, m_Root ? m_Root->m_devNode : 0, m_Material);
}

NurbsPrimitive::NurbsPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int cv_width, int cv_height, const sp Color)
	: Node(root, NodeType, Name, Color), m_ControlVertex(0), m_IsControlVertexEditable(false)
{
	init(cv_width, cv_height);
}

NurbsPrimitive::NurbsPrimitive(const NurbsPrimitive& other)
	: Node(other), m_ControlVertex(0), m_IsControlVertexEditable(false)
{
	init(other.m_ControlVertexWidth, other.m_ControlVertexHeight);
}

void NurbsPrimitive::init(int w, int h)
{
	if (m_ControlVertex) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			delete[] m_ControlVertex[i];
		delete m_ControlVertex;
	}
	m_ControlVertexWidth = w;
	m_ControlVertexHeight = h;
	m_ControlVertex = new Point*[m_ControlVertexWidth];
	for (int i = 0; i < m_ControlVertexWidth; i++)
		m_ControlVertex[i] = new Point[m_ControlVertexHeight];
}

NurbsPrimitive::~NurbsPrimitive()
{
	for (int i = 0; i < m_ControlVertexWidth; i++)
		delete[] m_ControlVertex[i];
	delete m_ControlVertex;
}

bool NurbsPrimitive::OnControlVertex()
{
	if (m_IsControlVertexEditable)
		return false;
	m_IsControlVertexEditable = true;
	m_Selected = false;
	return true;
}

bool NurbsPrimitive::OnObjectMode()
{
	if (!m_IsControlVertexEditable)
		return false;
	m_IsControlVertexEditable = false;
	return true;
}

bool NurbsPrimitive::IsInside(const sp& L) const
{
	return ( L.y >= 0.0 );
}

bool NurbsPrimitive::GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const
{
	if (m_IsControlVertexEditable) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				if (m_ControlVertex[i][j].GetInfo3(K, L, info, NULL, true))
					return true;
		return false;
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

void NurbsPrimitive::Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const
{
	if (m_IsControlVertexEditable) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].Draw_Outline(pDC, raytraceview, mat);
	}
	Node::Draw_Outline(pDC, raytraceview, mat);
}

bool NurbsPrimitive::getManipulatorMatrix(matrix& mat) const
{
	if (!m_IsControlVertexEditable)
		return Node::getManipulatorMatrix(mat);

	bool ans = false;
	matrix m1;
	int c = 0;
	for (int i = 0; i < m_ControlVertexWidth; i++) {
		for (int j = 0; j < m_ControlVertexHeight; j++) {
			matrix m2;
			if (m_ControlVertex[i][j].getManipulatorMatrix(m2)) {
				m1 = m1 * m2;
				c++;
				ans = true;
			}
		}
	}
	if (ans) {
		m1 = expand(1.0f / c) * m1;
		mat = mat * m1;
	}
	return ans;
}

void NurbsPrimitive::InsertItem(CTreeCtrl& c, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	c.SetItemData(c.InsertItem(m_Name, 13, 12, hParent, hInsertAfter), (DWORD_PTR)this);
}

void NurbsPrimitive::Serialize(CArchive& ar)
{
	Node::Serialize(ar);

	if (ar.IsStoring()) {
		ar << m_ControlVertexWidth;
		ar << m_ControlVertexHeight;
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].Serialize(ar);
	} else {
		int w, h;
		ar >> w;
		ar >> h;
		init(w,h);
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].Serialize(ar);
	}
}

bool NurbsPrimitive::ChangeSelection(const CRect* pRect, int cx, int cy, const matrix& mat)
{
	matrix m = mat * m_Matrix;
	if (m_IsControlVertexEditable) {
		bool ans = false;
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				ans |= m_ControlVertex[i][j].ChangeSelection(pRect, cx, cy, m);
		return ans;
	}
	return Node::ChangeSelection(pRect, cx, cy, mat);
}

bool NurbsPrimitive::ResetSelection()
{
	bool ans = false;
	for (int i = 0; i < m_ControlVertexWidth; i++)
		for (int j = 0; j < m_ControlVertexHeight; j++)
			ans |= m_ControlVertex[i][j].ResetSelection();
	ans |= Node::ResetSelection();
	return ans;
}

void NurbsPrimitive::Move(eAxis axis, float d)
{
	if (m_IsControlVertexEditable) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].Move(axis, d);
	} else {
		Node::Move(axis, d);
	}
}

void NurbsPrimitive::Rotate(eAxis axis, float d)
{
	if (m_IsControlVertexEditable) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].Rotate(axis, d);
	}
	else {
		Node::Rotate(axis, d);
	}
}

void NurbsPrimitive::Scale(eAxis axis, float d)
{
	if (m_IsControlVertexEditable) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].Scale(axis, d);
	}
	else {
		Node::Scale(axis, d);
	}
}

void NurbsPrimitive::MovePivot(eAxis axis, float d)
{
	if (m_IsControlVertexEditable) {
		for (int i = 0; i < m_ControlVertexWidth; i++)
			for (int j = 0; j < m_ControlVertexHeight; j++)
				m_ControlVertex[i][j].MovePivot(axis, d);
	}
	else {
		Node::MovePivot(axis, d);
	}
}
