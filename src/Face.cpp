#include "StdAfx.h"
#include "Face.h"

Face::Face(int count, Vertex** pVertexes)
	: m_Count(count)
{
	m_pVertexes = new Vertex*[count];
	for (int i = 0; i < count; i++)
		m_pVertexes[i] = pVertexes[i];
}

Face::~Face()
{
	delete m_pVertexes;
}