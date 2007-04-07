#ifndef __SP_H
#define __SP_H

struct	matrix;

struct	sp
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
	void	print();
	POINT getPOINT(const CSize& size) const;
friend 	sp	operator*(  double, const sp& );
	D3DMATERIAL9 getMaterial() const;
};

#endif
