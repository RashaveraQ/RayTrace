#ifndef __INFO_H
#define __INFO_H

struct	sp;
class	Node;

struct Info
{
	D3DMATERIAL9	Material;
	bool			isEnter;	// ���荞��
	float			Distance;	// ��_�܂ł̋���
	sp				Cross;		// ��_���W
	sp				Vertical;	// �@���x�N�g��
	const Node *	pNode;		// 
	float	Refractive;			// ���ܗ�
	Info()
	{
		pNode = NULL;
	}
};

#endif
