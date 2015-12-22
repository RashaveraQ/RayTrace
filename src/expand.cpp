#include "stdafx.h"

expand::expand(	float r ) : matrix( 4, 4 )
{
	set_data(1, 1, r);
	set_data(2, 2, r);
	set_data(3, 3, r);
}
