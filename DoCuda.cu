#include <stdio.h>
#include "cutil4win.h"
#include <d3dx9.h>
#include <atltypes.h>

#define TARGET __device__
#include "task.h"
#include "sp.h"
#include "matrix.h"

#ifndef M_PI
#define M_PI (4.0*atan(1.0))
#endif

__constant__ Task cTask[100];
__constant__ int  cTaskIndex;
static int sTaskIndex = 0;

struct Info4cuda
{
	bool			valid;
	D3DMATERIAL9	Material;
	BOOL			isEnter;	// 入り込む
	double			Distance;	// 交点までの距離
	double			Cross_x;	// 交点座標x
	double			Cross_y;	// 交点座標y
	double			Cross_z;	// 交点座標z
	double			Vertical_x;	// 法線ベクトルx
	double			Vertical_y;	// 法線ベクトルy
	double			Vertical_z;	// 法線ベクトルz
	double			Refractive;	// 屈折率
	NodeInfo		nodeInfo;	//
};

#define STACK_SIZE	20

struct Stack {
	int				Index;
	bool			valid		[STACK_SIZE];
	D3DMATERIAL9	Material	[STACK_SIZE];
	BOOL			isEnter		[STACK_SIZE];	// 入り込む
	double			Distance	[STACK_SIZE];	// 交点までの距離
	double			Cross_x		[STACK_SIZE];	// 交点座標x
	double			Cross_y		[STACK_SIZE];	// 交点座標y
	double			Cross_z		[STACK_SIZE];	// 交点座標z
	double			Vertical_x	[STACK_SIZE];	// 法線ベクトルx
	double			Vertical_y	[STACK_SIZE];	// 法線ベクトルy
	double			Vertical_z	[STACK_SIZE];	// 法線ベクトルz
	double			Refractive	[STACK_SIZE];	// 屈折率
	NodeInfo		nodeInfo	[STACK_SIZE];	//
};

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

__device__
void GetInfo_Cone(const Task& task, const sp& K, const sp& L, Info4cuda& info)
{
	if (L.y > 1) {
		if (K.y >= 0) {
			info.valid = false;
			return;
		}
		double t = (1 - L.y) / K.y;
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

	double	a, b, c, d, t, t1, t2;
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

__device__
void GetInfo_Cylinder(const Task& task, const sp& K, const sp& L, Info4cuda& info)
{
	if (L.y < -1) {
		if (K.y <= 0) {
			info.valid = false;
			return;
		}

		double t = -(1 + L.y) / K.y;
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

		double t = (1 - L.y) / K.y;

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

	double	a, b, c, d, t, t1, t2;

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

#include "Torus4cuda.cu"

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

__device__
void GetInfo_Plus(const Task& task, const sp& K, const sp& L, Info4cuda& info, Stack& stack)
{
	Info4cuda	tmp;
	double	l = -1;

	for (int i = 0; i < task.data.gathering.member; i++) {
		int j = --stack.Index;
		if (stack.valid[j]) {
			if (l == -1 || stack.Distance[j] < l) {
				l = stack.Distance[j];
				info.valid		= stack.valid	[j];
				info.Material	= stack.Material[j];
				info.isEnter	= stack.isEnter	[j];	// 入り込む
				info.Distance	= stack.Distance[j];	// 交点までの距離
				info.Cross_x	= stack.Cross_x[j];		// 交点座標x
				info.Cross_y	= stack.Cross_y[j];		// 交点座標y
				info.Cross_z	= stack.Cross_z[j];		// 交点座標z
				info.Vertical_x	= stack.Vertical_x[j];	// 法線ベクトルx
				info.Vertical_y	= stack.Vertical_y[j];	// 法線ベクトルy
				info.Vertical_z	= stack.Vertical_z[j];	// 法線ベクトルz
				info.Refractive	= stack.Refractive[j];	// 屈折率
				info.nodeInfo	= stack.nodeInfo[j];	//
			}
		}
	}

	if (l < 0) {
		info.valid = false;
		return;
	}

	if (info.Material.Diffuse.r < 0)
		info.Material = task.nodeInfo.m_Material;
	info.valid = true;
}

__device__
bool GetInfo2(const sp& K, const sp& L, Info4cuda& info)
{
	Stack stack;
	stack.Index = 0;
	
	for (int idx = 0; idx < cTaskIndex; idx++) {
		Info4cuda	inf;
		
		matrix m;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m.m_data[i][j] = cTask[idx].m[i][j];
			}
		}

		sp L2 = m * L;
		sp K2 = m * (K + L) - L2;

		switch (cTask[idx].type) {
		case SPHERE:
			GetInfo_Sphere(cTask[idx], K2, L2, inf);
			break;
		case PLANE:
			break;
		case PLUS:
			GetInfo_Plus(cTask[idx], K2, L2, inf, stack);
			break;
		case MINUS:
			break;
		case MULTIPLE:
			break;
		case CONE:
			GetInfo_Cone(cTask[idx], K2, L2, inf);
			break;
		case CYLINDER:
			GetInfo_Cylinder(cTask[idx], K2, L2, inf);
			break;
		case TORUS:
			GetInfo_Torus(cTask[idx], K2, L2, inf);
			break;
		case POLYGON:
			break;
		case CUBE:
			GetInfo_Cube(cTask[idx], K2, L2, inf);
			break;
		case TEAPOT:
			break;
		default:
			break;
		}
		
		sp vertical(inf.Vertical_x, inf.Vertical_y, inf.Vertical_z);
		sp cross(inf.Cross_x, inf.Cross_y, inf.Cross_z);
		
		matrix Inv_m = m.Inv();
		vertical = Inv_m * (vertical + cross) - Inv_m * cross;
		cross = Inv_m * cross;
		inf.Distance = (cross - L).abs();
		inf.Refractive = inf.nodeInfo.m_Refractive;
		if (inf.isEnter)
			inf.Refractive = 1 / inf.Refractive;

		inf.Vertical_x = vertical.x;
		inf.Vertical_y = vertical.y;
		inf.Vertical_z = vertical.z;
		inf.Cross_x = cross.x;
		inf.Cross_y = cross.y;
		inf.Cross_z = cross.z;
		
		// inf をスタックに積む。
		stack.valid			[stack.Index] = inf.valid;
		stack.Material		[stack.Index] = inf.Material;
		stack.isEnter		[stack.Index] = inf.isEnter;	// 入り込む
		stack.Distance		[stack.Index] = inf.Distance;	// 交点までの距離
		stack.Cross_x		[stack.Index] = inf.Cross_x;	// 交点座標x
		stack.Cross_y		[stack.Index] = inf.Cross_y;	// 交点座標y
		stack.Cross_z		[stack.Index] = inf.Cross_z;	// 交点座標z
		stack.Vertical_x	[stack.Index] = inf.Vertical_x;	// 法線ベクトルx
		stack.Vertical_y	[stack.Index] = inf.Vertical_y;	// 法線ベクトルy
		stack.Vertical_z	[stack.Index] = inf.Vertical_z;	// 法線ベクトルz
		stack.Refractive	[stack.Index] = inf.Refractive;	// 屈折率
		stack.nodeInfo		[stack.Index] = inf.nodeInfo;	//
		stack.Index++;
		info = inf;
	}

	return info.valid;
}

__device__
sp GetColor(sp K, sp L, const sp& Light)
{
	sp ans(127,127,127);

	enum eMode {
		eNone, eReflect, eThrough
	};
	eMode mode = eNone;

	Info4cuda info;
	
	for (int i = 0; i < 10; i++) {
		
		switch (mode) {
		case eNone:
			break;
		case eReflect:
			// 反射率で色を混ぜる。
			info.Material = (info.nodeInfo.m_Reflect * ans + (1 - info.nodeInfo.m_Reflect) * sp(info.Material)).getMaterial();
			break;
		case eThrough:
			// 透過率で色を混ぜる。
			info.Material = (info.nodeInfo.m_Through * ans + (1 - info.nodeInfo.m_Through) * sp(info.Material)).getMaterial();
			break;
		}

		if (!GetInfo2(K, L, info))
			break;
			
		sp k = K.e();
		sp v(info.Vertical_x, info.Vertical_y, info.Vertical_y);

		v = v.e();
/*
		// 反射率がある場合、
		if (info.nodeInfo.m_Reflect > 0) {
			sp k2 = k - 2 * (v * k) * v;
			sp cross(info.Cross_x, info.Cross_y, info.Cross_z);
			sp l2 = cross + 1E-05 * k2;
			// 反射した視線ベクトルから色を取得。
			K = k2, L = l2, mode = eReflect;
			continue;
		}

		// 透過率がある場合、
		if (info.nodeInfo.m_Through > 0) {
			double r = info.Refractive;
			double i = k * v;
			sp k2 = r * (k -i * v - sqrt(r * r - 1.0 + i * i) * v);
			sp cross(info.Cross_x, info.Cross_y, info.Cross_z);
			sp l2 = cross + 1E-05 * k2;
			// 屈折した視線ベクトルから色を取得。
			K = k2, L = l2, mode = eThrough;
			continue;
		}
*/
		// 光源より色を補正。
		sp vertical(info.Vertical_x, info.Vertical_y, info.Vertical_z);
		
		double	x = -Light.e() * vertical.e();
		x = (x > 0.0) ? x : 0.0;
		double t = 64 + 191 * sin(M_PI / 2 * x);
		double b = 191 * (1 - cos(M_PI / 2 * x));
		
		ans =  (t - b) * sp(info.Material) / 255 + sp(b,b,b);
	}

	return ans;
}

__global__
void kernel(unsigned long* dst, int imageW, int imageH, matrix* pMatrix, sp* pLight)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;
   	double rx = 20.0 * px / imageW - 10.0;
	double ry = 20.0 * py / imageH - 10.0;

	sp k(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
	sp l(rx, ry, -20);
    
	k = *pMatrix * (k + l) - *pMatrix * l;
	l = *pMatrix * l;

	sp c = GetColor(k, l, *pLight);

	if (px <= imageW && py <= imageH) {
		dst[px + py * imageW] = RGB(c.x, c.y, c.z);
	}
}

void DoCuda(unsigned long* out, const int imageW, const int imageH, const matrix* m, const sp* light)
{
	matrix*		pMatrix;
	sp*			pLight;
	unsigned long* d_data;
    unsigned int mem_size = imageW * imageH * sizeof(unsigned long);
    

    CUDA_SAFE_CALL(cudaMalloc((void**)&d_data, mem_size));
	CUDA_SAFE_CALL(cudaMalloc((void**)&pMatrix, sizeof(matrix)));
	CUDA_SAFE_CALL(cudaMalloc((void**)&pLight, sizeof(sp)));

	CUDA_SAFE_CALL(cudaMemcpy(pMatrix, m, sizeof(matrix), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(pLight, light, sizeof(sp), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpyToSymbol<int>(cTaskIndex, &sTaskIndex, sizeof(int)));

    dim3 threads(1,1);
    dim3 grid(488,488);
//    dim3 grid(iDivUp(imageW, 16), iDivUp(imageH, 16));

	// execute the kernel
	kernel<<< grid, threads >>>(d_data, imageW, imageH, pMatrix, pLight);
	CUT_CHECK_ERROR("kernel failed.");

    // copy results from device to host
    CUDA_SAFE_CALL(cudaMemcpy(out, d_data, mem_size, cudaMemcpyDeviceToHost));

    // cleanup memory
    CUDA_SAFE_CALL(cudaFree(d_data));
    CUDA_SAFE_CALL(cudaFree(pMatrix));
    CUDA_SAFE_CALL(cudaFree(pLight));
}

void ClearTask()
{
	sTaskIndex = 0;
	CUDA_SAFE_CALL(cudaMemcpyToSymbol<int>(cTaskIndex, &sTaskIndex, sizeof(int)));
}

void AddTask(const Task& task)
{
	CUDA_SAFE_CALL(cudaMemcpyToSymbol<Task>(cTask[0], &task, sizeof(Task), sTaskIndex * sizeof(Task)));
	sTaskIndex++;
}
