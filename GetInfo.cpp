TARGET BOOL BaseSphere::GetInfo(const sp* K, const sp* L, Info* info)
{
	double	a = (*K) * (*K);
	double	b = (*K) * (*L);
	double	c = (*L) * (*L) - 1.0; 

	double	bb_ac = b*b - a*c;

	if (bb_ac < 0)
		return FALSE;

	double	t;
	double	t1 = (-b + sqrt(bb_ac)) / a;
	double	t2 = (-b - sqrt(bb_ac)) / a;

	info->isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
			info->isEnter = 1;
		} else
			t = t1;
	} else {
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	info->Cross = info->Vertical = (*K) * t + (*L);
	info->Distance = t * sqrt((*K) * (*K));

	double x,y,z, th, phy;

	x = info->Vertical.x;
	y = info->Vertical.y;
	z = info->Vertical.z;

	th = atan2(y, sqrt(x*x+z*z)) / M_PI + .5;
	phy = atan2(x, -z) / (2 * M_PI) + .5;

	info->Material = GetPixel(phy, th).getMaterial();
	info->pNode = this;

	return TRUE;
}

TARGET BOOL BasePlane::GetInfo(const sp* K, const sp* L, Info* info)
{
	double	t = ( K->z ) ? -L->z / K->z : (( L->z > 0 ) ? DBL_MAX : -DBL_MAX);

	if (t <= 0)
		return FALSE;

	info->Cross = (*K) * t + (*L);
	info->Vertical = sp(0, 0, -1);
	info->Distance = t * sqrt((*K) * (*K));
	info->isEnter = (L->z < 0);
	info->Material = GetPixel(info->Cross.x, info->Cross.y).getMaterial();
	info->pNode = this;
	
	return TRUE;
}

TARGET BOOL BasePlus::GetInfo(const sp* K, const sp* L, Info* info)
{
	Info	tmp;
	int		n;
	double	l = -1;

	for (int i = 0; i < m_Member; i++) {
		if (m_Node[i]->GetInfo2(K, L, &tmp)) {
			if (l == -1 || tmp.Distance < l) {
				l = tmp.Distance;
				n = i;
				*info = tmp;
			}
		}
	}

	if (l < 0)
		return FALSE;

	if (info->Material.Diffuse.r < 0)
		info->Material = m_Material;

	return TRUE;
}

TARGET int BaseMinus::cmp_distance(double a, double b)
{
	if (a >= 0) {
		if (b >= 0)
			return (a < b) ? -1 : 1;
		else
			return -1;
	} else {
		if (b >= 0)
			return 1;
		else
			return 0;
	}
}

TARGET BOOL BaseMinus::GetInfo(const sp* K, const sp* L, Info* info)
{
	Info	l_info;
	Info	r_info;
	sp		l = *L;
	int		left, right;

	BOOL	b;

	if (!(m_Member >= 1 && m_Node[0]->GetInfo2(K, &l, &l_info)))
		return FALSE;
	do {
		left  = (m_Member >= 1) ? m_Node[0]->GetInfo2(K, &l, &l_info) : 0;
		right = (m_Member >= 2) ? m_Node[1]->GetInfo2(K, &l, &r_info) : 0;
		
		if (left == 0 && right == 0)
			return FALSE;

		// 左が、右より近い場合。
		if (cmp_distance(l_info.Distance, r_info.Distance) < 0) {
			l = l_info.Cross;
			info->Material = l_info.Material;
		} else {
			l = r_info.Cross;
			info->Material = r_info.Material;
		}

		sp a = m_Matrix * l;
		b = IsInside(&a);
	} while (b == false);

	info->isEnter = 1;
	info->Distance = sqrt(((*L) - l) * ((*L) - l));
	info->Cross = l;

	if (info->Material.Diffuse.r < 0)
		info->Material = m_Material;

	return TRUE;
}

TARGET BOOL BaseMultiple::GetInfo(const sp* K, const sp* L, Info* info)
{
	Info	tmp;
	double	l = -1;
	int		i, j, n = -1;
	BOOL	flag;

	if (!m_Member)
		return FALSE;

	// すべての要素について、
	for (i = 0; i < m_Member; i++)
	{
		// 視点の先に、交点がない場合。
		if (!m_Node[i]->GetInfo2(K, L, &tmp))
			return FALSE;

		if (tmp.Distance <= l || !tmp.isEnter)
			continue;

		info->Cross = tmp.Cross;

		for (flag = TRUE, j = 0; j < m_Member; j++) {
			if (i == j)
				continue;

			sp a = m_Matrix * info->Cross;

			if (!m_Node[j]->IsInside(&a))
				flag = FALSE;
		}

		if (flag)
			l = tmp.Distance, n = i;
	}

	if (n < 0)
		return FALSE;

	m_Node[n]->GetInfo2(K, L, info);

	if (info->Material.Diffuse.r < 0)
		info->Material = m_Material;

	return TRUE;
}

TARGET BOOL BaseCone::GetInfo(const sp* K, const sp* L, Info* info)
{
	if (L->y > 1) {
		if (K->y >= 0)
			return FALSE;

		double t = (1 - L->y) / K->y;

		sp p = (*K) * t + (*L);

		if (p.x * p.x + p.z * p.z <= 1) {
			info->Cross = p;
			info->Vertical = sp(0,1,0);
			info->Distance = t * sqrt((*K) * (*K));
			info->isEnter = 1;
			info->Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info->pNode = this;
			info->Refractive = m_Refractive;

			return TRUE;
		}
	}

	double	a, b, c, d, t, t1, t2;

	c = K->x * L->y - K->y * L->x, c *= c, d = c;
	c = K->z * L->y - K->y * L->z, c *= c, d += c;
	c = K->x * L->z - K->z * L->x, c *= c, d -= c;

	if ( d < 0 )
		return FALSE;

	d = sqrt( d );

	a = -( K->x * L->x + K->z * L->z - K->y * L->y );
	b = K->x * K->x + K->z * K->z - K->y * K->y;

	t1 = (a + d) / b;
	t2 = (a - d) / b;

	if (fabs(t1) < 1E-10 || fabs(t2) < 1E-10)
		return FALSE;

	if (t1 > 0)
	{
		if (t2 > 0)
		{
			if (t1 < t2)
				t = ( K->y * t1 + L->y > 0 ) ? t1 : t2;
			else
				t = ( K->y * t2 + L->y > 0 ) ? t2 : t1;
		}
		else
			t = t1;
	}
	else
	{
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	info->isEnter = !IsInside( L );
	sp p = (*K) * t + (*L);
	info->Cross = info->Vertical = p;

	if (p.y < 0 || p.x*p.x + p.z*p.z > 1) 
		return FALSE;

	info->Vertical.y *= -1;

	info->Distance = t * sqrt((*K) * (*K));
	info->Material = m_Material;
	info->pNode = this;

	return TRUE;
}

TARGET BOOL BaseCylinder::GetInfo(const sp* K, const sp* L, Info* info)
{
	if (L->y < -1)
	{
		if (K->y <= 0)
			return FALSE;

		double t = -(1 + L->y) / K->y;

		sp	p = (*K) * t + (*L);

		if (p.x * p.x + p.z * p.z <= 1) {
			info->Cross = p;
			info->Vertical = sp(0,-1,0);
			info->Distance = t * sqrt((*K) * (*K));
			info->isEnter = 1;
			info->Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info->pNode = this;
			return TRUE;
		}
	}

	if (L->y > 1)
	{
		if (K->y >= 0)
			return FALSE;

		double t = (1 - L->y) / K->y;

		sp	p = (*K) * t + (*L);

		if (p.x * p.x + p.z * p.z <= 1) {
			info->Cross = p;
			info->Vertical = sp(0,1,0);
			info->Distance = t * sqrt((*K) * (*K));
			info->isEnter = 1;
			info->Material = GetPixel(.5*(p.x+1),.5*(p.z+1)).getMaterial();
			info->pNode = this;

			return TRUE;
		}
	}

	double	a, b, c, d, t, t1, t2;

	c = K->x * L->z - K->z * L->x;
	c *= c;
	a = K->x * K->x + K->z * K->z;
	d = a - c;

	if (d < 0)
		return FALSE;

	d = sqrt(d);
	b = -(K->x * L->x + K->z * L->z);

	t1 = (b + d) / a;
	t2 = (b - d) / a;

	info->isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
			info->isEnter = 1;
		} else
			t = t1;
	} else {
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	sp p = (*K) * t + (*L);

	if (p.y < -1 || 1 < p.y)
		return FALSE;

	info->isEnter = !IsInside(L);
	info->Cross = info->Vertical = p;
	info->Vertical.y = 0;
	info->Distance = t * sqrt((*K) * (*K));
	info->Material = m_Material;
	info->pNode = this;

	return TRUE;
}

TARGET BOOL BaseTorus::GetInfo(const sp* K, const sp* L, Info* info)
{
	int Solve_Polynomial(int d, double *k, double min, double max, double *r);

	double	R2 = m_R * m_R;
	double	a = (*K) * (*K);
	double	b = 2 * ((*K) * (*L));
	double	c = (*L) * (*L) + R2 - m_r * m_r;

	double	k[20], r[50];
	int		n;

	k[4] = a * a;
	k[3] = 2 * a * b;
	k[2] = 2 * a * c + b * b - 4 * R2 * (K->x * K->x + K->y * K->y);
	k[1] = 2 * b * c - 8 * R2 * (K->x * L->x + K->y * L->y);
	k[0] = c * c - 4 * R2 * (L->x * L->x + L->y * L->y);

	n = Solve_Polynomial(4, k, 0.0, 2000, r);

	if (n == 0)
		return FALSE;

	if (fabs(r[0]) < 1.0)
		return FALSE;

	sp		p;
	double	th;

	info->isEnter = IsInside(L) == TRUE ? 1 : 0;
	info->Cross = p = (*K) * r[0] + (*L);
	
	if (p.x == 0.0) {
		info->Vertical = sp(0, p.y - ((p.y > 0) ? 1 : -1) * m_R, p.z);
	} else {
		th = atan2( p.y , p.x );
		info->Vertical = sp(p.x - m_R * cos(th), p.y - m_R * sin(th), p.z);
	}

	info->Distance = r[0] * sqrt((*K) * (*K));

	double x,y,z, phy;

	x = info->Vertical.x;
	y = info->Vertical.y;
	z = info->Vertical.z;

	th = acos(y) / (2 * M_PI); if (x < 0) th = 1 - th;
	phy = acos(z / sqrt(x * x + z * z)) / (2 * M_PI);
	if (x < 0)
		phy = 1 - phy;

	info->Material = GetPixel(phy, th).getMaterial();
	info->pNode = this;
	
	return TRUE;
}

TARGET BOOL BasePolygon2::GetInfo(const sp* K, const sp* L, Info* info)
{
	double	a = (*K) * (*K);
	double	b = (*K) * (*L);
	double	c = (*L) * (*L) - 1.0; 

	double	bb_ac = b*b - a*c;

	if (bb_ac < 0)
		return FALSE;

	double	t;
	double	t1 = (-b + sqrt(bb_ac)) / a;
	double	t2 = (-b - sqrt(bb_ac)) / a;

	info->isEnter = 0;
	if (t1 > 0)
	{
		if (t2 > 0)
		{
			info->isEnter = 1;
			t = (t1 < t2) ? t1 : t2;
		}
		else
			t = t1;
	}
	else
	{
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	info->Cross = info->Vertical = (*K) * t + (*L);
	info->Distance = t * sqrt((*K) * (*K));
	info->Material = m_Material;
	info->pNode = this;
	
	return TRUE;
}

TARGET BOOL BaseCube::GetInfo(const sp* K, const sp* L, Info* info)
{
	info->isEnter = !IsInside(L);

	if (info->isEnter)
	{
		//外側から外に向かう場合、
		if ((L->x < -1.0 && K->x <= 0.0) || (1.0 < L->x && 0.0 <= K->x) ||
			(L->y < -1.0 && K->y <= 0.0) || (1.0 < L->y && 0.0 <= K->y) ||
			(L->z < -1.0 && K->z <= 0.0) || (1.0 < L->z && 0.0 <= K->z))
			 return FALSE;
	}

	int		i, j, k;
	double	T[6], t1, t;
	sp		c;

	// 全ての面までの距離を求める。
	T[0] = (1.0 - L->x) / K->x; T[1] = - (1.0 + L->x) / K->x;
	T[2] = (1.0 - L->y) / K->y; T[3] = - (1.0 + L->y) / K->y;
	T[4] = (1.0 - L->z) / K->z; T[5] = - (1.0 + L->z) / K->z;

	// t1 に最大値を代入する。
	for (i = 1, t1 = T[0]; i < 6; i++)
		if (t1 < T[i])
			t1 = T[i];

	// 負の場合、t1 を代入する。
	for (i = 0; i < 6; i++)
		if (T[i] < 0.0)
			T[i] = t1;

	for (k = 0; k < 3; k++)
	{
		for (i = 1, j = 0, t = T[0]; i < 6; i++)
			if (t > T[i])
				t = T[i], j = i;

		if (!info->isEnter)
			break;

		c = (*K) * t + (*L);

		if (-1.0 <= c.x && c.x <= 1.0 && -1.0 <= c.y && c.y <= 1.0 && -1.0 <= c.z && c.z <= 1.0)
			break;

		T[j] = t1;
	}

	if (k == 3)
		return FALSE;

	info->Cross = (*K) * t + (*L);
	info->Distance = t * sqrt((*K) * (*K));
	info->Material = m_Material;

	switch (j)
	{
	case 0:
		info->Vertical = sp(1, 0, 0);
		break;
	case 1:
		info->Vertical = sp(-1, 0, 0);
		break;
	case 2:
		info->Vertical = sp(0, 1, 0);
		break;
	case 3:
		info->Vertical = sp(0,-1, 0);
		break;
	case 4:
		info->Vertical = sp(0, 0, 1);
		break;
	case 5:
		info->Vertical = sp(0, 0,-1);
		break;
	}
	info->pNode = this;

	return TRUE;
}

TARGET BOOL BaseTeapot::GetInfo(const sp* K, const sp* L, Info* info)
{
	double	a = (*K) * (*K);
	double	b = (*K) * (*L);
	double	c = (*L) * (*L) - 1.0; 

	double	bb_ac = b*b - a*c;

	if (bb_ac < 0)
		return FALSE;

	double	t;
	double	t1 = (-b + sqrt(bb_ac)) / a;
	double	t2 = (-b - sqrt(bb_ac)) / a;

	info->isEnter = 0;
	if (t1 > 0) {
		if (t2 > 0) {
			t = (t1 < t2) ? t1 : t2;
			info->isEnter = 1;
		} else
			t = t1;
	} else {
		if (t2 > 0)
			t = t2;
		else
			return FALSE;
	}

	info->Cross = info->Vertical = (*K) * t + (*L);
	info->Distance = t * sqrt((*K) * (*K));

	double x,y,z, th, phy;

	x = info->Vertical.x;
	y = info->Vertical.y;
	z = info->Vertical.z;

	th = atan2(y, sqrt(x*x+z*z)) / M_PI + .5;
	phy = atan2(x, -z) / (2 * M_PI) + .5;

	info->Material = GetPixel(phy, th).getMaterial();
	info->pNode = this;

	return TRUE;
}
