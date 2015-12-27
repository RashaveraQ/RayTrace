#pragma once
#include "Gathering.cuh"

class DevPlus : public DevGathering
{
public:
	// コンストラクタ
	__device__ DevPlus(DevNode** const root = 0) : DevGathering(root, PLUS) {}
	__device__ DevPlus(const DevPlus& other) : DevGathering(other) {}

	__device__ virtual	bool GetInfo(const sp& K, const sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevPlus(*this); }
	__device__ void AttachRoot(DevNode** const root);
};
