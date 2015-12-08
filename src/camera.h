#ifndef __CAMERA_H
#define __CAMERA_H

struct	sp;
class	Node;

class	Camera	: Node
{
	sp	m_Angle;	// 方向
	float	m_sWidth;	// スコープ幅
	float	m_sHeight;	// スコープ高さ
	matrix	m_Trans;	// アフィン補正
public:
	Camera(const sp& angle, float swidth, float sheight);
	

#endif
