#include "stdafx.h"
#include <float.h>

#define CV_WIDTH 4
#define CV_HEIGHT 4

IMPLEMENT_SERIAL(PolygonPlane, CObject, 1)

PolygonPlane::PolygonPlane(Node* const root, const TCHAR* const Name, const sp Color)
	: PolygonPrimitive(root, ePolygonPlane, Name, CV_WIDTH * CV_HEIGHT, Color)
{
	for (int i = 0; i < CV_WIDTH; i++)
		for (int j = 0; j < CV_HEIGHT; j++)
			m_pVertexes[i + CV_WIDTH * j] = Vertex(sp(-3 + 2 * i, 0, -3 + 2 * j));

	m_NumberOfFaces = 2 * (CV_WIDTH - 1) * (CV_HEIGHT - 1);
	m_pFaces = new Face*[m_NumberOfFaces];
	int n = 0;
	for (int i = 0; i < CV_WIDTH - 1; i++)
		for (int j = 0; j < CV_HEIGHT - 1; j++) {
			m_pFaces[n++] = new Face(m_pVertexes + i     + CV_WIDTH * j,
									 m_pVertexes + i + 1 + CV_WIDTH * j,
									 m_pVertexes + i     + CV_WIDTH * (j + 1));
			m_pFaces[n++] = new Face(m_pVertexes + i + 1 + CV_WIDTH * (j + 1),
									 m_pVertexes + i     + CV_WIDTH * (j + 1),
									 m_pVertexes + i + 1 + CV_WIDTH * j);
		}

	if (!newDeviceNode())
		exit(1);
}

PolygonPlane::PolygonPlane(const PolygonPlane& other)
	: PolygonPrimitive(other)
{
	if (!newDeviceNode())
		exit(1);
}
