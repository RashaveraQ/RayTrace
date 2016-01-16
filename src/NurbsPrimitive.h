#ifndef __NURBS_PRIMITIVE_H
#define __NURBS_PRIMITIVE_H

class	Node;
struct	matrix;
class   Vertex;

#define CV_MAX_WIDTH 100
#define CV_MAX_HEIGHT 100

class NurbsPrimitive : public Object
{
protected:
	int m_ControlVertexWidth;
	int m_ControlVertexHeight;
	Vertex& getControlVertex(int w, int h) const;
	bool newDeviceNode();
public:
	// コンストラクタ
	NurbsPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int cv_width, int cv_height, const sp Color = sp(255, 255, 255));
	NurbsPrimitive(const NurbsPrimitive& other);
	const Node*	MakeCopy() const { return new NurbsPrimitive(*this); }
	bool OnControlVertex() { return Object::OnVertex(); }
};

#endif
