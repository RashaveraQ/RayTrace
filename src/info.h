﻿#ifndef __INFO_H
#define __INFO_H

struct	sp;
class	Node;
class	Selectable;

struct Info
{
	D3DMATERIAL9		Material;
	float				Distance;		// 交点までの距離
	sp					Cross;			// 交点座標
	sp					Vertical;		// 法線ベクトル
	const Node*			pNode;			// 
	const Selectable*	pSelectable;	// 
	Info()
	{
		pSelectable = NULL;
		pNode = NULL;
	}
};

#endif
