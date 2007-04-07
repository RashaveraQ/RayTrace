#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

class Node;

struct Geometry
{
	enum {
		eBasic,
		eMesh
	} m_Type;

	union {
		struct {
			LPDIRECT3DVERTEXBUFFER9	pD3dVertexBuffer;
			UINT					primitiveCount;
			D3DPRIMITIVETYPE		primitiveType;
		};
		struct {
			LPD3DXMESH				pd3dXMesh;
			LPD3DXMATRIXA16			pmatWorld;
		};
	} m_Data;

	const Node *m_pNode; 

	Geometry() {}
	Geometry(const Node *pNode, LPDIRECT3DVERTEXBUFFER9 pVB, D3DPRIMITIVETYPE pt, UINT pc);
	Geometry(const Node *pNode, LPD3DXMESH pd3dxmesh, const struct matrix& world);

	void Clear();
	bool Draw(LPDIRECT3DDEVICE9 pd3dDevice);
};

#endif // __GEOMETRY_H__