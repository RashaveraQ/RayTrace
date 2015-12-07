#pragma once
#include "cuda_runtime.h"
#include <d3d9.h>
#include "sp.cuh"

class DevNode;

struct DevInfo
{
	D3DMATERIAL9	Material;
	bool			isEnter;	// ���荞��
	double			Distance;	// ��_�܂ł̋���
	Sp				Cross;		// ��_���W
	Sp				Vertical;	// �@���x�N�g��
	const DevNode *	pNode;		// 
	double	Refractive;			// ���ܗ�
	__device__ DevInfo()
	{
		pNode = 0;
	}
};
