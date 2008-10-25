#ifndef __SP4CUDA_H
#define __SP4CUDA_H

//#include <d3dx9.h>

struct sp4cuda
{
	double	x;
	double	y;
	double	z;
};

#include "matrix4cuda.h"

TARGET sp4cuda sp_sp(double ix, double iy, double iz);
TARGET sp4cuda sp_sp2(sp4cuda in);
TARGET sp4cuda sp_sp3(matrix4cuda m);
TARGET double sp_abs(sp4cuda in);
TARGET sp4cuda sp_plus(sp4cuda lhs, sp4cuda rhs);
TARGET double sp_internal_multiple(sp4cuda lhs, sp4cuda rhs);
TARGET sp4cuda sp_minus(sp4cuda lhs, sp4cuda rhs);
TARGET sp4cuda sp_minus_signed(sp4cuda lhs);
TARGET sp4cuda sp_multiple(sp4cuda lhs, double k);
TARGET sp4cuda sp_divide(sp4cuda lhs, double a);
TARGET sp4cuda sp_multiple2(double k, sp4cuda rhs);
TARGET sp4cuda sp_e(sp4cuda lhs);
//TARGET D3DMATERIAL9 sp_getMaterial(const sp4cuda& s);
//TARGET sp4cuda sp_sp(const D3DMATERIAL9& mtrl);

#endif	// __SP4CUDA_H
