#include "stdafx.h"

rotate_y::rotate_y( double angle ) : matrix( 4, 4 )
{
	double	th = angle / 180.0 * M_PI;

	double costh = cos(th);
	double sinth = sin(th);

	set_data(1, 1,  costh);
	set_data(1, 3,  sinth);
	set_data(3, 1, -sinth);
	set_data(3, 3,  costh);
}

