#include "stdafx.h"

rotate_z::rotate_z( double angle ) : matrix( 4, 4 )
{
	double	th = angle / 180.0 * M_PI;

	double costh = cos(th);
	double sinth = sin(th);

	set_data(1, 1, costh);
	set_data(2, 1, sinth);
	set_data(1, 2, -sinth);
	set_data(2, 2, costh);
}

