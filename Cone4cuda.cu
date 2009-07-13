__device__
void GetInfo_Cone(const Task& task, const sp& K, const sp& L, Info4cuda& info)
{
	if (L.y > 1) {
		if (K.y >= 0) {
			info.valid = false;
			return;
		}
		float t = (1 - L.y) / K.y;
		sp p = K*t + L;
		if (p.x * p.x + p.z * p.z <= 1) {
			info.Cross_x = p.x;
			info.Cross_y = p.y;
			info.Cross_z = p.z;
			info.Vertical_x = 0;
			info.Vertical_y = 1;
			info.Vertical_z = 0;
			info.Distance = t * sqrt(K*K);
			info.isEnter = 1;
//			info.Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info.Material = sp(256 * task.nodeInfo.m_Material.Diffuse.r,
							   256 * task.nodeInfo.m_Material.Diffuse.g,
							   256 * task.nodeInfo.m_Material.Diffuse.b).getMaterial();
			info.nodeInfo = task.nodeInfo;
			info.Refractive = task.nodeInfo.m_Refractive;
			info.valid = true;
			return;
		}
	}

	float	a, b, c, d, t, t1, t2;
	c = K.x * L.y - K.y * L.x, c *= c, d = c;
	c = K.z * L.y - K.y * L.z, c *= c, d += c;
	c = K.x * L.z - K.z * L.x, c *= c, d -= c;
	if ( d < 0 ) {
		info.valid = false;
		return;
	}
	d = sqrt( d );
	a = -( K.x * L.x + K.z * L.z - K.y * L.y );
	b = K.x * K.x + K.z * K.z - K.y * K.y;
	t1 = ( a + d ) / b;
	t2 = ( a - d ) / b;
	if (fabs(t1) < 1E-10 || fabs(t2) < 1E-10) {
		info.valid = false;
		return;
	}
	if (t1 > 0) {
		if (t2 > 0) {
			if (t1 < t2)
				t = (K.y * t1 + L.y > 0) ? t1 : t2;
			else
				t = (K.y * t2 + L.y > 0) ? t2 : t1;
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

	info.isEnter = (0 > L.y || L.y > 1 || sqrt(L.x * L.x + L.z * L.z) > L.y);
	sp p = K * t + L;
	info.Cross_x = info.Vertical_x = p.x;
	info.Cross_y = info.Vertical_y = p.y;
	info.Cross_z = info.Vertical_z = p.z;

	if (p.y < 0 || p.x * p.x + p.z * p.z > 1) {
		info.valid = false;
		return;
	}

	info.Vertical_y *= -1;

	info.Distance = t * sqrt(K * K);
	info.Material = task.nodeInfo.m_Material;
	info.nodeInfo = task.nodeInfo;
	info.valid = true;
}
