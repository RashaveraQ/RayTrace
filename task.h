#ifndef __TASK_H__
#define __TASK_H__

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

struct NodeInfo {
	float			m_Color_r;
	float			m_Color_g;
	float			m_Color_b;
	float			m_Reflect ;		// îΩéÀó¶
	float			m_Through ;  	// ìßâﬂó¶
	float			m_Refractive ;	// ã¸ê‹ó¶
};

struct Task {
	node_type	type;
	float		m[4][4];
	NodeInfo	nodeInfo;
	union {
		struct {
			int		member;
		} gathering;
		struct {
			float	R;
			float	r;
		} torus;
	} data;
};

extern int taskIndex_;
extern Task task_[100];

#endif // __TASK_H__