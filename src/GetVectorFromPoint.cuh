#pragma once

struct fsize
{
	float	top;
	float	left;
	float	bottom;
	float	right;
};

struct Sp;
struct Matrix;

__device__
void GetVectorFromPoint(Sp& k, Sp& l, int px, int py, const fsize* pView, int cx, int cy, const Matrix* pMatrix);

