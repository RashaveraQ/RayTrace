#ifndef __CAMERA_H
#define __CAMERA_H

struct	sp;
class	Node;

class	Camera	: Node
{
	sp	m_Angle;	// ����
	double	m_sWidth;	// �X�R�[�v��
	double	m_sHeight;	// �X�R�[�v����
	matrix	m_Trans;	// �A�t�B���␳
public:
	Camera( const sp& angle, double swidth, double sheight );
	

#endif
