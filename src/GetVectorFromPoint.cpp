#include "stdafx.h"
#include "GetVectorFromPoint.h"

void GetVectorFromPoint(sp& k, sp& l, int px, int py, const fsize* pView, int cx, int cy, matrix* const pMatrix)
{
	float rx = (pView->right - pView->left) * px / cx + pView->left;
	float ry = (pView->bottom - pView->top) * py / cx + pView->top;

	k = sp(0.01f * rx / (pView->right - pView->left), 0.01f * ry / (pView->bottom - pView->top), 0.01f);
	l = sp(rx, ry, -20);

	matrix& m = *pMatrix;
	k = m * (k + l) - m * l;
	l = m * l;
}