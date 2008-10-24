#ifndef __DOCUDA_H
#define __DOCUDA_H

#include "sp4cuda.h"
#include "matrix4cuda.h"

extern "C" void RunMandelbrot0(unsigned long* dst, const int imageW, const int imageH, const int crunch, const double xOff, const double yOff, const double scale, const uchar4 colors, const int frame, const int animationFrame, const matrix* m, const sp* light);
extern "C" void DoCuda(unsigned long* out, const int imageW, const int imageH, const matrix4cuda* m, const sp4cuda* light);
extern "C" void ClearTask();
extern "C" void AddTask(const Task& task);

#endif // __DOCUDA_H
