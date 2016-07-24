#pragma once

#include "node.h"
#include "Selectable.h"
class Vertex;

class Face //: public Selectable
{
	Vertex*	m_pVertexes[3];
public:
	Face(Vertex* pVertex1, Vertex* pVertex2, Vertex* pVertex3);
	virtual ~Face();
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& mat) const;
};

