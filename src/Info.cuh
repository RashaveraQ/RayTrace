#pragma once
#include "cuda_runtime.h"
#include <d3d9.h>
#include "sp.cuh"

class DevNode;

struct DevInfo
{
	D3DMATERIAL9	Material;
	float			Distance;	// 交点までの距離
	sp				Cross;		// 交点座標
	sp				Vertical;	// 法線ベクトル
	const DevNode *	pNode;		// 
	__device__ DevInfo()
	{
		pNode = 0;
	}
};
