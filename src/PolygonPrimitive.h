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

	// ▽▽ 暫定 ▽▽
	int m_ControlVertexWidth;
	int m_ControlVertexHeight;
	Vertex& getControlVertex(int w, int h) const;
	// △△ 暫定 △△

public:
	// コンストラクタ
	PolygonPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int cv_width, int cv_height, const sp Color = sp(255, 255, 255));
	PolygonPrimitive(const PolygonPrimitive& other);
	const Node*	MakeCopy() const { return new PolygonPrimitive(*this); }
	bool OnPolygonSelectedVertex() { return Object::OnVertex(); }
};

