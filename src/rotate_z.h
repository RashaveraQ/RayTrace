#ifndef __ROTATE_Z_H
#define __ROTATE_Z_H

#include "matrix.cuh"

struct	rotate_z : public matrix
{
	rotate_z( float angle );
};

#endif
