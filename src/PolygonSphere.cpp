#include "stdafx.h"
#include <float.h>

#define RADIUS 3
#define SUBDIVISIONS_AXIS 20
#define SUBDIVISIONS_HEIGHT 20

IMPLEMENT_SERIAL(PolygonSphere, CObject, 1)

PolygonSphere::PolygonSphere(Node* const root, const TCHAR* const Name, const sp Color)
	: PolygonPrimitive(root, ePolygonSphere, Name, SUBDIVISIONS_AXIS * (SUBDIVISIONS_HEIGHT - 1) + 2, Color)
{
	int nV = 0;
	m_pVertexes[nV++] = Vertex(sp(0, RADIUS, 0));
	for (int h = 1; h < SUBDIVISIONS_HEIGHT; h++) {
		float vy = RADIUS * cos((float)h / (float)SUBDIVISIONS_HEIGHT * M_PI);
		float r = RADIUS * sin((float)h / (float)SUBDIVISIONS_HEIGHT * M_PI);
		for (int a = 0; a < SUBDIVISIONS_AXIS; a++) {
			float th = (float)a / (float)SUBDIVISIONS_AXIS * 2 * M_PI;
			float vx = r * cos(th);
			float vz = r * sin(th);
			m_pVertexes[nV++] = Vertex(sp(vx, vy, vz));
		}
	}
	m_pVertexes[nV] = Vertex(sp(0, -RADIUS, 0));

	m_NumberOfFaces = 2 * SUBDIVISIONS_AXIS * (SUBDIVISIONS_HEIGHT - 1);
	m_pFaces = new Face*[m_NumberOfFaces];
	int n = 0;
	for (int h = 0; h < SUBDIVISIONS_HEIGHT; h++) {
		for (int a = 0; a < SUBDIVISIONS_AXIS - 1; a++) {
			switch (h) {
			case 0:
				m_pFaces[n++] = new Face(m_pVertexes, m_pVertexes + a + 1, m_pVertexes + a);
				break;
			case SUBDIVISIONS_HEIGHT - 1:
				m_pFaces[n++] = new Face(m_pVertexes + a     + SUBDIVISIONS_AXIS * h,
										 m_pVertexes + a + 1 + SUBDIVISIONS_AXIS * h,
										 m_pVertexes + 1     + SUBDIVISIONS_AXIS * (h + 1));
				break;
			default:
				m_pFaces[n++] = new Face(m_pVertexes + a     + SUBDIVISIONS_AXIS * h,
										 m_pVertexes + a + 1 + SUBDIVISIONS_AXIS * h,
										 m_pVertexes + a     + SUBDIVISIONS_AXIS * (h + 1));
				m_pFaces[n++] = new Face(m_pVertexes + a     + SUBDIVISIONS_AXIS * (h + 1),
										 m_pVertexes + a + 1 + SUBDIVISIONS_AXIS * h,
										 m_pVertexes + a + 1 + SUBDIVISIONS_AXIS * (h + 1));
				break;
			}
		}
		switch (h) {
		case 0:
			m_pFaces[n++] = new Face(m_pVertexes, m_pVertexes + 1, m_pVertexes + SUBDIVISIONS_AXIS);
			break;
		case SUBDIVISIONS_HEIGHT - 1:
			m_pFaces[n++] = new Face(m_pVertexes + SUBDIVISIONS_AXIS + SUBDIVISIONS_AXIS * h,
									 m_pVertexes + 1                 + SUBDIVISIONS_AXIS * h,
									 m_pVertexes + nV);
			break;
		default:
			m_pFaces[n++] = new Face(m_pVertexes + SUBDIVISIONS_AXIS + SUBDIVISIONS_AXIS * h,
									 m_pVertexes + 1                 + SUBDIVISIONS_AXIS * h,
									 m_pVertexes + SUBDIVISIONS_AXIS + SUBDIVISIONS_AXIS * (h + 1));
			m_pFaces[n++] = new Face(m_pVertexes + SUBDIVISIONS_AXIS + SUBDIVISIONS_AXIS * (h + 1),
									 m_pVertexes + 1                 + SUBDIVISIONS_AXIS * h,
									 m_pVertexes + 1                 + SUBDIVISIONS_AXIS * (h + 1));
			break;
		}
	}

	if (!newDeviceNode())
		exit(1);
}

PolygonSphere::PolygonSphere(const PolygonSphere& other)
	: PolygonPrimitive(other)
{
	if (!newDeviceNode())
		exit(1);
}
