#ifndef __ROTATE_H
#define __ROTATE_H

#include "matrix.h"

struct	rotate : public matrix
{
	rotate( sp axis, double angle);
	rotate( double nx = 0.0, double ny = 0.0, double nz = 1.0, double angle = 0.0 );
	void	init( double nx, double ny, double nz, double angle );
};

#endif
