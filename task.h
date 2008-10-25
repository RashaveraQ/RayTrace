#ifndef __TASK_H__
#define __TASK_H__

//#include <d3dx9.h>

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

struct NodeInfo {
//	D3DMATERIAL9	m_Material;
	double			m_Color_r;
	double			m_Color_g;
	double			m_Color_b;
	double			m_Reflect ;		// îΩéÀó¶
	double			m_Through ;  	// ìßâﬂó¶
	double			m_Refractive ;	// ã¸ê‹ó¶
};

struct Task {
	node_type	type;
	double		m[4][4];
	NodeInfo	nodeInfo;
	union {
		struct {
			int		member;
		} gathering;
		struct {
			double	R;
			double	r;
		} torus;
	} data;
};

#endif // __TASK_H__