#include "stdafx.h"

rotate_y::rotate_y( float angle ) : matrix( 4, 4 )
{
	float	th = angle / 180.0f * M_PI;

	float costh = cos(th);
	float sinth = sin(th);

	set_data(1, 1,  costh);
	set_data(1, 3,  sinth);
	set_data(3, 1, -sinth);
	set_data(3, 3,  costh);
}

