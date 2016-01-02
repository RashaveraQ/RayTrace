#include "stdafx.h"
#include <float.h>

NurbsPlane::NurbsPlane(Node* const root, const TCHAR* const Name, const sp Color)
	: NurbsPrimitive(root, Name, Color)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m_ControlVertex[i][j] = sp(-3 + 2 * i, 0, -3 + 2 * j);

	if (!newDeviceNode())
		exit(1);
}

NurbsPlane::NurbsPlane(const NurbsPlane& other)
	: NurbsPrimitive(other)
{
	if (!newDeviceNode())
		exit(1);
}