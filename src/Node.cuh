#pragma once

#include "cuda_runtime.h"
#ifndef M_PI
#define M_PI (4.0f*atanf(1.0f))
#endif

#include <d3d9.h>
#include "info.cuh"
#include "sp.cuh"
#include "matrix.cuh"
#include "node_type.h"

class DevNode
{
	__device__ virtual	bool IsInside(const sp& L) const = 0;
protected:
	DevNode*	m_pParent;
	node_type	m_NodeType;

	matrix		m_Scale;	// スケール変換
	matrix		m_Rotate;	// 回転
	matrix		m_Move;		// 平行移動
	matrix		m_Pivot;	// マニュピレータの中心点

	DevNode**	m_Root;

	__device__ sp GetPixel(float x, float y) const;

public:
	__device__ DevNode(DevNode** const root, node_type NodeType, const sp Color = sp(-1, -1, -1));
	__device__ DevNode(const DevNode &other);
	__device__ virtual void SetRoot(DevNode** const root) { m_Root = root; }

	__device__ bool GetInfo2(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide);
	__device__ sp GetColor(const sp& K, const sp& L, int nest, const DevInfo* pHint, bool fromOutSide);
	__device__ virtual	bool GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const = 0;
	__device__ bool IsInside2(const sp& L);

	__device__ virtual const DevNode* MakeCopy() const = 0;
	__device__ virtual bool Delete(DevNode*) { return false; }

	__device__ void SetParent(DevNode* pParent) {
		m_pParent = pParent;
	}

	matrix		m_Matrix;
	D3DMATERIAL9 m_Material;
	float		m_Reflect;		// 反射率
	float		m_Through;  	// 透過率
	float		m_Refractive;	// 屈折率
};

