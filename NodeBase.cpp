#include "stdafx.h"
#include "RayTraceDoc.h"

#include "NodeBase_kernel.cpp"

BOOL NodeBase::MakeMemoryDCfromTextureFileName()
{
	HBITMAP		h;
	BITMAP		b;
	class CBitmap*	p;

	CDC	dc;
	dc.Attach(m_hTextureDC);
	dc.DeleteDC();
	dc.CreateCompatibleDC(NULL);

	if (!(h = (HBITMAP)LoadImage(NULL, m_TextureFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE)))
		return FALSE;

	(p = CBitmap::FromHandle(h))->GetObject(sizeof(BITMAP), &b);

	m_TextureSize.cx = b.bmWidth;
	m_TextureSize.cy = b.bmHeight;

	dc.SelectObject(p);
	dc.BitBlt(0, 0, m_TextureSize.cx, m_TextureSize.cy, &dc, 0, 0, SRCCOPY);

	m_hTextureDC = dc.Detach();

	return TRUE;
}

NodeBase::NodeBase(const CRayTraceDoc* const pDoc, node_type NodeType, const char* const Name, const sp Color)
	: m_NodeType(NodeType), m_Reflect(0), m_Through(0), m_Refractive(1), m_TextureFileName(0)
{
	m_pDoc = (const CRayTraceDoc*)pDoc;
	Set_Name(Name);
	MakeMemoryDCfromTextureFileName();
	m_Material = Color.getMaterial();
}

NodeBase::NodeBase(const NodeBase& other) : m_Scale(4,4), m_Rotate(4,4), m_Move(4,4), m_Matrix(4,4)
{
	m_NodeType = other.m_NodeType;
	memcpy(m_Name, other.m_Name, 99);
	m_Material = other.m_Material;
	m_Scale = other.m_Scale;
	m_Rotate = other.m_Rotate;
	m_Move = other.m_Move;
	m_Matrix = other.m_Matrix;
	m_Boundary = other.m_Boundary;
	m_Reflect = other.m_Reflect;
	m_Through = other.m_Through;
	m_Refractive = other.m_Refractive;
	
	m_TextureFileName = (char *)malloc(strlen(other.m_TextureFileName) + 1);
	memcpy(m_TextureFileName, other.m_TextureFileName, strlen(other.m_TextureFileName));

	MakeMemoryDCfromTextureFileName();
}

void NodeBase::Set_Name(const char* const str)
{
	memcpy(m_Name, str, 99);
}  
