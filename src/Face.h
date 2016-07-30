#pragma once

#include "node.h"
#include "Selectable.h"
class Vertex;

class Face //: public Selectable
{
	Vertex*	m_pVertexes[3];		// 頂点
	sp		m_Normal;			// 法線ベクトル
public:
	Face(Vertex* pVertex1, Vertex* pVertex2, Vertex* pVertex3);
	virtual ~Face();
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const;
	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
};

