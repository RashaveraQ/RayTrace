#ifndef __TASK_H__
#define __TASK_H__

#include "matrix.h"

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

struct Task {
	const Task*	next;
	node_type	type;
	matrix		Matrix;
	union {
		struct {
			int member;
		} gathering;
	};
	TARGET Task() : next(0) {}
};

#endif // __TASK_H__