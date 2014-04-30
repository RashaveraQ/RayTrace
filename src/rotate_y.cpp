#include "stdafx.h"

rotate_y::rotate_y( double angle ) : matrix( 4, 4 )
{
	double	th = angle / 180.0 * M_PI;

	m_data[0][0] = cos(th);
	m_data[0][2] = sin(th);
	m_data[2][0] = -sin(th);
	m_data[2][2] = cos(th);
//	set_data( 1, 1,  cos( th ) );
//	set_data( 1, 3,  sin( th ) );
//	set_data( 3, 1, -sin( th ) );
//	set_data( 3, 3,  cos( th ) );
}

