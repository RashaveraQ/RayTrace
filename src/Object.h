#pragma once

class	Node;
struct	matrix;
class   Vertex;

class Object : public Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
	void init(int num);
protected:
	int m_NumberOfVertexes;
	Vertex* m_pVertexes;
	bool m_IsVertexEditable;
public:
	// コンストラクタ
	Object(Node* const root, node_type NodeType, const TCHAR* const Name, int num_of_vertexes, const sp Color = sp(255, 255, 255));
	Object(const Object& other);
	virtual ~Object();
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& matrix) const {}
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool OnVertex();
	bool OnObjectMode();
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	void Serialize(CArchive& ar);
	bool ChangeSelection(const CRect* pRect, int cx, int cy, const matrix& mat);
	bool ResetSelection();
	bool getManipulatorMatrix(matrix& mat) const;
	void Move(eAxis axis, float d);
	void Rotate(const sp& o, eAxis axis, float d);
	void Scale(const sp& o, eAxis axis, float d);
	void MovePivot(eAxis axis, float d);
};