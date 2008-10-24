#include <stdio.h>
#include "cutil4win.h"
//#include <atltypes.h>

#define TARGET __device__ __host__
#include "task.h"
#include "sp4cuda.cpp"
#include "matrix4cuda.cpp"

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
//#include "Cone4cuda.cu"
//#include "Cylinder4cuda.cu"
//#include "Torus4cuda.cu"
//#include "Cube4cuda.cu"
#include "Plus4cuda.cu"

__device__
bool GetInfo2(const sp4cuda& K, const sp4cuda& L, Info4cuda& info)
{
	Stack stack;
	stack.Index = 0;
	
	for (int idx = 0; idx < cTaskIndex; idx++) {
		Info4cuda	inf;
		matrix4cuda m = matrix_matrix(4,4,cTask[idx].m);

		sp4cuda L2 = sp_sp(matrix_multiple(m, matrix_matrix(L)));
		sp4cuda K2 = sp_minus(sp_sp(matrix_multiple(m, matrix_matrix(sp_plus(K,L)))), L2);

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
			//GetInfo_Cone(cTask[idx], K2, L2, inf);
			break;
		case CYLINDER:
			//GetInfo_Cylinder(cTask[idx], K2, L2, inf);
			break;
		case TORUS:
			//GetInfo_Torus(cTask[idx], K2, L2, inf);
			break;
		case POLYGON:
			break;
		case CUBE:
			//GetInfo_Cube(cTask[idx], K2, L2, inf);
			break;
		case TEAPOT:
			break;
		default:
			break;
		}
		
		sp4cuda vertical = sp_sp(inf.Vertical_x, inf.Vertical_y, inf.Vertical_z);
		sp4cuda cross = sp_sp(inf.Cross_x, inf.Cross_y, inf.Cross_z);
		
		matrix4cuda Inv_m = matrix_Inv(m);
		vertical = sp_sp(matrix_minus(matrix_multiple(Inv_m, matrix_matrix(sp_plus(vertical, cross))),
						matrix_multiple(Inv_m, matrix_matrix(cross))));
		cross = sp_sp(matrix_multiple(Inv_m, matrix_matrix(cross)));

		inf.Distance = sp_abs(sp_minus(cross, L));
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
sp4cuda GetColor(sp4cuda K, sp4cuda L, const sp4cuda& Light)
{
	sp4cuda ans = sp_sp(127,127,127);

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
			info.Material = sp_getMaterial(sp_plus(
				sp_multiple(info.nodeInfo.m_Reflect, ans),
				sp_multiple(1 - info.nodeInfo.m_Reflect, sp_sp(info.Material))));
			break;
		case eThrough:
			// 透過率で色を混ぜる。
			info.Material = sp_getMaterial(sp_plus(
				sp_multiple(info.nodeInfo.m_Through, ans),
				sp_multiple(1 - info.nodeInfo.m_Through, sp_sp(info.Material))));
			break;
		}

		if (!GetInfo2(K, L, info))
			break;
			
		sp4cuda k = sp_e(K);
		sp4cuda v = sp_sp(info.Vertical_x, info.Vertical_y, info.Vertical_y);

		v = sp_e(v);
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
		sp4cuda vertical = sp_sp(info.Vertical_x, info.Vertical_y, info.Vertical_z);
		
		double	x = -sp_multiple(sp_e(Light), sp_e(vertical));
		x = (x > 0.0) ? x : 0.0;
		double t = 64 + 191 * sin(M_PI / 2 * x);
		double b = 191 * (1 - cos(M_PI / 2 * x));
		
		ans = sp_plus(sp_divide(sp_multiple(t - b, sp_sp(info.Material)), 255), sp_sp(b,b,b));
	}

	return ans;
}

__global__
void kernel(unsigned long* dst, int imageW, int imageH, matrix4cuda* pMatrix, sp4cuda* pLight)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;

	if (px < imageW && py < imageH) {
   		double rx = 20.0 * px / imageW - 10.0;
		double ry = 20.0 * py / imageH - 10.0;

		sp4cuda k = sp_sp(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
		sp4cuda l = sp_sp(rx, ry, -20);
	    
		k = sp_sp(matrix_minus(matrix_multiple(*pMatrix, matrix_matrix(sp_plus(k, l))), matrix_multiple(*pMatrix, matrix_matrix(l))));
		l = sp_sp(matrix_multiple(*pMatrix, matrix_matrix(l)));

		sp4cuda c = GetColor(k, l, *pLight);

		dst[px + py * imageW] = RGB(c.x, c.y, c.z);
	}
}

// Increase the grid size by 1 if the image width or height does not divide evenly
// by the thread block dimensions
inline int iDivUp(int a, int b)
{
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
} // iDivUp

extern "C"
void DoCuda(unsigned long* out, const int imageW, const int imageH, const matrix4cuda* m, const sp4cuda* light)
{
	matrix4cuda*		pMatrix;
	sp4cuda*			pLight;
	unsigned long* d_data;
    unsigned int mem_size = imageW * imageH * sizeof(unsigned long);
    
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_data, mem_size));
	CUDA_SAFE_CALL(cudaMalloc((void**)&pMatrix, sizeof(matrix4cuda)));
	CUDA_SAFE_CALL(cudaMalloc((void**)&pLight, sizeof(sp4cuda)));

	CUDA_SAFE_CALL(cudaMemcpy(pMatrix, m, sizeof(matrix4cuda), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(pLight, light, sizeof(sp4cuda), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpyToSymbol<int>(cTaskIndex, &sTaskIndex, sizeof(int)));

    dim3 threads(16,16);
//    dim3 grid(488,488);
    dim3 grid(iDivUp(imageW, 16), iDivUp(imageH, 16));

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

extern "C"
void ClearTask()
{
	sTaskIndex = 0;
	CUDA_SAFE_CALL(cudaMemcpyToSymbol<int>(cTaskIndex, &sTaskIndex, sizeof(int)));
}

extern "C"
void AddTask(const Task& task)
{
	CUDA_SAFE_CALL(cudaMemcpyToSymbol<Task>(cTask[0], &task, sizeof(Task), sTaskIndex * sizeof(Task)));
	sTaskIndex++;
}

#if 0
// The dimensions of the thread block
#define BLOCKDIM_X 16
#define BLOCKDIM_Y 16

#define ABS(n) ((n) < 0 ? -(n) : (n))

// The Mandelbrot CUDA GPU thread function
template<class T>
__global__ void Mandelbrot0(unsigned long* dst, const int imageW, const int imageH, const int crunch, const T xOff, const T yOff, const T scale, const uchar4 colors, const int frame, const int animationFrame, matrix4cuda* pMatrix, sp4cuda* pLight)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;

	if (px < imageW && py < imageH) {
   		double rx = 20.0 * px / imageW - 10.0;
		double ry = 20.0 * py / imageH - 10.0;

		sp4cuda k = sp_sp(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
		sp4cuda l = sp_sp(rx, ry, -20);
	    
		k = sp_sp(matrix_minus(matrix_multiple(*pMatrix, matrix_matrix(sp_plus(k, l))), matrix_multiple(*pMatrix, matrix_matrix(l))));
		l = sp_sp(matrix_multiple(*pMatrix, matrix_matrix(l)));

		sp4cuda c = GetColor(k, l, *pLight);

		dst[px + py * imageW] = RGB(c.x, c.y, c.z);
	}
} // Mandelbrot0


// The host CPU Mandebrot thread spawner
void RunMandelbrot0(unsigned long* out, const int imageW, const int imageH, const int crunch, const double xOff, const double yOff, const double scale, const uchar4 colors, const int frame, const int animationFrame, const matrix4cuda* m, const sp4cuda* light)
{
	matrix4cuda*		pMatrix;
	sp4cuda*			pLight;
	unsigned long* d_data;
    unsigned int mem_size = imageW * imageH * sizeof(unsigned long);
    
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_data, mem_size));
	CUDA_SAFE_CALL(cudaMalloc((void**)&pMatrix, sizeof(matrix4cuda)));
	CUDA_SAFE_CALL(cudaMalloc((void**)&pLight, sizeof(sp4cuda)));

	CUDA_SAFE_CALL(cudaMemcpy(pMatrix, m, sizeof(matrix4cuda), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(pLight, light, sizeof(sp4cuda), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpyToSymbol<int>(cTaskIndex, &sTaskIndex, sizeof(int)));

    dim3 threads(BLOCKDIM_X, BLOCKDIM_Y);
    dim3 grid(iDivUp(imageW, BLOCKDIM_X), iDivUp(imageH, BLOCKDIM_Y));

	Mandelbrot0<float><<<grid, threads>>>(out, imageW, imageH, crunch, (float)xOff, (float)yOff, (float)scale, colors, frame, animationFrame, pMatrix, pLight);
    CUT_CHECK_ERROR("Mandelbrot kernel execution failed.\n");

    // copy results from device to host
    CUDA_SAFE_CALL(cudaMemcpy(out, d_data, mem_size, cudaMemcpyDeviceToHost));

    // cleanup memory
    CUDA_SAFE_CALL(cudaFree(d_data));
    CUDA_SAFE_CALL(cudaFree(pMatrix));
    CUDA_SAFE_CALL(cudaFree(pLight));
} // RunMandelbrot0

#endif