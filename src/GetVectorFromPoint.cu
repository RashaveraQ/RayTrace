#include "GetVectorFromPoint.cuh"
#include "Sp.cuh"
#include "Matrix.cuh"

__device__
void GetVectorFromPoint(Sp& k, Sp& l, int px, int py, const fsize* pView, int cx, int cy, const Matrix* pMatrix)
{
	float rx = (pView->right - pView->left) * px / cx + pView->left;
	float ry = (pView->bottom - pView->top) * py / cx + pView->top;

	k = Sp(0.01 * rx / (pView->right - pView->left), 0.01 * ry / (pView->bottom - pView->top), 0.01);
	l = Sp(rx, ry, -20);

	const Matrix& m = *pMatrix;
	k = m * (k + l) - m * l;
	l = m * l;
}