#pragma once
#include "Node.cuh"

class DevSphere : public DevNode
{
public:
	// コンストラクタ
	__device__ DevSphere(DevNode* const root = 0, const char* const Name = "Sphere", const Sp Color = Sp(255, 255, 255));
	__device__ DevSphere(const DevSphere& other) : DevNode(other) {}

	__device__ virtual bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ virtual bool IsInside(const Sp& L) const;
	__device__ const DevNode* MakeCopy() const { return new DevSphere(*this); }
};
