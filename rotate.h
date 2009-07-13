#ifndef __ROTATE_H
#define __ROTATE_H

#include "matrix.h"

struct	rotate : public matrix
{
	rotate( sp axis, float angle);
	rotate( float nx = 0.0, float ny = 0.0, float nz = 1.0, float angle = 0.0 );
	void	init( float nx, float ny, float nz, float angle );
};

#endif
