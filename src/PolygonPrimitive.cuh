﻿#pragma once
#include "Node.cuh"

class DevPolygonPrimitive : public DevNode
{
public:
	// コンストラクタ
	__device__ DevPolygonPrimitive(DevNode** const root = 0, const sp Color = sp(255, 255, 255));
	__device__ DevPolygonPrimitive(const DevPolygonPrimitive& other) : DevNode(other) {}

	__device__ virtual	bool GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevPolygonPrimitive(*this); }
};