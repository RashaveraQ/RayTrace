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

#include "Sphere4cuda.cu"
#include "Cone4cuda.cu"
#include "Cylinder4cuda.cu"
#include "Torus4cuda.cu"
#include "Cube4cuda.cu"
#include "Plus4cuda.cu"

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
