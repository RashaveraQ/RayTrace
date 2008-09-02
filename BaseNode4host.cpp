#include "stdafx.h"
#include "RayTrace.h"
#include "BaseNode.h"

void BaseNode::updateDeviceData()
{
	cudaError_t err;

	if (m_DeviceData) {
	    if (cudaSuccess != (err = cudaFree(m_DeviceData))) {
			MessageBox(0, cudaGetErrorString(err), "cudaFree at Node::updateDeviceData()", MB_OK);
		}
	}

	if (cudaSuccess != (err = cudaMalloc((void**)&m_DeviceData, sizeof(BaseNode)))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMalloc at Node::updateDeviceData()", MB_OK);
	}

	if (cudaSuccess != (err = cudaMemcpy(m_DeviceData, this, sizeof(BaseNode), cudaMemcpyHostToDevice))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy at Node::updateDeviceData()", MB_OK);
	}
}

BaseNode::BaseNode(node_type NodeType, const char* const Name, const sp Color)
	: m_NodeType(NodeType), m_Reflect(0), m_Through(0), m_Refractive(1), m_DeviceData(0), m_TextureFileName(0)
{
	Set_Name(Name);
	m_Material = Color.getMaterial();
	updateDeviceData();
}

BaseNode::~BaseNode()
{
	cudaError_t err;

	if (m_DeviceData) {
	    if (cudaSuccess != (err = cudaFree(m_DeviceData))) {
			MessageBox(0, cudaGetErrorString(err), "cudaFree at BaseNode::~BaseNode()", MB_OK);
		}
	}
}

BOOL BaseNode::MakeMemoryDCfromTextureFileName()
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

#include "BaseNode.cpp"

