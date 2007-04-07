#include "stdafx.h"

rotate_z::rotate_z( double angle ) : matrix( 4, 4 )
{
	double	th = angle / 180.0 * M_PI;

	m_data[0][0] = m_data[1][1] = cos(th);
	m_data[1][0] = sin(th);
	m_data[0][1] = -m_data[1][0];

//	set_data( 1, 1, cos( th ) );
//	set_data( 1, 2, -sin( th ) );
//	set_data( 2, 1, sin( th ) );
//	set_data( 2, 2, cos( th ) );
}

