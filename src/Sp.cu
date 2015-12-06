#pragma once

#include "sp.cuh"
#include "Matrix.cuh"

Sp::Sp( const Matrix& m )
{
	x = m.get_data(1,1);
	y = m.get_data(2,1);
	z = m.get_data(3,1);
}

bool Sp::operator==( const Sp& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}

bool Sp::operator!=( const Sp& a ) const
{
	return x != a.x || y != a.y || z != a.z;
}

Sp&	Sp::operator=( const Sp& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

double	Sp::operator*( const Sp& a ) const
{
	return x*a.x+y*a.y+z*a.z;
}

Sp Sp::operator+( const Sp& a) const
{
	return Sp( x+a.x, y+a.y, z+a.z );
}

Sp Sp::operator-( const Sp& a) const
{
	return Sp( x-a.x, y-a.y, z-a.z );
}

Sp Sp::operator-( void ) const
{
	return Sp( -x, -y, -z );
}

Sp Sp::operator*( double k ) const
{
	return Sp( x*k, y*k, z*k );
}

Sp Sp::operator/( double a ) const
{
	return Sp( x/a, y/a, z/a );
}
/*
void Sp::print()
{
	printf( "(%f,%f,%f)\n", x, y, z );
}
*/
Sp	operator*( double k, const Sp& a)
{
	return a*k;
}

Sp	Sp::e() const
{
	double	r = sqrt( x*x+y*y+z*z );

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

Sp::Sp(const D3DMATERIAL9& mtrl)
{
	x = 256 * mtrl.Diffuse.r;
	y = 256 * mtrl.Diffuse.g;
	z = 256 * mtrl.Diffuse.b;
}
