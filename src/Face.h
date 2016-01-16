#pragma once

#include "node.h"
#include "Selectable.h"
class Vertex;

class Face : public Selectable
{
	int		m_Count;
	Vertex** m_pVertexes;
public:
	Face(int count, Vertex** pVertexes);
	virtual ~Face();
};

