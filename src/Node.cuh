#pragma once

#include "cuda_runtime.h"
#ifndef M_PI
#define M_PI (4.0f*atanf(1.0f))
#endif

#include <d3d9.h>
#include "info.cuh"
#include "sp.cuh"
#include "Matrix.cuh"

enum node_type
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

class DevNode
{
protected:
	DevNode* m_pParent;
	node_type	m_NodeType;
	D3DMATERIAL9 m_Material;

	Matrix		m_Scale;	// スケール変換
	Matrix		m_Rotate;	// 回転
	Matrix		m_Move;		// 平行移動
	Matrix		m_Pivot;	// マニュピレータの中心点
	Matrix		m_Matrix;

	float		m_Reflect;		// 反射率
	float		m_Through;  	// 透過率
	float		m_Refractive;	// 屈折率

	DevNode* m_Root;

public:
	__device__ DevNode(DevNode* const root, node_type NodeType, const char* const Name, const Sp Color = Sp(-1, -1, -1))
		: m_Root(root), m_pParent(0), m_NodeType(NodeType), m_Reflect(0), m_Through(0), m_Refractive(1)
	{
		m_Material = Color.getMaterial();
	}
	__device__ DevNode(const DevNode &other);

	__device__ bool GetInfo2(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide);
	__device__ Sp GetColor(const Sp& K, const Sp& L, int nest, const DevInfo* pHint, bool fromOutSide);
	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const = 0;
};