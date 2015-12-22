#include "stdafx.h"

sp::sp( const matrix& m )
{
	x = m.get_data(1,1);
	y = m.get_data(2,1);
	z = m.get_data(3,1);
}

bool sp::operator==( const sp& a ) const
{
	return x == a.x && y == a.y && z == a.z;
}

bool sp::operator!=( const sp& a ) const
{
	return x != a.x || y != a.y || z != a.z;
}

sp&	sp::operator=( const sp& a )
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

float	sp::operator*(const sp& a) const
{
	return x*a.x+y*a.y+z*a.z;
}

sp	sp::operator+( const sp& a) const
{
	return sp( x+a.x, y+a.y, z+a.z );
}

sp      sp::operator-( const sp& a) const
{
	return sp( x-a.x, y-a.y, z-a.z );
}

sp      sp::operator-( void ) const
{
	return sp( -x, -y, -z );
}

sp      sp::operator*(float k) const
{
	return sp( x*k, y*k, z*k );
}

sp	sp::operator/(float a) const
{
	return sp( x/a, y/a, z/a );
}

void	sp::print()
{
	printf( "(%f,%f,%f)\n", x, y, z );
}

sp	operator*(float k, const sp& a)
{
	return a*k;
}

sp	sp::e() const
{
	float	r = sqrt(x*x + y*y + z*z);

	return sp( x/r, y/r, z/r );
}

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

sp::sp(const D3DMATERIAL9& mtrl)
{
	x = 256 * mtrl.Diffuse.r;
	y = 256 * mtrl.Diffuse.g;
	z = 256 * mtrl.Diffuse.b;
}
