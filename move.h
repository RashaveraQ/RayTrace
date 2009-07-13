#ifndef	__MOVE_H
#define	__MOVE_H

#include "matrix.h"

struct	move : public matrix
{
	move( float x = 0.0, float y = 0.0, float z = 0.0 ) : matrix( 4, 4 )
	{
		init( x, y, z );
	}
	void	init( float x, float y, float z )
	{
		m_data[0][3] = x;
		m_data[1][3] = y;
		m_data[2][3] = z;
/*		set_data( 1, 4, x );
		set_data( 2, 4, y );
		set_data( 3, 4, z );
*/	}
};

#endif
