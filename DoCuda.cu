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

#define STACK_SIZE	50

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
void GetInfo_Plus(const Task& task, const sp& K, const sp& L, Info4cuda& info, Stack& stack)
{
	Info4cuda	tmp;
	double	l = -1;

	for (int i = 0; i < task.member; i++) {
		int j = stack.Index--;
		//if (m_Node[i]->GetInfo2(K, L, tmp)) {
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

	for (int idx = 0; idx < cTaskIndex; idx++) {
		Info4cuda	inf;
		
		matrix m;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m.m_data[i][j] = cTask[idx].m[i][j];
			}
		}
		matrix Inv_m = m.Inv();

		sp L2 = Inv_m * L;
		sp K2 = Inv_m * (K + L) - L2;

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
			break;
		case CYLINDER:
			break;
		case TORUS:
			break;
		case POLYGON:
			break;
		case CUBE:
			break;
		case TEAPOT:
			break;
		}
		
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
	}

	bool ans = stack.valid[0];

	return ans;
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
//	sp c((imageW * cTaskIndex) % 256, (imageH * cTaskIndex) % 256, 0);

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

    dim3 threads(16,16);
    dim3 grid(16,16);
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
