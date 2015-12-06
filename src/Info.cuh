#pragma once
#include "cuda_runtime.h"
#include <d3d9.h>
#include "sp.cuh"

class DevNode;

struct DevInfo
{
	D3DMATERIAL9	Material;
	bool			isEnter;	// 入り込む
	double			Distance;	// 交点までの距離
	Sp				Cross;		// 交点座標
	Sp				Vertical;	// 法線ベクトル
	const DevNode *	pNode;		// 
	double	Refractive;			// 屈折率
	__device__ DevInfo()
	{
		pNode = 0;
	}
};
