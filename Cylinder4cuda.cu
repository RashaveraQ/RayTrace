__device__
void GetInfo_Cylinder(const Task& task, const sp& K, const sp& L, Info4cuda& info)
{
	if (L.y < -1) {
		if (K.y <= 0) {
			info.valid = false;
			return;
		}

		float t = -(1 + L.y) / K.y;
		sp	p = K * t + L;

		if (p.x * p.x + p.z * p.z <= 1) {
			info.Cross_x = p.x;
			info.Cross_y = p.y;
			info.Cross_z = p.z;
			info.Vertical_x = 0;
			info.Vertical_y = -1;
			info.Vertical_z = 0;
			info.Distance = t * sqrt(K * K);
			info.isEnter = 1;
//			info.Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info.Material = sp(256 * task.nodeInfo.m_Material.Diffuse.r,
							   256 * task.nodeInfo.m_Material.Diffuse.g,
							   256 * task.nodeInfo.m_Material.Diffuse.b).getMaterial();
			info.nodeInfo = task.nodeInfo;
			info.valid = true;
			return;
		}
	}

	if (L.y > 1) {
		if (K.y >= 0) {
			info.valid = false;
			return;
		}

		float t = (1 - L.y) / K.y;

		sp	p = K*t+L;

		if (p.x * p.x + p.z * p.z <= 1) {
			info.Cross_x = p.x;
			info.Cross_y = p.y;
			info.Cross_z = p.z;
			info.Vertical_x = 0;
			info.Vertical_y = 1;
			info.Vertical_z = 0;
			info.Distance = t * sqrt(K * K);
			info.isEnter = 1;
//			info.Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info.Material = sp(256 * task.nodeInfo.m_Material.Diffuse.r,
							   256 * task.nodeInfo.m_Material.Diffuse.g,
							   256 * task.nodeInfo.m_Material.Diffuse.b).getMaterial();
			info.nodeInfo = task.nodeInfo;
			info.valid = true;
			return;
		}
	}

	float	a, b, c, d, t, t1, t2;

	c = K.x * L.z - K.z * L.x;
	c *= c;
	a = K.x * K.x + K.z * K.z;
	d = a - c;

	if (d < 0) {
		info.valid = false;
		return;
	}

	d = sqrt(d);
	b = -(K.x * L.x + K.z * L.z);

	t1 = (b + d) / a;
	t2 = (b - d) / a;

	info.isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
			info.isEnter = 1;
		} else
			t = t1;
	} else {
		if (t2 > 0)
			t = t2;
		else {
			info.valid = false;
			return;
		}
	}

	sp p = K * t + L;

	if (p.y < -1 || 1 < p.y) {
		info.valid = false;
		return;
	}

	info.isEnter = (-1 > L.y || L.y > 1 || sqrt(L.x*L.x+L.z*L.z) > 1.0);

	info.Cross_x = info.Vertical_x = p.x;
	info.Cross_y = info.Vertical_y = p.y;
	info.Cross_z = info.Vertical_z = p.z;
	info.Vertical_y = 0;
	info.Distance = t * sqrt(K * K);
	info.Material = task.nodeInfo.m_Material;
	info.nodeInfo = task.nodeInfo;
	info.valid = true;
}
