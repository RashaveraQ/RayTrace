#include "stdafx.h"

Geometry::Geometry(const Node *pNode, LPDIRECT3DVERTEXBUFFER9 pVB, D3DPRIMITIVETYPE pt, UINT pc)
 : m_pNode(pNode), m_Type(eBasic)
{
	m_Data.pD3dVertexBuffer = pVB;
	m_Data.primitiveType = pt;
	m_Data.primitiveCount = pc;
}

Geometry::Geometry(const Node *pNode, LPD3DXMESH pd3dxmesh, const struct matrix& world)
 : m_pNode(pNode), m_Type(eMesh)
{
	m_Data.pd3dXMesh = pd3dxmesh;
	m_Data.pmatWorld = new D3DXMATRIXA16;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_Data.pmatWorld->m[i][j] = (float)world.get_data(j + 1, i + 1);
}

bool Geometry::Draw(LPDIRECT3DDEVICE9 pd3dDevice)
{
	bool ret;
	pd3dDevice->SetMaterial(&m_pNode->m_Material);

	switch (m_Type) {
	case eBasic:
		ret = D3D_OK == pd3dDevice->SetStreamSource(0, m_Data.pD3dVertexBuffer, 0, sizeof(CUSTOMVERTEX))
		&& D3D_OK == pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX)
		&& D3D_OK == pd3dDevice->DrawPrimitive(m_Data.primitiveType, 0, m_Data.primitiveCount);
		break;
	case eMesh:
		ret = D3D_OK == pd3dDevice->SetTransform(D3DTS_WORLD, m_Data.pmatWorld)
		&& D3D_OK == m_Data.pd3dXMesh->DrawSubset(0);
		break;
	}

	return ret;
}

void Geometry::Clear()
{
	switch (m_Type) {
	case eBasic:
		m_Data.pD3dVertexBuffer->Release();
		break;
	case eMesh:
		delete m_Data.pmatWorld;
		m_Data.pd3dXMesh->Release();
		break;
	}
}
