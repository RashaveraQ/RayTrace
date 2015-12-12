#pragma once
#include "Node.cuh"

class DevCylinder : public DevNode
{
public:
	// コンストラクタ
	__device__ DevCylinder(DevNode* const root = 0, const char* const Name = "Cylinder", const Sp Color = Sp(255, 255, 255));
	__device__ DevCylinder(const DevCylinder& other) : DevNode(other) {}

	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const;
	__device__ bool IsInside(const Sp& L) const;
};
