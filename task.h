#ifndef __TASK_H__
#define __TASK_H__

#include "matrix.h"

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

struct Task {
	node_type	type;
	float		m[4][4];
	int			member;
};

#endif // __TASK_H__