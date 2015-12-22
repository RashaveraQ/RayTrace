#include "stdafx.h"

rotate_x::rotate_x( float angle ) : matrix( 4, 4 )
{
	float	th = angle / 180.0f * M_PI;

	float costh = cosf(th);
	float sinth = sinf(th);

	set_data(2, 2,  costh);
	set_data(2, 3, -sinth);
	set_data(3, 2,  sinth);
	set_data(3, 3,  costh);
}

