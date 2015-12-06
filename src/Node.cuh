#include "cuda_runtime.h"
#ifndef M_PI
#define M_PI (4.0*atan(1.0))
#endif

class DevInfo;
struct Sp;
#include "Matrix.cuh"

class DevNode
{
protected:
//	DevNode* m_pParent;

	Matrix		m_Scale;	// スケール変換
	Matrix		m_Rotate;	// 回転
	Matrix		m_Move;		// 平行移動
	Matrix		m_Pivot;	// マニュピレータの中心点
	Matrix		m_Matrix;

	float		m_Reflect;		// 反射率
	float		m_Through;  	// 透過率
	float		m_Refractive;	// 屈折率
	
	DevNode* m_Root;

public:
	__device__ bool GetInfo2(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide);
	__device__ Sp GetColor(const Sp& K, const Sp& L, int nest, const DevInfo* pHint, bool fromOutSide);
	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const = 0;
};