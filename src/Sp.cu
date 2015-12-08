#pragma once

#include <math.h>
#include "sp.cuh"
#include "Matrix.cuh"

__device__
Sp::Sp(const Matrix& m)
{
	x = m.get_data(1,1);
	y = m.get_data(2,1);
	z = m.get_data(3,1);
}

__device__
bool Sp::operator==( const Sp& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}

__device__ 
bool Sp::operator!=(const Sp& a) const
{
	return x != a.x || y != a.y || z != a.z;
}

__device__ 
Sp&	Sp::operator=(const Sp& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

__device__ 
float	Sp::operator*(const Sp& a) const
{
	return x*a.x+y*a.y+z*a.z;
}

__device__ 
Sp Sp::operator+(const Sp& a) const
{
	return Sp( x+a.x, y+a.y, z+a.z );
}

__device__
Sp Sp::operator-(const Sp& a) const
{
	return Sp( x-a.x, y-a.y, z-a.z );
}

__device__
Sp Sp::operator-( void ) const
{
	return Sp( -x, -y, -z );
}

__device__
Sp Sp::operator*(float k) const
{
	return Sp( x*k, y*k, z*k );
}

__device__
Sp Sp::operator/(float a) const
{
	return Sp( x/a, y/a, z/a );
}
/*
void Sp::print()
{
	printf( "(%f,%f,%f)\n", x, y, z );
}
*/

__device__ 
Sp	operator*(float k, const Sp& a)
{
	return a*k;
}

__device__
Sp	Sp::e() const
{
	float	r = sqrt(x*x + y*y + z*z);

	return Sp( x/r, y/r, z/r );
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
D3DMATERIAL9 Sp::getMaterial() const
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
Sp::Sp(const D3DMATERIAL9& mtrl)
{
	x = 256 * mtrl.Diffuse.r;
	y = 256 * mtrl.Diffuse.g;
	z = 256 * mtrl.Diffuse.b;
}
