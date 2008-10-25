#ifndef __MATRIX4CUDA_H
#define __MATRIX4CUDA_H

struct	matrix4cuda
{
	int		m_Width;
	int		m_Height;
	double	m_data[4][4];
};

#include "sp4cuda.h"

TARGET matrix4cuda matrix_matrix(int width, int height, double d[4][4]);
TARGET matrix4cuda matrix_matrix1(int gyo, int retu);
TARGET matrix4cuda matrix_matrix2(matrix4cuda Matrix);
TARGET matrix4cuda matrix_matrix3(sp4cuda Sp, double d = 1);
TARGET matrix4cuda matrix_minus(matrix4cuda lhs, matrix4cuda rhs);
TARGET matrix4cuda matrix_plus(matrix4cuda lhs, matrix4cuda rhs);
TARGET matrix4cuda matrix_scalar_multiple(matrix4cuda lhs, double k);
TARGET matrix4cuda matrix_divide(matrix4cuda lhs, double k);
TARGET matrix4cuda matrix_multiple(matrix4cuda lhs, matrix4cuda rhs);
TARGET double matrix_M(matrix4cuda in, int gyo, int retu);
TARGET double matrix_d(matrix4cuda in);
TARGET matrix4cuda matrix_Inv(matrix4cuda in);

#endif // __MATRIX4CUDA_H
