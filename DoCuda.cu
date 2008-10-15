#include <stdio.h>
#include "cutil4win.h"
#include <d3dx9.h>
#include <atltypes.h>

#define TARGET __device__
#include "task.h"
#include "matrix.h"

struct Info
{
	bool			valid;
	D3DMATERIAL9	Material;
	BOOL			isEnter;	// 入り込む
	double			Distance;	// 交点までの距離
	sp				Cross;		// 交点座標
	sp				Vertical;	// 法線ベクトル
	double			Refractive;	// 屈折率
	NodeInfo		nodeInfo;	//
};

struct Stack
{
	Info*	data[100];
	int		index;
};

#ifndef M_PI
#define M_PI (4.0*atan(1.0))
#endif

__constant__ Task cTask[100];
__constant__ int  cTaskIndex;
static int sTaskIndex = 0;

__device__
void GetInfo_Sphere(const Task& task, const sp& K, const sp& L, Info& info)
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

	info.Cross = info.Vertical = K * t + L;
	info.Distance = t * sqrt(K * K);

	double x,y,z, th, phy;

	x = info.Vertical.x;
	y = info.Vertical.y;
	z = info.Vertical.z;

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
bool GetInfo2(const sp& K, const sp& L, Info& info)
{
	Stack	stack;
	stack.index = 0;
	
	// stack.data のメモリ確保をする事。

	
	for (int idx = 0; idx < cTaskIndex; idx++) {
		Info	inf;
		
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

			// inf をスタックに積む。
	
			break;
		case PLANE:
			break;
		case PLUS:
//			GetInfo_Plus(cTask[idx], k, l, inf);
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
	}

	bool ans = stack.data[0]->valid;
	
	// stack.data のメモリを解放する事。
	
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

	Info info;
	
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
		sp v = info.Vertical.e();

		// 反射率がある場合、
		if (info.nodeInfo.m_Reflect > 0) {
			sp k2 = k - 2 * (v * k) * v;
			sp l2 = info.Cross + 1E-05 * k2;
			// 反射した視線ベクトルから色を取得。
			K = k2, L = l2, mode = eReflect;
			continue;
		}

		// 透過率がある場合、
		if (info.nodeInfo.m_Through > 0) {
			double r = info.Refractive;
			double i = k * v;
			sp k2 = r * (k -i * v - sqrt(r * r - 1.0 + i * i) * v);
			sp l2 = info.Cross + 1E-05 * k2;
			// 屈折した視線ベクトルから色を取得。
			K = k2, L = l2, mode = eThrough;
			continue;
		}

		// 光源より色を補正。
		double	x = -Light.e() * info.Vertical.e();
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
