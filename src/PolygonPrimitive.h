#pragma once

class	Node;
struct	matrix;
class	Vertex;
class	Face;

class PolygonPrimitive : public Object
{
protected:
	int		m_NumberOfFaces;
	Face**	m_pFaces;
	bool newDeviceNode();

public:
	// コンストラクタ
	PolygonPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int num_of_vertexes, const sp Color = sp(255, 255, 255));
	PolygonPrimitive(const PolygonPrimitive& other);
	virtual ~PolygonPrimitive();
	const Node*	MakeCopy() const { return new PolygonPrimitive(*this); }
	bool OnPolygonSelectedVertex() { return Object::OnVertex(); }
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
	bool IsInside(const sp& L) const;
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
};

