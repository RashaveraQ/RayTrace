#ifndef __INFO_H
#define __INFO_H

struct	sp;
class	Node;

struct Info
{
	D3DMATERIAL9	Material;
	BOOL			isEnter;	// 入り込む
	double			Distance;	// 交点までの距離
	sp				Cross;		// 交点座標
	sp				Vertical;	// 法線ベクトル
	const Node *	pNode;		// 
	double	Refractive;			// 屈折率
	TARGET Info()
	{
		pNode = NULL;
	}
};

#endif
