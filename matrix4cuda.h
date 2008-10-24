#ifndef __MATRIX4CUDA_H
#define __MATRIX4CUDA_H

struct	matrix4cuda
{
	int		m_Width;
	int		m_Height;
	double	m_data[4][4];
};

#include "sp4cuda.h"

TARGET matrix4cuda matrix_matrix(int gyo, int retu);
TARGET matrix4cuda matrix_matrix(const matrix4cuda& Matrix);
TARGET matrix4cuda matrix_matrix(const int w, const int h, const double d[4][4]);
TARGET matrix4cuda matrix_matrix(const sp4cuda& Sp, double d = 1);
TARGET matrix4cuda matrix_minus(const matrix4cuda& lhs, const matrix4cuda& rhs);
TARGET matrix4cuda matrix_plus(const matrix4cuda& lhs, const matrix4cuda& rhs);
TARGET matrix4cuda matrix_multiple(const matrix4cuda& lhs, double k);
TARGET matrix4cuda matrix_divide(const matrix4cuda& lhs, double k);
TARGET matrix4cuda matrix_multiple(const matrix4cuda& lhs, const matrix4cuda& rhs);
TARGET double matrix_M(const matrix4cuda& in, int gyo, int retu);
TARGET double matrix_d(const matrix4cuda& in);
TARGET matrix4cuda matrix_Inv(const matrix4cuda& in);

#endif // __MATRIX4CUDA_H
