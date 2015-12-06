#pragma once

#include <d3d9.h>
struct	Matrix;

struct	Sp
{
	float	x;
	float	y;
	float	z;
	__device__ Sp() : x(0), y(0), z(0) {} 
	__device__ Sp(double ix, double iy, double iz) : x(ix), y(iy), z(iz) {}
	__device__ Sp(const Sp& isp) : x(isp.x), y(isp.y), z(isp.z) {}
	__device__ Sp(const Matrix& mat);
	__device__ Sp(const D3DMATERIAL9& mtrl);
	__device__ bool operator==(const Sp&) const;
	__device__ bool operator!=(const Sp&) const;
	__device__ Sp&	operator=(const Sp&);
	__device__ double	operator*(const Sp&) const;
	__device__ Sp	operator+(const Sp&) const;
	__device__ Sp	operator-(const Sp&) const;
	__device__ Sp	operator-(void) const;
	__device__ Sp	operator*(double) const;
	__device__ Sp	operator/(double) const;
	__device__ Sp	e() const;
	__device__ double	abs() const { return sqrt(x*x + y*y + z*z); };
	//void	print();
//	POINT getPOINT(const CSize& size) const;
	__device__ friend 	Sp	operator*(double, const Sp&);
	__device__ D3DMATERIAL9 getMaterial() const;
};
