#define TARGET __device__ __host__
#include "Solve_Polynomial.inc"

__device__
void GetInfo_Torus(const Task& task, const sp& K, const sp& L, Info4cuda& info)
{
	int Solve_Polynomial(int d, double *k, double min, double max, double *r);

	double m_R = task.data.torus.R;
	double m_r = task.data.torus.r;

	double	R2 = m_R * m_R;
	double	a = K * K;
	double	b = 2 * (K * L);
	double	c = L * L + R2 - m_r * m_r;

	double	k[20], r[50];
	int		n;

	k[4] = a * a;
	k[3] = 2 * a * b;
	k[2] = 2 * a * c + b * b - 4 * R2 * (K.x * K.x + K.y * K.y);
	k[1] = 2 * b * c - 8 * R2 * (K.x * L.x + K.y * L.y);
	k[0] = c * c - 4 * R2 * (L.x * L.x + L.y * L.y);

	n = Solve_Polynomial(4, k, 0.0, 2000, r);

	if (n == 0 || fabs(r[0]) < 1.0) {
		info.valid = false;
		return;
	}

	double	th;

	double d = m_R - sqrt(L.x * L.x + L.y * L.y);
	d *= d;

	info.isEnter = (m_r * m_r <= d + L.z * L.z);
	
	sp p = K * r[0] + L;
	info.Cross_x = p.x;
	info.Cross_y = p.y;
	info.Cross_z = p.z;
	
	if (p.x == 0.0) {
		info.Vertical_x = 0;
		info.Vertical_y = p.y - ((p.y > 0) ? 1 : -1) * m_R;
		info.Vertical_z = p.z;
	} else {
		th = atan2(p.y , p.x);
		info.Vertical_x = p.x - m_R * cos(th);
		info.Vertical_y = p.y - m_R * sin(th);
		info.Vertical_z = p.z;
	}

	info.Distance = r[0] * sqrt(K * K);

	double x,y,z, phy;

	x = info.Vertical_x;
	y = info.Vertical_y;
	z = info.Vertical_z;

	th = acos(y) / (2 * M_PI); if (x < 0) th = 1 - th;
	phy = acos(z / sqrt(x * x + z * z)) / (2 * M_PI);
	if (x < 0)
		phy = 1 - phy;

	//info.Material = GetPixel(phy, th).getMaterial();
	info.Material = sp(256 * task.nodeInfo.m_Material.Diffuse.r,
					   256 * task.nodeInfo.m_Material.Diffuse.g,
					   256 * task.nodeInfo.m_Material.Diffuse.b).getMaterial();
	info.nodeInfo = task.nodeInfo;

	info.valid = true;
}

