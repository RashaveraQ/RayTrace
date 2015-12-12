﻿#pragma once
#include "Node.cuh"

class DevMinus : public DevNode
{
public:
	// コンストラクタ
	__device__ DevMinus(DevNode* const root = 0, const char* const Name = "Minus", const Sp Color = Sp(255, 255, 255));
	__device__ DevMinus(const DevMinus& other) : DevNode(other) {}

	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const Sp& L) const;
};
