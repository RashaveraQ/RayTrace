__device__
void GetInfo_Cube(const Task& task, const sp& K, const sp& L, Info4cuda& info)
{
	info.isEnter = (-1.0 > L.x || L.x > 1.0 || -1.0 > L.y || L.y > 1.0 || -1.0 > L.z || L.z > 1.0);

	if (info.isEnter) {
		//外側から外に向かう場合、
		if ((L.x < -1.0 && K.x <= 0.0) || (1.0 < L.x && 0.0 <= K.x) ||
			(L.y < -1.0 && K.y <= 0.0) || (1.0 < L.y && 0.0 <= K.y) ||
			(L.z < -1.0 && K.z <= 0.0) || (1.0 < L.z && 0.0 <= K.z)) {
			info.valid = false;
			return;
		}
	}

	int		i, j, k;
	double	T[6], t1, t;
	sp		c;

	// 全ての面までの距離を求める。
	T[0] = (1.0 - L.x) / K.x; T[1] = - (1.0 + L.x) / K.x;
	T[2] = (1.0 - L.y) / K.y; T[3] = - (1.0 + L.y) / K.y;
	T[4] = (1.0 - L.z) / K.z; T[5] = - (1.0 + L.z) / K.z;

	// t1 に最大値を代入する。
	for (i = 1, t1 = T[0]; i < 6; i++)
		if (t1 < T[i])
			t1 = T[i];

	// 負の場合、t1 を代入する。
	for (i = 0; i < 6; i++)
		if (T[i] < 0.0)
			T[i] = t1;

	for (k = 0; k < 3; k++)	{
		for (i = 1, j = 0, t= T[0]; i < 6; i++)
			if (t > T[i])
				t = T[i], j = i;

		if (!info.isEnter)
			break;

		c = K * t + L;

		if (-1.0 <= c.x && c.x <= 1.0 && -1.0 <= c.y && c.y <= 1.0 && -1.0 <= c.z && c.z <= 1.0)
			break;

		T[j] = t1;
	}

	if (k == 3) {
		info.valid = false;
		return;
	}

	sp cross = K * t + L;
	info.Cross_x = cross.x;
	info.Cross_y = cross.y;
	info.Cross_z = cross.z;
	info.Distance = t * sqrt(K * K);
	info.Material = task.nodeInfo.m_Material;

	switch (j) {
	case 0:
		info.Vertical_x = 1;
		info.Vertical_y = 0;
		info.Vertical_z = 0;
		break;
	case 1:
		info.Vertical_x = -1;
		info.Vertical_y = 0;
		info.Vertical_z = 0;
		break;
	case 2:
		info.Vertical_x = 0;
		info.Vertical_y = 1;
		info.Vertical_z = 0;
		break;
	case 3:
		info.Vertical_x = 0;
		info.Vertical_y = -1;
		info.Vertical_z = 0;
		break;
	case 4:
		info.Vertical_x = 0;
		info.Vertical_y = 0;
		info.Vertical_z = 1;
		break;
	case 5:
		info.Vertical_x = 0;
		info.Vertical_y = 0;
		info.Vertical_z = -1;
		break;
	}
	info.nodeInfo = task.nodeInfo;
	info.valid = true;
}
