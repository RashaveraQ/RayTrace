#include "stdafx.h"

rotate_x::rotate_x( double angle ) : matrix( 4, 4 )
{
	double	th = angle / 180.0 * M_PI;

	double costh = cos(th);
	double sinth = sin(th);

	set_data(2, 2,  costh);
	set_data(2, 3, -sinth);
	set_data(3, 2,  sinth);
	set_data(3, 3,  costh);
}

