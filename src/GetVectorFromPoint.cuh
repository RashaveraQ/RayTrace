#pragma once

struct fsize
{
	float	top;
	float	left;
	float	bottom;
	float	right;
};

struct sp;
struct matrix;

__device__
void GetVectorFromPoint(sp& k, sp& l, int px, int py, const fsize* pView, int cx, int cy, const matrix* pMatrix);

