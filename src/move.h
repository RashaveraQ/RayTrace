#ifndef	__MOVE_H
#define	__MOVE_H

#include "matrix.h"

struct	move : public matrix
{
	move( double x = 0.0, double y = 0.0, double z = 0.0 ) : matrix( 4, 4 )
	{
		init( x, y, z );
	}
	void	init( double x, double y, double z )
	{
		set_data( 1, 4, x );
		set_data( 2, 4, y );
		set_data( 3, 4, z );
	}
};

#endif
