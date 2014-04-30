#include "stdafx.h"

matrix::matrix(int gyo,int retu)
{
	m_Height = gyo;
	m_Width = retu;

	m_data[0][0] = 1.0;
	m_data[1][1] = 1.0;
	m_data[2][2] = 1.0;
	m_data[3][3] = 1.0;
	m_data[0][1] = 0.0;
	m_data[0][2] = 0.0;
	m_data[0][3] = 0.0;
	m_data[1][0] = 0.0;
	m_data[1][2] = 0.0;
	m_data[1][3] = 0.0;
	m_data[2][0] = 0.0;
	m_data[2][1] = 0.0;
	m_data[2][3] = 0.0;
	m_data[3][0] = 0.0;
	m_data[3][1] = 0.0;
	m_data[3][2] = 0.0;
}

matrix::matrix(const matrix& Matrix)
{
	m_Width = Matrix.get_width();
	m_Height = Matrix.get_height();

	for ( int i = 0; i < m_Height; i++ )
		for ( int j = 0; j < m_Width; j++ )
			m_data[i][j] = Matrix.m_data[i][j];
}

matrix::matrix(const sp& Sp, double d)
{
	m_Width = 1;
	m_Height = 4;

	m_data[0][0] = Sp.x;
	m_data[1][0] = Sp.y;
	m_data[2][0] = Sp.z;
	m_data[3][0] = d;
}

matrix	matrix::operator=(const matrix& Matrix)
{
	for ( int i = 0; i < m_Height; i++ )
		for ( int j = 0; j < m_Width; j++ )
			m_data[i][j] = Matrix.m_data[i][j];
	return *this;
}

matrix	matrix::operator-(const matrix& Matrix) const
{
	matrix	ans( m_Height, m_Width );

	for ( int i = 0; i < m_Height; i++ )
		for ( int j = 0; j < m_Width; j++ )
			ans.m_data[i][j] = m_data[i][j] - Matrix.m_data[i][j];

	return ans;
}

matrix	matrix::operator+(const matrix& Matrix) const
{
	matrix	ans( m_Height, m_Width );

	for ( int i = 0; i < m_Height; i++ )
		for ( int j = 0; j < m_Width; j++ )
			ans.m_data[i][j] =  m_data[i][j] + Matrix.m_data[i][j];

	return ans;
}

matrix	matrix::operator*(double k) const
{
	matrix	ans( m_Height, m_Width );

	for ( int i = 0; i < m_Height; i++ )
		for ( int j = 0; j < m_Width; j++ )
			ans.m_data[i][j] = m_data[i][j] * k;

	return ans;
}

matrix	matrix::operator/(double k) const
{
	matrix	ans( m_Height, m_Width );

	for (int i = 0; i < m_Height; i++)
		for (int j = 0; j < m_Width; j++)
			ans.m_data[i][j] = m_data[i][j] / k;

	return ans;
}

matrix	matrix::operator*(const matrix& Matrix) const
{
	matrix	ans( m_Height, Matrix.get_width() );

	for (int i = 0; i < ans.m_Height; i++) {
		for (int j = 0; j < ans.m_Width; j++) {
			double data = 0.0;
			for ( int k = 0; k < m_Width; k++ )
				data += m_data[i][k] * Matrix.m_data[k][j];
			ans.m_data[i][j] = data;
		}
	}

	return ans;
}

matrix	matrix::Inv() const
{
	matrix	mat( m_Width, m_Height );

	double	A = d();
	double	m;

	int	gyo, retu;

	for ( gyo = 1; gyo <= m_Height; gyo++ )
	{
		for ( retu = 1; retu <= m_Width; retu++ )
		{
			m = M(retu, gyo);
			m = ( ( gyo + retu ) % 2 ) ? -m : m;
			mat.m_data[gyo-1][retu-1] = m / A; // set_data( gyo, retu, m / A );
		}
	}

	return mat;
}

double	matrix::M( int gyo, int retu ) const
{
	double ans = 0;

	switch ( retu )
	{
	case 1:
		switch ( gyo )
		{
		case 1:	// M11
			ans = m_data[1][1] * m_data[2][2] * m_data[3][3] - m_data[1][1] * m_data[2][3] * m_data[3][2]
				+ m_data[1][2] * m_data[2][3] * m_data[3][1] - m_data[1][2] * m_data[2][1] * m_data[3][3]
				+ m_data[1][3] * m_data[2][1] * m_data[3][2] - m_data[1][3] * m_data[2][2] * m_data[3][1];
			return ans;
		case 2: // M12
			ans = m_data[0][1] * m_data[2][2] * m_data[3][3] - m_data[0][1] * m_data[2][3] * m_data[3][2]
				+ m_data[0][2] * m_data[2][3] * m_data[3][1] - m_data[0][2] * m_data[2][1] * m_data[3][3]
				+ m_data[0][3] * m_data[2][1] * m_data[3][2] - m_data[0][3] * m_data[2][2] * m_data[3][1];
			return ans;
		case 3: // M13
			ans = m_data[0][1] * m_data[1][2] * m_data[3][3] - m_data[0][1] * m_data[1][3] * m_data[3][2]
				+ m_data[0][2] * m_data[1][3] * m_data[3][1] - m_data[0][2] * m_data[1][1] * m_data[3][3]
				+ m_data[0][3] * m_data[1][1] * m_data[3][2] - m_data[0][3] * m_data[1][2] * m_data[3][1];
			return ans;
		case 4:	// M14
			ans = m_data[0][1] * m_data[1][2] * m_data[2][3] - m_data[0][1] * m_data[1][3] * m_data[2][2]
				+ m_data[0][2] * m_data[1][3] * m_data[2][1] - m_data[0][2] * m_data[1][1] * m_data[2][3]
				+ m_data[0][3] * m_data[1][1] * m_data[2][2] - m_data[0][3] * m_data[1][2] * m_data[2][1];
			return ans;
		}
	case 2:
		switch ( gyo )
		{
		case 1:	// M21
			ans = m_data[1][0] * m_data[2][2] * m_data[3][3] - m_data[1][0] * m_data[2][3] * m_data[3][2]
				+ m_data[1][2] * m_data[2][3] * m_data[3][0] - m_data[1][2] * m_data[2][0] * m_data[3][3]
				+ m_data[1][3] * m_data[2][0] * m_data[3][2] - m_data[1][3] * m_data[2][2] * m_data[3][0];
			return ans;
		case 2:	// M22
			ans = m_data[0][0] * m_data[2][2] * m_data[3][3] - m_data[0][0] * m_data[2][3] * m_data[3][2]
				+ m_data[0][2] * m_data[2][3] * m_data[3][0] - m_data[0][2] * m_data[2][0] * m_data[3][3]
				+ m_data[0][3] * m_data[2][0] * m_data[3][2] - m_data[0][3] * m_data[2][2] * m_data[3][0];
			return ans;
		case 3:	// M23
			ans = m_data[0][0] * m_data[1][2] * m_data[3][3] - m_data[0][0] * m_data[1][3] * m_data[3][2]
				+ m_data[0][2] * m_data[1][3] * m_data[3][0] - m_data[0][2] * m_data[1][0] * m_data[3][3]
				+ m_data[0][3] * m_data[1][0] * m_data[3][2] - m_data[0][3] * m_data[1][2] * m_data[3][0];
			return ans;
		case 4:	// M24
			ans = m_data[0][0] * m_data[1][2] * m_data[2][3] - m_data[0][0] * m_data[1][3] * m_data[2][2]
				+ m_data[0][2] * m_data[1][3] * m_data[2][0] - m_data[0][2] * m_data[1][0] * m_data[2][3]
				+ m_data[0][3] * m_data[1][0] * m_data[2][2] - m_data[0][3] * m_data[1][2] * m_data[2][0];
			return ans;
		}
	case 3:
		switch ( gyo )
		{
		case 1:	// M31
			ans = m_data[1][0] * m_data[2][1] * m_data[3][3] - m_data[1][0] * m_data[2][3] * m_data[3][1]
				+ m_data[1][1] * m_data[2][3] * m_data[3][0] - m_data[1][1] * m_data[2][0] * m_data[3][3]
				+ m_data[1][3] * m_data[2][0] * m_data[3][1] - m_data[1][3] * m_data[2][1] * m_data[3][0];
			return ans;
		case 2:	// M32
			ans = m_data[0][0] * m_data[2][1] * m_data[3][3] - m_data[0][0] * m_data[2][3] * m_data[3][1]
				+ m_data[0][1] * m_data[2][3] * m_data[3][0] - m_data[0][1] * m_data[2][0] * m_data[3][3]
				+ m_data[0][3] * m_data[2][0] * m_data[3][1] - m_data[0][3] * m_data[2][1] * m_data[3][0];
			return ans;
		case 3:	// M33
			ans = m_data[0][0] * m_data[1][1] * m_data[3][3] - m_data[0][0] * m_data[1][3] * m_data[3][1]
				+ m_data[0][1] * m_data[1][3] * m_data[3][0] - m_data[0][1] * m_data[1][0] * m_data[3][3]
				+ m_data[0][3] * m_data[1][0] * m_data[3][1] - m_data[0][3] * m_data[1][1] * m_data[3][0];
			return ans;
		case 4:	// M34
			ans = m_data[0][0] * m_data[1][1] * m_data[2][3] - m_data[0][0] * m_data[1][3] * m_data[2][1]
				+ m_data[0][1] * m_data[1][3] * m_data[2][0] - m_data[0][1] * m_data[1][0] * m_data[2][3]
				+ m_data[0][3] * m_data[1][0] * m_data[2][1] - m_data[0][3] * m_data[1][1] * m_data[2][0];
			return ans;
		}
	case 4:
		switch ( gyo )
		{
		case 1: // M41
			ans = m_data[1][0] * m_data[2][1] * m_data[3][2] - m_data[1][0] * m_data[2][2] * m_data[3][1]
				+ m_data[1][1] * m_data[2][2] * m_data[3][0] - m_data[1][1] * m_data[2][0] * m_data[3][2]
				+ m_data[1][2] * m_data[2][0] * m_data[3][1] - m_data[1][2] * m_data[2][1] * m_data[3][0];
			return ans;
		case 2:	// M42
			ans = m_data[0][0] * m_data[2][1] * m_data[3][2] - m_data[0][0] * m_data[2][2] * m_data[3][1]
				+ m_data[0][1] * m_data[2][2] * m_data[3][0] - m_data[0][1] * m_data[2][0] * m_data[3][2]
				+ m_data[0][2] * m_data[2][0] * m_data[3][1] - m_data[0][2] * m_data[2][1] * m_data[3][0];
			return ans;
		case 3:	// M43
			ans = m_data[0][0] * m_data[1][1] * m_data[3][2] - m_data[0][0] * m_data[1][2] * m_data[3][1]
				+ m_data[0][1] * m_data[1][2] * m_data[3][0] - m_data[0][1] * m_data[1][0] * m_data[3][2]
				+ m_data[0][2] * m_data[1][0] * m_data[3][1] - m_data[0][2] * m_data[1][1] * m_data[3][0];
			return ans;
		case 4:	// M44
			ans = m_data[0][0] * m_data[1][1] * m_data[2][2] - m_data[0][0] * m_data[1][2] * m_data[2][1]
				+ m_data[0][1] * m_data[1][2] * m_data[2][0] - m_data[0][1] * m_data[1][0] * m_data[2][2]
				+ m_data[0][2] * m_data[1][0] * m_data[2][1] - m_data[0][2] * m_data[1][1] * m_data[2][0];
			return ans;
		}
	}
	return ans;
}

double	matrix::d() const
{
	double	ans = 0;

	switch ( m_Height )
	{
	case 2:
		ans = m_data[0][0] * m_data[1][1]
			- m_data[0][1] * m_data[1][0];
		return ans;
	case 3:
		ans = m_data[0][0] * m_data[1][1] * m_data[2][2]
			- m_data[0][0] * m_data[1][2] * m_data[2][1]
			+ m_data[0][1] * m_data[1][2] * m_data[2][0]
			- m_data[0][1] * m_data[1][0] * m_data[2][2]
			+ m_data[0][2] * m_data[1][0] * m_data[2][1]
			- m_data[0][2] * m_data[1][1] * m_data[2][0];
		return ans;
	case 4:
		ans = m_data[0][0] * m_data[1][1] * m_data[2][2] * m_data[3][3]
			- m_data[0][0] * m_data[1][1] * m_data[3][2] * m_data[2][3]
			- m_data[0][0] * m_data[2][1] * m_data[1][2] * m_data[3][3]
			+ m_data[0][0] * m_data[2][1] * m_data[3][2] * m_data[1][3]
			+ m_data[0][0] * m_data[3][1] * m_data[1][2] * m_data[2][3]
			- m_data[0][0] * m_data[3][1] * m_data[2][2] * m_data[1][3]
			- m_data[1][0] * m_data[0][1] * m_data[2][2] * m_data[3][3]
			+ m_data[1][0] * m_data[0][1] * m_data[3][2] * m_data[2][3]
			+ m_data[1][0] * m_data[2][1] * m_data[0][2] * m_data[3][3]
			- m_data[1][0] * m_data[2][1] * m_data[3][2] * m_data[0][3]
			- m_data[1][0] * m_data[3][1] * m_data[0][2] * m_data[2][3]
			+ m_data[1][0] * m_data[3][1] * m_data[2][2] * m_data[0][3]
			+ m_data[2][0] * m_data[0][1] * m_data[1][2] * m_data[3][3]
			- m_data[2][0] * m_data[0][1] * m_data[3][2] * m_data[1][3]
			- m_data[2][0] * m_data[1][1] * m_data[0][2] * m_data[3][3]
			+ m_data[2][0] * m_data[1][1] * m_data[3][2] * m_data[0][3]
			+ m_data[2][0] * m_data[3][1] * m_data[0][2] * m_data[1][3]
			- m_data[2][0] * m_data[3][1] * m_data[1][2] * m_data[0][3]
			- m_data[3][0] * m_data[0][1] * m_data[1][2] * m_data[2][3]
			+ m_data[3][0] * m_data[0][1] * m_data[2][2] * m_data[1][3]
			+ m_data[3][0] * m_data[1][1] * m_data[0][2] * m_data[2][3]
			- m_data[3][0] * m_data[1][1] * m_data[2][2] * m_data[0][3]
			- m_data[3][0] * m_data[2][1] * m_data[0][2] * m_data[1][3]
			+ m_data[3][0] * m_data[2][1] * m_data[1][2] * m_data[0][3];
		return ans;
	}
	return ans;
}

void	matrix::print() const
{
	int	i, j;

	for ( i = 0; i < m_Height; i++ )
	{
		for ( j = 0; j < m_Width; j++ )
			printf( " %f", m_data[i][j] );
		printf( "\n" );
	}
}


