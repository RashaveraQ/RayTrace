#pragma once

class	Node;
struct	matrix;
class	Vertex;
class	Face;

class PolygonPrimitive : public Object
{
protected:
	Face** pFaces;
	bool newDeviceNode();
public:
	// コンストラクタ
	PolygonPrimitive(Node* const root, node_type NodeType, const TCHAR* const Name, int num_of_vertexes, Vertex** pVertexes, const sp Color = sp(255, 255, 255));
	PolygonPrimitive(const PolygonPrimitive& other);
	const Node*	MakeCopy() const { return new PolygonPrimitive(*this); }
};

