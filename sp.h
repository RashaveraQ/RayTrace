#ifndef __SP_H
#define __SP_H

#include "sp4cuda.h"

struct	matrix;

struct sp
{
	double	x;
	double	y;
	double	z;
	sp() : x(0), y(0), z(0) {} 
	sp(double ix, double iy, double iz) : x(ix), y(iy), z(iz) {}
	sp( const sp& isp) : x(isp.x), y(isp.y), z(isp.z) {}
	sp( const matrix& mat );
	sp( const D3DMATERIAL9& mtrl);
	sp&	operator=( const sp& );
	double	operator*( const sp& ) const;
	sp	operator+( const sp& ) const;
	sp	operator-( const sp& ) const;
	sp	operator-( void ) const;
	sp	operator*( double ) const;
	sp	operator/( double ) const;
	sp	e() const;
	double	abs() { return sqrt(x*x+y*y+z*z); };
	POINT getPOINT(const CSize& size) const;
friend 	sp	operator*(  double, const sp& );
	D3DMATERIAL9 getMaterial() const;
	operator sp4cuda() { return sp_sp(x,y,z); }
};

#include "matrix.h"

inline sp::sp(const matrix& m)
{
	x = m.m_data[0][0];
	y = m.m_data[1][0];
	z = m.m_data[2][0];
}

inline sp& sp::operator=(const sp& a)
{
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

inline double sp::operator*(const sp& a) const
{
	return x * a.x + y * a.y + z * a.z;
}

inline sp sp::operator+(const sp& a) const
{
	return sp(x + a.x, y + a.y, z + a.z);
}

inline sp sp::operator-(const sp& a) const
{
	return sp(x - a.x, y - a.y, z - a.z);
}

inline sp sp::operator-(void) const
{
	return sp(-x, -y, -z);
}

inline sp sp::operator*(double k) const
{
	return sp(x * k, y * k, z * k);
}

inline sp sp::operator/( double a ) const
{
	return sp( x/a, y/a, z/a );
}

inline sp operator*( double k, const sp& a)
{
	return a*k;
}

inline sp sp::e() const
{
	double	r = sqrt( x*x+y*y+z*z );

	return sp( x/r, y/r, z/r );
}

inline POINT sp::getPOINT(const CSize& size) const
{
#define PERSPECTIVE_RATIO 0.1
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

inline D3DMATERIAL9 sp::getMaterial() const
{
	D3DMATERIAL9	mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = (float)(x / 256);
	mtrl.Diffuse.g = mtrl.Ambient.g = (float)(y / 256);
	mtrl.Diffuse.b = mtrl.Ambient.b = (float)(z / 256);
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	return mtrl;
}

inline sp::sp(const D3DMATERIAL9& mtrl)
{
	x = 256 * mtrl.Diffuse.r;
	y = 256 * mtrl.Diffuse.g;
	z = 256 * mtrl.Diffuse.b;
}

#endif