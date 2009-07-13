#ifndef __MATRIX4CUDA_H
#define __MATRIX4CUDA_H

struct	matrix4cuda
{
	int		m_Width;
	int		m_Height;
	float	m_data[4][4];
};

#include "sp4cuda.h"

extern "C" {
matrix4cuda matrix_matrix(int width, int height, float d[4][4]);
matrix4cuda matrix_matrix1(int gyo, int retu);
matrix4cuda matrix_matrix2(matrix4cuda Matrix);
matrix4cuda matrix_matrix3(sp4cuda Sp, float d = 1);
matrix4cuda matrix_minus(matrix4cuda lhs, matrix4cuda rhs);
matrix4cuda matrix_plus(matrix4cuda lhs, matrix4cuda rhs);
matrix4cuda matrix_scalar_multiple(matrix4cuda lhs, float k);
matrix4cuda matrix_divide(matrix4cuda lhs, float k);
matrix4cuda matrix_multiple(matrix4cuda lhs, matrix4cuda rhs);
float matrix_M(matrix4cuda in, int gyo, int retu);
float matrix_d(matrix4cuda in);
matrix4cuda matrix_Inv(matrix4cuda in);
}

#endif // __MATRIX4CUDA_H
