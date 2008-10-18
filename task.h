#ifndef __TASK_H__
#define __TASK_H__

#include "matrix.h"

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

struct NodeInfo {
	D3DMATERIAL9	m_Material;
	double			m_Reflect ;		// îΩéÀó¶
	double			m_Through ;  	// ìßâﬂó¶
	double			m_Refractive ;	// ã¸ê‹ó¶
};

struct Task {
	node_type	type;
	float		m[4][4];
	NodeInfo	nodeInfo;
	int			gathering_member;
	double		torus_R;
	double		torus_r;
};

#endif // __TASK_H__