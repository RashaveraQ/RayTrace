#include "stdafx.h"
#include "GetVectorFromPoint.h"

void GetVectorFromPoint(sp& k, sp& l, int px, int py, const fsize* pView, int cx, int cy, const matrix* pMatrix)
{
	double rx = (pView->right - pView->left) * px / cx + pView->left;
	double ry = (pView->bottom - pView->top) * py / cx + pView->top;

	k = sp(0.01 * rx / (pView->right - pView->left), 0.01 * ry / (pView->bottom - pView->top), 0.01);
	l = sp(rx, ry, -20);

	matrix m = pMatrix->Inv();
	k = m * (k + l) - m * l;
	l = m * l;
}