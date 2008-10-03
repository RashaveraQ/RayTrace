#ifndef __TASK_H__
#define __TASK_H__

#include "matrix.h"

struct Task {
	const Task*	next;
	node_type	type;
	matrix		Matrix;
	union {
		struct {
			int member;
		} gathering;
	};
	Task() : next(0) {}
};

#endif // __TASK_H__