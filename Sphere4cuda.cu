
extern "C" __device__
Info4cuda GetInfo_Sphere(Task task, sp4cuda K, sp4cuda L)
{
	Info4cuda info;

	float	a = sp_internal_multiple(K, K);
	float	b = sp_internal_multiple(K, L);
	float	c = sp_internal_multiple(L, L) - 1.0; 

	float	bb_ac = b * b - a * c;

	if (bb_ac < 0) {
		info.valid = false;
		return info;
	}

	float	t;
	float	t1 = (-b + sqrtf(bb_ac)) / a;
	float	t2 = (-b - sqrtf(bb_ac)) / a;

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
			return info;
		}
	}

	sp4cuda cross = sp_plus(sp_multiple(K, t), L);
	info.Cross_x = info.Vertical_x = cross.x;
	info.Cross_y = info.Vertical_y = cross.y;
	info.Cross_z = info.Vertical_z = cross.z;
	
	info.Distance = t * sqrtf(sp_internal_multiple(K,K));

/*
	float x, y, z, th, phy;
	x = info.Vertical_x;
	y = info.Vertical_y;
	z = info.Vertical_z;
	float th, phy
	th = atan2(y, sqrt(x*x+z*z)) / M_PI + .5;
	phy = atan2(x, -z) / (2 * M_PI) + .5;
	info.Material = GetPixel(phy, th).getMaterial();
*/
//	info.Material = sp_getMaterial(sp_sp(256 * task.nodeInfo.m_Material.Diffuse.r, 256 * task.nodeInfo.m_Material.Diffuse.g, 256 * task.nodeInfo.m_Material.Diffuse.b));
	info.Color_r = task.nodeInfo.m_Color_r;
	info.Color_g = task.nodeInfo.m_Color_g;
	info.Color_b = task.nodeInfo.m_Color_b;
	info.nodeInfo = task.nodeInfo;
	info.valid = true;
	
	return info;
}
