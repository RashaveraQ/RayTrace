#ifndef __SP4CUDA_H
#define __SP4CUDA_H

#include <d3dx9.h>

struct sp4cuda
{
	double	x;
	double	y;
	double	z;
};

#include "matrix4cuda.h"

TARGET sp4cuda sp_sp(double ix, double iy, double iz);
TARGET sp4cuda sp_sp(const sp4cuda& in);
TARGET double sp_abs(const sp4cuda& in);
TARGET sp4cuda sp_sp(const matrix4cuda& m);
TARGET sp4cuda sp_plus(const sp4cuda& lhs, const sp4cuda& rhs);
TARGET double sp_multiple(const sp4cuda& lhs, const sp4cuda& rhs);
TARGET sp4cuda sp_minus(const sp4cuda& lhs, const sp4cuda& rhs);
TARGET sp4cuda sp_minus(const sp4cuda& lhs);
TARGET sp4cuda sp_multiple(const sp4cuda& lhs, double k);
TARGET sp4cuda sp_divide(const sp4cuda& lhs, double a);
TARGET sp4cuda sp_multiple(double k, const sp4cuda& rhs);
TARGET sp4cuda sp_e(const sp4cuda& lhs);
TARGET D3DMATERIAL9 sp_getMaterial(const sp4cuda& s);
TARGET sp4cuda sp_sp(const D3DMATERIAL9& mtrl);

#endif	// __SP4CUDA_H
