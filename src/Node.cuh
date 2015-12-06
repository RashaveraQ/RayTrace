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

	Matrix		m_Scale;	// �X�P�[���ϊ�
	Matrix		m_Rotate;	// ��]
	Matrix		m_Move;		// ���s�ړ�
	Matrix		m_Pivot;	// �}�j���s���[�^�̒��S�_
	Matrix		m_Matrix;

	float		m_Reflect;		// ���˗�
	float		m_Through;  	// ���ߗ�
	float		m_Refractive;	// ���ܗ�
	
	DevNode* m_Root;

public:
	__device__ bool GetInfo2(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide);
	__device__ Sp GetColor(const Sp& K, const Sp& L, int nest, const DevInfo* pHint, bool fromOutSide);
	__device__ virtual	bool GetInfo(const Sp& K, const Sp& L, DevInfo& info, const DevInfo* pHint, bool fromOutSide) const = 0;
};