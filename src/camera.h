#ifndef __CAMERA_H
#define __CAMERA_H

struct	sp;
class	Node;

class	Camera	: Node
{
	sp	m_Angle;	// ����
	float	m_sWidth;	// �X�R�[�v��
	float	m_sHeight;	// �X�R�[�v����
	matrix	m_Trans;	// �A�t�B���␳
public:
	Camera(const sp& angle, float swidth, float sheight);
	

#endif
