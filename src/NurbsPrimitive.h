#ifndef __NURBS_PRIMITIVE_H
#define __NURBS_PRIMITIVE_H

class	Node;
struct	matrix;
class   Vertex;

#define CV_MAX_WIDTH 100
#define CV_MAX_HEIGHT 100

class NurbsPrimitive : public Node
{
	static Boundary sBoundary;
	Boundary getBoundary() { return sBoundary; }
	void init(int w, int h);
protected:
	bool m_IsControlVertexEditable;
	int m_ControlVertexWidth;
	int m_ControlVertexHeight;
	Vertex** m_ControlVertex;
	virtual bool newDeviceNode();
public:
	// コンストラクタ
	NurbsPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int cv_width, int cv_height, const sp Color = sp(255, 255, 255));
	NurbsPrimitive(const NurbsPrimitive& other);
	virtual ~NurbsPrimitive();
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& matrix) const {}
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
	bool IsInside(const sp& L) const;
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
	void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	const Node*	MakeCopy() const { return new NurbsPrimitive(*this); }
	bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	void Serialize(CArchive& ar);
	bool OnControlVertex();
	bool OnObjectMode();
	bool ChangeSelection(const CRect* pRect, int cx, int cy, const matrix& mat);
	bool ResetSelection();
	bool getManipulatorMatrix(matrix& mat) const;
	void Move(eAxis axis, float d);
	void Rotate(const sp& o, eAxis axis, float d);
	void Scale(const sp& o, eAxis axis, float d);
	void MovePivot(eAxis axis, float d);
};

#endif
