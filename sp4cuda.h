#ifndef __SP4CUDA_H
#define __SP4CUDA_H

struct sp4cuda
{
	float	x;
	float	y;
	float	z;
};

#include "matrix4cuda.h"

extern "C" {
sp4cuda sp_sp(float ix, float iy, float iz);
sp4cuda sp_sp2(sp4cuda in);
sp4cuda sp_sp3(matrix4cuda m);
float sp_abs(sp4cuda in);
sp4cuda sp_plus(sp4cuda lhs, sp4cuda rhs);
float sp_internal_multiple(sp4cuda lhs, sp4cuda rhs);
sp4cuda sp_minus(sp4cuda lhs, sp4cuda rhs);
sp4cuda sp_minus_signed(sp4cuda lhs);
sp4cuda sp_multiple(sp4cuda lhs, float k);
sp4cuda sp_divide(sp4cuda lhs, float a);
sp4cuda sp_multiple2(float k, sp4cuda rhs);
sp4cuda sp_e(sp4cuda lhs);
}

#endif	// __SP4CUDA_H
