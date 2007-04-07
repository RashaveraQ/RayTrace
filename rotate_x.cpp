#include "stdafx.h"

rotate_x::rotate_x( double angle ) : matrix( 4, 4 )
{
	double	th = angle / 180.0 * M_PI;

	m_data[1][1] = cos(th);
	m_data[1][2] = -sin(th);
	m_data[2][1] = sin(th);
	m_data[2][2] = cos(th);
	
	//set_data( 2, 2,  cos( th ) );
	//set_data( 2, 3, -sin( th ) );
	//set_data( 3, 2,  sin( th ) );
	//set_data( 3, 3,  cos( th ) );
}

