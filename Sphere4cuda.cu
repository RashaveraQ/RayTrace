__device__
void GetInfo_Sphere(const Task& task, const sp& K, const sp& L, Info4cuda& info)
{
	double	a = K * K;
	double	b = K * L;
	double	c = L * L - 1.0; 

	double	bb_ac = b*b - a*c;

	if (bb_ac < 0) {
		info.valid = false;
		return;
	}

	double	t;
	double	t1 = (-b + sqrt(bb_ac)) / a;
	double	t2 = (-b - sqrt(bb_ac)) / a;

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

	sp cross = K * t + L;
	info.Cross_x = info.Vertical_x = cross.x;
	info.Cross_y = info.Vertical_y = cross.y;
	info.Cross_z = info.Vertical_z = cross.z;
	
	info.Distance = t * sqrt(K * K);

	double x,y,z, th, phy;

	x = info.Vertical_x;
	y = info.Vertical_y;
	z = info.Vertical_z;

	th = atan2(y, sqrt(x*x+z*z)) / M_PI + .5;
	phy = atan2(x, -z) / (2 * M_PI) + .5;

	//info.Material = GetPixel(phy, th).getMaterial();
	info.Material = sp(	256 * task.nodeInfo.m_Material.Diffuse.r,
						256 * task.nodeInfo.m_Material.Diffuse.g,
						256 * task.nodeInfo.m_Material.Diffuse.b).getMaterial();
	info.nodeInfo = task.nodeInfo;
	info.valid = true;
}
