#ifndef TARGET
#define TARGET
#endif	// TARGET

#include "matrix4cuda.h"

TARGET matrix4cuda matrix_matrix(int gyo, int retu)
{
	matrix4cuda ans;

	ans.m_Height = gyo;
	ans.m_Width = retu;

	ans.m_data[0][0] = 1.0;
	ans.m_data[1][1] = 1.0;
	ans.m_data[2][2] = 1.0;
	ans.m_data[3][3] = 1.0;
	ans.m_data[0][1] = 0.0;
	ans.m_data[0][2] = 0.0;
	ans.m_data[0][3] = 0.0;
	ans.m_data[1][0] = 0.0;
	ans.m_data[1][2] = 0.0;
	ans.m_data[1][3] = 0.0;
	ans.m_data[2][0] = 0.0;
	ans.m_data[2][1] = 0.0;
	ans.m_data[2][3] = 0.0;
	ans.m_data[3][0] = 0.0;
	ans.m_data[3][1] = 0.0;
	ans.m_data[3][2] = 0.0;

	return ans;
}

TARGET matrix4cuda matrix_matrix(const matrix4cuda& Matrix)
{
	matrix4cuda ans;

	ans.m_Width = Matrix.m_Width;
	ans.m_Height = Matrix.m_Height;

	for (int i = 0; i < ans.m_Height; i++)
		for (int j = 0; j < ans.m_Width; j++)
			ans.m_data[i][j] = Matrix.m_data[i][j];

	return ans;
}

TARGET matrix4cuda matrix_matrix(const int w, const int h, const double d[4][4])
{
	matrix4cuda ans;

	ans.m_Width = w;
	ans.m_Height = h;

	for (int i = 0; i < ans.m_Height; i++)
		for (int j = 0; j < ans.m_Width; j++)
			ans.m_data[i][j] = d[i][j];

	return ans;
}

TARGET matrix4cuda matrix_matrix(const sp4cuda& Sp, double d)
{
	matrix4cuda ans;
	
	ans.m_Width = 1;
	ans.m_Height = 4;

	ans.m_data[0][0] = Sp.x;
	ans.m_data[1][0] = Sp.y;
	ans.m_data[2][0] = Sp.z;
	ans.m_data[3][0] = d;

	return ans;
}

TARGET matrix4cuda matrix_minus(const matrix4cuda& lhs, const matrix4cuda& rhs)
{
	matrix4cuda ans = matrix_matrix(lhs.m_Height, lhs.m_Width);

	for (int i = 0; i < lhs.m_Height; i++)
		for (int j = 0; j < lhs.m_Width; j++)
			ans.m_data[i][j] = lhs.m_data[i][j] - rhs.m_data[i][j];

	return ans;
}

TARGET matrix4cuda matrix_plus(const matrix4cuda& lhs, const matrix4cuda& rhs)
{
	matrix4cuda ans = matrix_matrix(lhs.m_Height, lhs.m_Width);

	for (int i = 0; i < lhs.m_Height; i++)
		for (int j = 0; j < lhs.m_Width; j++)
			ans.m_data[i][j] = lhs.m_data[i][j] + rhs.m_data[i][j];

	return ans;
}

TARGET matrix4cuda matrix_multiple(const matrix4cuda& lhs, double k)
{
	matrix4cuda ans = matrix_matrix(lhs.m_Height, lhs.m_Width);

	for (int i = 0; i < lhs.m_Height; i++)
		for (int j = 0; j < lhs.m_Width; j++)
			ans.m_data[i][j] = lhs.m_data[i][j] * k;

	return ans;
}

TARGET matrix4cuda matrix_divide(const matrix4cuda& lhs, double k)
{
	matrix4cuda ans = matrix_matrix(lhs.m_Height, lhs.m_Width);

	for (int i = 0; i < lhs.m_Height; i++)
		for (int j = 0; j < lhs.m_Width; j++)
			ans.m_data[i][j] = lhs.m_data[i][j] / k;

	return ans;
}

TARGET matrix4cuda matrix_multiple(const matrix4cuda& lhs, const matrix4cuda& rhs)
{
	matrix4cuda ans = matrix_matrix(lhs.m_Height, rhs.m_Width);

	for (int i = 0; i < ans.m_Height; i++) {
		for (int j = 0; j < ans.m_Width; j++) {
			double data = 0.0;
			for (int k = 0; k < lhs.m_Width; k++)
				data += lhs.m_data[i][k] * rhs.m_data[k][j];
			ans.m_data[i][j] = data;
		}
	}

	return ans;
}

TARGET double matrix_M(const matrix4cuda& in, int gyo, int retu)
{
	double ans = 0;

	switch (retu) {
	case 1:
		switch (gyo) {
		case 1:	// M11
			ans = in.m_data[1][1] * in.m_data[2][2] * in.m_data[3][3] - in.m_data[1][1] * in.m_data[2][3] * in.m_data[3][2]
				+ in.m_data[1][2] * in.m_data[2][3] * in.m_data[3][1] - in.m_data[1][2] * in.m_data[2][1] * in.m_data[3][3]
				+ in.m_data[1][3] * in.m_data[2][1] * in.m_data[3][2] - in.m_data[1][3] * in.m_data[2][2] * in.m_data[3][1];
			return ans;
		case 2: // M12
			ans = in.m_data[0][1] * in.m_data[2][2] * in.m_data[3][3] - in.m_data[0][1] * in.m_data[2][3] * in.m_data[3][2]
				+ in.m_data[0][2] * in.m_data[2][3] * in.m_data[3][1] - in.m_data[0][2] * in.m_data[2][1] * in.m_data[3][3]
				+ in.m_data[0][3] * in.m_data[2][1] * in.m_data[3][2] - in.m_data[0][3] * in.m_data[2][2] * in.m_data[3][1];
			return ans;
		case 3: // M13
			ans = in.m_data[0][1] * in.m_data[1][2] * in.m_data[3][3] - in.m_data[0][1] * in.m_data[1][3] * in.m_data[3][2]
				+ in.m_data[0][2] * in.m_data[1][3] * in.m_data[3][1] - in.m_data[0][2] * in.m_data[1][1] * in.m_data[3][3]
				+ in.m_data[0][3] * in.m_data[1][1] * in.m_data[3][2] - in.m_data[0][3] * in.m_data[1][2] * in.m_data[3][1];
			return ans;
		case 4:	// M14
			ans = in.m_data[0][1] * in.m_data[1][2] * in.m_data[2][3] - in.m_data[0][1] * in.m_data[1][3] * in.m_data[2][2]
				+ in.m_data[0][2] * in.m_data[1][3] * in.m_data[2][1] - in.m_data[0][2] * in.m_data[1][1] * in.m_data[2][3]
				+ in.m_data[0][3] * in.m_data[1][1] * in.m_data[2][2] - in.m_data[0][3] * in.m_data[1][2] * in.m_data[2][1];
			return ans;
		}
	case 2:
		switch (gyo) {
		case 1:	// M21
			ans = in.m_data[1][0] * in.m_data[2][2] * in.m_data[3][3] - in.m_data[1][0] * in.m_data[2][3] * in.m_data[3][2]
				+ in.m_data[1][2] * in.m_data[2][3] * in.m_data[3][0] - in.m_data[1][2] * in.m_data[2][0] * in.m_data[3][3]
				+ in.m_data[1][3] * in.m_data[2][0] * in.m_data[3][2] - in.m_data[1][3] * in.m_data[2][2] * in.m_data[3][0];
			return ans;
		case 2:	// M22
			ans = in.m_data[0][0] * in.m_data[2][2] * in.m_data[3][3] - in.m_data[0][0] * in.m_data[2][3] * in.m_data[3][2]
				+ in.m_data[0][2] * in.m_data[2][3] * in.m_data[3][0] - in.m_data[0][2] * in.m_data[2][0] * in.m_data[3][3]
				+ in.m_data[0][3] * in.m_data[2][0] * in.m_data[3][2] - in.m_data[0][3] * in.m_data[2][2] * in.m_data[3][0];
			return ans;
		case 3:	// M23
			ans = in.m_data[0][0] * in.m_data[1][2] * in.m_data[3][3] - in.m_data[0][0] * in.m_data[1][3] * in.m_data[3][2]
				+ in.m_data[0][2] * in.m_data[1][3] * in.m_data[3][0] - in.m_data[0][2] * in.m_data[1][0] * in.m_data[3][3]
				+ in.m_data[0][3] * in.m_data[1][0] * in.m_data[3][2] - in.m_data[0][3] * in.m_data[1][2] * in.m_data[3][0];
			return ans;
		case 4:	// M24
			ans = in.m_data[0][0] * in.m_data[1][2] * in.m_data[2][3] - in.m_data[0][0] * in.m_data[1][3] * in.m_data[2][2]
				+ in.m_data[0][2] * in.m_data[1][3] * in.m_data[2][0] - in.m_data[0][2] * in.m_data[1][0] * in.m_data[2][3]
				+ in.m_data[0][3] * in.m_data[1][0] * in.m_data[2][2] - in.m_data[0][3] * in.m_data[1][2] * in.m_data[2][0];
			return ans;
		}
	case 3:
		switch (gyo) {
		case 1:	// M31
			ans = in.m_data[1][0] * in.m_data[2][1] * in.m_data[3][3] - in.m_data[1][0] * in.m_data[2][3] * in.m_data[3][1]
				+ in.m_data[1][1] * in.m_data[2][3] * in.m_data[3][0] - in.m_data[1][1] * in.m_data[2][0] * in.m_data[3][3]
				+ in.m_data[1][3] * in.m_data[2][0] * in.m_data[3][1] - in.m_data[1][3] * in.m_data[2][1] * in.m_data[3][0];
			return ans;
		case 2:	// M32
			ans = in.m_data[0][0] * in.m_data[2][1] * in.m_data[3][3] - in.m_data[0][0] * in.m_data[2][3] * in.m_data[3][1]
				+ in.m_data[0][1] * in.m_data[2][3] * in.m_data[3][0] - in.m_data[0][1] * in.m_data[2][0] * in.m_data[3][3]
				+ in.m_data[0][3] * in.m_data[2][0] * in.m_data[3][1] - in.m_data[0][3] * in.m_data[2][1] * in.m_data[3][0];
			return ans;
		case 3:	// M33
			ans = in.m_data[0][0] * in.m_data[1][1] * in.m_data[3][3] - in.m_data[0][0] * in.m_data[1][3] * in.m_data[3][1]
				+ in.m_data[0][1] * in.m_data[1][3] * in.m_data[3][0] - in.m_data[0][1] * in.m_data[1][0] * in.m_data[3][3]
				+ in.m_data[0][3] * in.m_data[1][0] * in.m_data[3][1] - in.m_data[0][3] * in.m_data[1][1] * in.m_data[3][0];
			return ans;
		case 4:	// M34
			ans = in.m_data[0][0] * in.m_data[1][1] * in.m_data[2][3] - in.m_data[0][0] * in.m_data[1][3] * in.m_data[2][1]
				+ in.m_data[0][1] * in.m_data[1][3] * in.m_data[2][0] - in.m_data[0][1] * in.m_data[1][0] * in.m_data[2][3]
				+ in.m_data[0][3] * in.m_data[1][0] * in.m_data[2][1] - in.m_data[0][3] * in.m_data[1][1] * in.m_data[2][0];
			return ans;
		}
	case 4:
		switch (gyo) {
		case 1: // M41
			ans = in.m_data[1][0] * in.m_data[2][1] * in.m_data[3][2] - in.m_data[1][0] * in.m_data[2][2] * in.m_data[3][1]
				+ in.m_data[1][1] * in.m_data[2][2] * in.m_data[3][0] - in.m_data[1][1] * in.m_data[2][0] * in.m_data[3][2]
				+ in.m_data[1][2] * in.m_data[2][0] * in.m_data[3][1] - in.m_data[1][2] * in.m_data[2][1] * in.m_data[3][0];
			return ans;
		case 2:	// M42
			ans = in.m_data[0][0] * in.m_data[2][1] * in.m_data[3][2] - in.m_data[0][0] * in.m_data[2][2] * in.m_data[3][1]
				+ in.m_data[0][1] * in.m_data[2][2] * in.m_data[3][0] - in.m_data[0][1] * in.m_data[2][0] * in.m_data[3][2]
				+ in.m_data[0][2] * in.m_data[2][0] * in.m_data[3][1] - in.m_data[0][2] * in.m_data[2][1] * in.m_data[3][0];
			return ans;
		case 3:	// M43
			ans = in.m_data[0][0] * in.m_data[1][1] * in.m_data[3][2] - in.m_data[0][0] * in.m_data[1][2] * in.m_data[3][1]
				+ in.m_data[0][1] * in.m_data[1][2] * in.m_data[3][0] - in.m_data[0][1] * in.m_data[1][0] * in.m_data[3][2]
				+ in.m_data[0][2] * in.m_data[1][0] * in.m_data[3][1] - in.m_data[0][2] * in.m_data[1][1] * in.m_data[3][0];
			return ans;
		case 4:	// M44
			ans = in.m_data[0][0] * in.m_data[1][1] * in.m_data[2][2] - in.m_data[0][0] * in.m_data[1][2] * in.m_data[2][1]
				+ in.m_data[0][1] * in.m_data[1][2] * in.m_data[2][0] - in.m_data[0][1] * in.m_data[1][0] * in.m_data[2][2]
				+ in.m_data[0][2] * in.m_data[1][0] * in.m_data[2][1] - in.m_data[0][2] * in.m_data[1][1] * in.m_data[2][0];
			return ans;
		}
	}
	return ans;
}

TARGET double matrix_d(const matrix4cuda& in)
{
	double	ans = 0;

	switch (in.m_Height)	{
	case 2:
		ans = in.m_data[0][0] * in.m_data[1][1]
			- in.m_data[0][1] * in.m_data[1][0];
		return ans;
	case 3:
		ans = in.m_data[0][0] * in.m_data[1][1] * in.m_data[2][2]
			- in.m_data[0][0] * in.m_data[1][2] * in.m_data[2][1]
			+ in.m_data[0][1] * in.m_data[1][2] * in.m_data[2][0]
			- in.m_data[0][1] * in.m_data[1][0] * in.m_data[2][2]
			+ in.m_data[0][2] * in.m_data[1][0] * in.m_data[2][1]
			- in.m_data[0][2] * in.m_data[1][1] * in.m_data[2][0];
		return ans;
	case 4:
		ans = in.m_data[0][0] * in.m_data[1][1] * in.m_data[2][2] * in.m_data[3][3]
			- in.m_data[0][0] * in.m_data[1][1] * in.m_data[3][2] * in.m_data[2][3]
			- in.m_data[0][0] * in.m_data[2][1] * in.m_data[1][2] * in.m_data[3][3]
			+ in.m_data[0][0] * in.m_data[2][1] * in.m_data[3][2] * in.m_data[1][3]
			+ in.m_data[0][0] * in.m_data[3][1] * in.m_data[1][2] * in.m_data[2][3]
			- in.m_data[0][0] * in.m_data[3][1] * in.m_data[2][2] * in.m_data[1][3]
			- in.m_data[1][0] * in.m_data[0][1] * in.m_data[2][2] * in.m_data[3][3]
			+ in.m_data[1][0] * in.m_data[0][1] * in.m_data[3][2] * in.m_data[2][3]
			+ in.m_data[1][0] * in.m_data[2][1] * in.m_data[0][2] * in.m_data[3][3]
			- in.m_data[1][0] * in.m_data[2][1] * in.m_data[3][2] * in.m_data[0][3]
			- in.m_data[1][0] * in.m_data[3][1] * in.m_data[0][2] * in.m_data[2][3]
			+ in.m_data[1][0] * in.m_data[3][1] * in.m_data[2][2] * in.m_data[0][3]
			+ in.m_data[2][0] * in.m_data[0][1] * in.m_data[1][2] * in.m_data[3][3]
			- in.m_data[2][0] * in.m_data[0][1] * in.m_data[3][2] * in.m_data[1][3]
			- in.m_data[2][0] * in.m_data[1][1] * in.m_data[0][2] * in.m_data[3][3]
			+ in.m_data[2][0] * in.m_data[1][1] * in.m_data[3][2] * in.m_data[0][3]
			+ in.m_data[2][0] * in.m_data[3][1] * in.m_data[0][2] * in.m_data[1][3]
			- in.m_data[2][0] * in.m_data[3][1] * in.m_data[1][2] * in.m_data[0][3]
			- in.m_data[3][0] * in.m_data[0][1] * in.m_data[1][2] * in.m_data[2][3]
			+ in.m_data[3][0] * in.m_data[0][1] * in.m_data[2][2] * in.m_data[1][3]
			+ in.m_data[3][0] * in.m_data[1][1] * in.m_data[0][2] * in.m_data[2][3]
			- in.m_data[3][0] * in.m_data[1][1] * in.m_data[2][2] * in.m_data[0][3]
			- in.m_data[3][0] * in.m_data[2][1] * in.m_data[0][2] * in.m_data[1][3]
			+ in.m_data[3][0] * in.m_data[2][1] * in.m_data[1][2] * in.m_data[0][3];
		return ans;
	}
	return ans;
}

TARGET matrix4cuda matrix_Inv(const matrix4cuda& in)
{
	matrix4cuda	mat = matrix_matrix(in.m_Width, in.m_Height);

	double	m;
	double	A = matrix_d(in);

	int	gyo, retu;

	for (gyo = 1; gyo <= in.m_Height; gyo++) {
		for (retu = 1; retu <= in.m_Width; retu++) {
			m = matrix_M(in, retu, gyo);
			m = ((gyo + retu) % 2) ? -m : m;
			mat.m_data[gyo - 1][retu - 1] = m / A;
		}
	}

	return mat;
}