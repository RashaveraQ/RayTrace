#pragma once
#include "Gathering.cuh"

class DevPlus : public DevGathering
{
public:
	// コンストラクタ
	__device__ DevPlus(DevNode* const root = 0, const char* const Name = "Plus") : DevGathering(root, PLUS, Name) {}
	__device__ DevPlus(const DevPlus& other) : DevGathering(other) {}

	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const Sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevPlus(*this); }
};
