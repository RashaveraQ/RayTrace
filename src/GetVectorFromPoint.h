#pragma once

struct sp;
struct fsize;
class CSize;
struct matrix;

TARGET
void GetVectorFromPoint(sp& k, sp& l, int px, int py, const fsize* pView, int cx, int cy, const matrix* pMatrix);

