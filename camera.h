#ifndef __CAMERA_H
#define __CAMERA_H

struct	sp;
class	Node;

class	Camera	: Node
{
	sp	m_Angle;	// 方向
	double	m_sWidth;	// スコープ幅
	double	m_sHeight;	// スコープ高さ
	matrix	m_Trans;	// アフィン補正
public:
	Camera( const sp& angle, double swidth, double sheight );
	

#endif
