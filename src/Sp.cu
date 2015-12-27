#pragma once

#include <math.h>
#include "sp.cuh"
#include "matrix.cuh"

__device__
sp::sp(const matrix& m)
{
	x = m.get_data(1,1);
	y = m.get_data(2,1);
	z = m.get_data(3,1);
}

__device__
bool sp::operator==( const sp& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}

__device__ 
bool sp::operator!=(const sp& a) const
{
	return x != a.x || y != a.y || z != a.z;
}

__device__ 
sp&	sp::operator=(const sp& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

__device__ 
float	sp::operator*(const sp& a) const
{
	return x*a.x+y*a.y+z*a.z;
}

__device__ 
sp sp::operator+(const sp& a) const
{
	return sp( x+a.x, y+a.y, z+a.z );
}

__device__
sp sp::operator-(const sp& a) const
{
	return sp( x-a.x, y-a.y, z-a.z );
}

__device__
sp sp::operator-( void ) const
{
	return sp( -x, -y, -z );
}

__device__
sp sp::operator*(float k) const
{
	return sp( x*k, y*k, z*k );
}

__device__
sp sp::operator/(float a) const
{
	return sp( x/a, y/a, z/a );
}
/*
void sp::print()
{
	printf( "(%f,%f,%f)\n", x, y, z );
}
*/

__device__ 
sp	operator*(float k, const sp& a)
{
	return a*k;
}

__device__
sp	sp::e() const
{
	float	r = sqrt(x*x + y*y + z*z);

	return sp( x/r, y/r, z/r );
}

/*
POINT sp::getPOINT(const CSize& size) const
{
	POINT ans;
	if (z > -20) {
		ans.x = (long)(((x / (PERSPECTIVE_RATIO * (z + 20))) + 10) * size.cx / 20);
		ans.y = (long)(((y / (PERSPECTIVE_RATIO * (z + 20))) + 10) * size.cx / 20);
	} else {
		ans.x = (long)(((x / 1E-10) + 10) * size.cx / 20);
		ans.y = (long)(((y / 1E-10) + 10) * size.cx / 20);
	}
	return ans;
}
*/

__device__
D3DMATERIAL9 sp::getMaterial() const
{
	D3DMATERIAL9	mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = (float)(x / 256);
	mtrl.Diffuse.g = mtrl.Ambient.g = (float)(y / 256);
	mtrl.Diffuse.b = mtrl.Ambient.b = (float)(z / 256);
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	return mtrl;
}

__device__ 
sp::sp(const D3DMATERIAL9& mtrl)
{
	x = 256 * mtrl.Diffuse.r;
	y = 256 * mtrl.Diffuse.g;
	z = 256 * mtrl.Diffuse.b;
}

__device__
void sp::getPOINT(long& ox, long& oy, long cx, long cy) const
{
	if (z > -20) {
		ox = (long)(((x / (PERSPECTIVE_RATIO * (z + 20))) + 10) * cx / 20);
		oy = (long)(((y / (PERSPECTIVE_RATIO * (z + 20))) + 10) * cx / 20);
	}
	else {
		ox = (long)(((x / 1E-10) + 10) * cx / 20);
		oy = (long)(((y / 1E-10) + 10) * cx / 20);
	}
}
