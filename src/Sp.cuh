#pragma once

#include <d3d9.h>
struct	matrix;

#ifndef PERSPECTIVE_RATIO
#define PERSPECTIVE_RATIO 0.1
#endif // PERSPECTIVE_RATIO

#ifdef __CUDACC__
#define TARGET __device__ __host__
#else
#define TARGET
#endif // __CUDACC__

struct sp
{
	float	x;
	float	y;
	float	z;
	TARGET sp() : x(0), y(0), z(0) {}
	TARGET sp(float ix, float iy, float iz) : x(ix), y(iy), z(iz) {}
	TARGET sp(const sp& isp) : x(isp.x), y(isp.y), z(isp.z) {}
	TARGET sp(const matrix& mat);
	TARGET sp(const D3DMATERIAL9& mtrl);
	TARGET bool operator==(const sp&) const;
	TARGET bool operator!=(const sp&) const;
	TARGET sp&	operator=(const sp&);
	TARGET float	operator*(const sp&) const;
	TARGET sp	operator+(const sp&) const;
	TARGET sp	operator-(const sp&) const;
	TARGET sp	operator-(void) const;
	TARGET sp	operator*(float) const;
	TARGET sp	operator/(float) const;
	TARGET sp	e() const;
	TARGET float	abs() const { return sqrt(x*x + y*y + z*z); };
	//void	print();
	TARGET void getPOINT(long& ox, long& oy, long cx, long cy) const;
	TARGET friend 	sp	operator*(float, const sp&);
	TARGET D3DMATERIAL9 getMaterial() const;
};

#undef TARGET
