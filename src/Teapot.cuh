﻿#pragma once
#include "Node.cuh"

class DevTeapot : public DevNode
{
public:
	// コンストラクタ
	__device__ DevTeapot(DevNode** const root = 0, const Sp Color = Sp(255, 255, 255));
	__device__ DevTeapot(const DevTeapot& other) : DevNode(other) {}

	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const Sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevTeapot(*this); }
};
