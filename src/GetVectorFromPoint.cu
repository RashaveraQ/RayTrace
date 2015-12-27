#include "GetVectorFromPoint.cuh"
#include "sp.cuh"
#include "matrix.cuh"

__device__
void GetVectorFromPoint(sp& k, sp& l, int px, int py, const fsize* pView, int cx, int cy, const matrix* pMatrix)
{
	float rx = (pView->right - pView->left) * px / cx + pView->left;
	float ry = (pView->bottom - pView->top) * py / cx + pView->top;

	k = sp(0.01 * rx / (pView->right - pView->left), 0.01 * ry / (pView->bottom - pView->top), 0.01);
	l = sp(rx, ry, -20);

	const matrix& m = *pMatrix;
	k = m * (k + l) - m * l;
	l = m * l;
}