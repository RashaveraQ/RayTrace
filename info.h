#ifndef __INFO_H
#define __INFO_H

struct	sp;
class	Node;

struct Info
{
	D3DMATERIAL9	Material;
	BOOL			isEnter;	// ���荞��
	double			Distance;	// ��_�܂ł̋���
	sp				Cross;		// ��_���W
	sp				Vertical;	// �@���x�N�g��
	const Node *	pNode;		// 
	double	Refractive;			// ���ܗ�
	TARGET Info()
	{
		pNode = NULL;
	}
};

#endif
