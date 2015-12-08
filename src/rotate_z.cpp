#include "stdafx.h"

rotate_z::rotate_z( float angle ) : matrix( 4, 4 )
{
	float	th = angle / 180.0f * M_PI;

	float costh = cosf(th);
	float sinth = sinf(th);

	set_data(1, 1, costh);
	set_data(2, 1, sinth);
	set_data(1, 2, -sinth);
	set_data(2, 2, costh);
}

