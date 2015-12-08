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

	th1 = th2 = 0.0f;

	if ( r = sqrtf( nx*nx + ny*ny ) )
	{
		if ( ny > 0.0 )
			th1 = asinf( nx / r );
		else
			th1 = M_PI - asinf( nx / r );
	}

	if ( r = sqrt( nx*nx + ny*ny + nz*nz ) )
	{
		if ( nz > 0.0f )
			th2 = acosf( nz / r );
		else
			th2 = M_PI - acosf( -nz / r );
	}

	matrix	ans(	rotate_z( 180.0f / M_PI * th1 ) *
			rotate_x( 180.0f / M_PI * th2 ) *
			rotate_z( angle ) *
			rotate_x( -180.0f / M_PI * th2 ) *
			rotate_z( -180.0f / M_PI * th1 ) );
	
	for ( int i = 1; i < 4; i++ )
		for ( int j = 1; j < 4; j++ )
			set_data(i,j, ans.get_data(i,j));
}
