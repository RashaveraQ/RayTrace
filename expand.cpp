#include "stdafx.h"

expand::expand(	float r ) : matrix( 4, 4 )
{
	m_data[0][0] = m_data[1][1] = m_data[2][2] = r;
}
