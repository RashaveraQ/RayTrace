#include "stdafx.h"

rotate::rotate(	float nx, float ny, float nz, float angle )
	: matrix( 4, 4 )
{
	init( nx, ny, nz, angle );
}

rotate::rotate(sp axis, float angle) : matrix(4,4)
{
	init( axis.x, axis.y, axis.z, angle);
}

void	rotate::init( float nx, float ny, float nz, float angle )
{
	float	th1, th2;
	float	r;

	th1 = th2 = 0.0;

	if ( r = sqrt( nx*nx + ny*ny ) )
	{
		if ( ny > 0.0 )
			th1 = asin( nx / r );
		else
			th1 = M_PI - asin( nx / r );
	}

	if ( r = sqrt( nx*nx + ny*ny + nz*nz ) )
	{
		if ( nz > 0.0 )
			th2 = acos( nz / r );
		else
			th2 = M_PI - acos( -nz / r );
	}

	matrix	ans(	rotate_z( 180.0 / M_PI * th1 ) *
			rotate_x( 180.0 / M_PI * th2 ) *
			rotate_z( angle ) *
			rotate_x( -180.0 / M_PI * th2 ) *
			rotate_z( -180.0 / M_PI * th1 ) );
	
	for ( int i = 0; i < 3; i++ )
		for ( int j = 0; j < 3; j++ )
			m_data[i][j] =  ans.m_data[i][j];
}
