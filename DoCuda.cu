#include <stdio.h>
#include "cutil4win.h"
//#include <atltypes.h>

#include "task.h"
#include "sp4cuda.cpp"
#include "matrix4cuda.cpp"

#ifndef M_PI
#define M_PI (4.0*atan(1.0))
#endif

struct Info4cuda
{
	bool			valid;
	float			Color_r;
	float			Color_g;
	float			Color_b;
	bool			isEnter;	// 入り込む
	float			Distance;	// 交点までの距離
	float			Cross_x;	// 交点座標x
	float			Cross_y;	// 交点座標y
	float			Cross_z;	// 交点座標z
	float			Vertical_x;	// 法線ベクトルx
	float			Vertical_y;	// 法線ベクトルy
	float			Vertical_z;	// 法線ベクトルz
	float			Refractive;	// 屈折率
	NodeInfo		nodeInfo;	//
};

#define STACK_SIZE	5

struct StackData {
	bool			valid;
	float			Color_r;
	float			Color_g;
	float			Color_b;
	bool			isEnter;	// 入り込む
	float			Distance;	// 交点までの距離
	float			Cross_x;	// 交点座標x
	float			Cross_y;	// 交点座標y
	float			Cross_z;	// 交点座標z
	float			Vertical_x;	// 法線ベクトルx
	float			Vertical_y;	// 法線ベクトルy
	float			Vertical_z;	// 法線ベクトルz
	float			Refractive;	// 屈折率
	NodeInfo		nodeInfo;	//
};

struct Stack {
	int			Index;
	StackData	data[5];
};

#include "Sphere4cuda.cu"
//#include "Cone4cuda.cu"
//#include "Cylinder4cuda.cu"
//#include "Torus4cuda.cu"
//#include "Cube4cuda.cu"
#include "Plus4cuda.cu"

extern "C" __global__ 
void kernel(unsigned long* dst, int imageW, int imageH, matrix4cuda* pMatrix, sp4cuda* pLight, int taskIndex, Task* task)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;

	if (px >= imageW || py >= imageH)
		return;

	dst[px + py * imageW] = px + py * imageW;
	
	float rx = 20.0 * px / imageW - 10.0;
	float ry = 20.0 * py / imageH - 10.0;

	sp4cuda K = sp_sp(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
	sp4cuda L = sp_sp(rx, ry, -20);


	K = sp_sp3(matrix_minus(matrix_multiple(*pMatrix, matrix_matrix3(sp_plus(K,L))), matrix_multiple(*pMatrix, matrix_matrix3(L))));
	L = sp_sp3(matrix_multiple(*pMatrix, matrix_matrix3(L)));
	
	sp4cuda Light = *pLight;
	sp4cuda ans = sp_sp(0,0,128);

	enum eMode {
		eNone, eReflect, eThrough
	};
	eMode mode = eNone;

	Info4cuda info;
	info.valid = false;
	
	for (int i = 0; i < 1; i++) {
		
		switch (mode) {
		case eNone:
			break;
		case eReflect:
			// 反射率で色を混ぜる。
			{
			sp4cuda s = sp_plus(
				sp_multiple2(info.nodeInfo.m_Reflect, ans),
				sp_multiple2(1 - info.nodeInfo.m_Reflect, sp_sp(info.Color_r, info.Color_g, info.Color_b)));
			info.Color_r = s.x;
			info.Color_g = s.y;
			info.Color_b = s.z;
			}
			break;
		case eThrough:
			// 透過率で色を混ぜる。
			{
			sp4cuda s = sp_plus(
				sp_multiple2(info.nodeInfo.m_Through, ans),
				sp_multiple2(1 - info.nodeInfo.m_Through, sp_sp(info.Color_r, info.Color_g, info.Color_b)));
			info.Color_r = s.x;
			info.Color_g = s.y;
			info.Color_b = s.z;
			}
			break;
		}

		Stack stack;
		stack.Index = 0;
		for (int idx = 0; idx < taskIndex; idx++) {
			matrix4cuda m = matrix_matrix(4,4,task[idx].m);
			sp4cuda L2 = sp_sp3(matrix_multiple(m, matrix_matrix3(L)));
			sp4cuda K2 = sp_minus(sp_sp3(matrix_multiple(m, matrix_matrix3(sp_plus(K,L)))), L2);

			Info4cuda	inf;
			switch (task[idx].type) {
			case SPHERE:
				inf = GetInfo_Sphere(task[idx], K2, L2);
				break;
			case PLANE:
				break;
			case PLUS:
				inf = GetInfo_Plus(task[idx], K2, L2, &stack);
				stack.Index -= task[idx].data.gathering.member;
				break;
			case MINUS:
				break;
			case MULTIPLE:
				break;
			case CONE:
				//inf = GetInfo_Cone(task[idx], K2, L2);
				break;
			case CYLINDER:
				//inf = GetInfo_Cylinder(task[idx], K2, L2);
				break;
			case TORUS:
				//inf = GetInfo_Torus(task[idx], K2, L2);
				break;
			case POLYGON:
				break;
			case CUBE:
				//inf = GetInfo_Cube(task[idx], K2, L2);
				break;
			case TEAPOT:
				break;
			default:
				break;
			}

			sp4cuda vertical = sp_sp(inf.Vertical_x, inf.Vertical_y, inf.Vertical_z);
			sp4cuda cross = sp_sp(inf.Cross_x, inf.Cross_y, inf.Cross_z);
			matrix4cuda Inv_m = matrix_Inv(m);
			sp4cuda vc = sp_plus(vertical, cross);
			matrix4cuda ivc = matrix_multiple(Inv_m, matrix_matrix3(vc));
			vertical = sp_sp3(matrix_minus(matrix_multiple(Inv_m, matrix_matrix3(vc)),
							matrix_multiple(Inv_m, matrix_matrix3(cross))));
			cross = sp_sp3(matrix_multiple(Inv_m, matrix_matrix3(cross)));
			sp4cuda cL = sp_minus(cross, L);
			float distance = sp_abs(cL);
			float refractive = inf.nodeInfo.m_Refractive;
			if (inf.isEnter)
				refractive = 1 / refractive;
			StackData*	sd = &(stack.data[stack.Index]);
			// inf をスタックに積む。
			sd->valid		= inf.valid;
			sd->Color_r		= inf.Color_r;
			sd->Color_g		= inf.Color_g;
			sd->Color_b		= inf.Color_b;
			sd->isEnter		= inf.isEnter;	// 入り込む
			sd->Distance	= distance;		// 交点までの距離
			sd->Cross_x		= cross.x;		// 交点座標x
			sd->Cross_y		= cross.y;		// 交点座標y
			sd->Cross_z		= cross.z;		// 交点座標z
			sd->Vertical_x	= vertical.x;	// 法線ベクトルx
			sd->Vertical_y	= vertical.y;	// 法線ベクトルy
			sd->Vertical_z	= vertical.z;	// 法線ベクトルz
			sd->Refractive	= refractive;	// 屈折率
			sd->nodeInfo	= inf.nodeInfo;	//
			stack.Index++;
			info = inf;
			return;
// OK return;
		}
// NG return;
		if (!info.valid)
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
			float r = info.Refractive;
			float i = k * v;
			sp k2 = r * (k -i * v - sqrtf(r * r - 1.0 + i * i) * v);
			sp cross(info.Cross_x, info.Cross_y, info.Cross_z);
			sp l2 = cross + 1E-05 * k2;
			// 屈折した視線ベクトルから色を取得。
			K = k2, L = l2, mode = eThrough;
			continue;
		}
*/
		// 光源より色を補正。
		sp4cuda vertical = sp_sp(info.Vertical_x, info.Vertical_y, info.Vertical_z);
		
		float	x = -sp_internal_multiple(sp_e(Light), sp_e(vertical));
		x = (x > 0.0) ? x : 0.0;
		float t = 64 + 191 * sinf(M_PI / 2 * x);
		float b = 191 * (1 - cosf(M_PI / 2 * x));
		
		ans = sp_plus(sp_divide(sp_multiple2(t - b, sp_sp(info.Color_r, info.Color_g, info.Color_b)), 255), sp_sp(b,b,b));
	}

	dst[px + py * imageW] = RGB(ans.x, ans.y, ans.z);
}

// Increase the grid size by 1 if the image width or height does not divide evenly
// by the thread block dimensions
inline int iDivUp(int a, int b)
{
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
} // iDivUp

extern "C"
void DoCuda(unsigned long* out, int imageW, int imageH, matrix4cuda m, sp4cuda light, int taskIndex, Task* task)
{
	matrix4cuda*	dMatrix;
	sp4cuda*		dLight;
	Task*			dTask;
	unsigned long*	d_data;
//	Stack*			dStack;
    unsigned int mem_size = imageW * imageH * sizeof(unsigned long);
 
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_data, mem_size));
	CUDA_SAFE_CALL(cudaMalloc((void**)&dMatrix, sizeof(matrix4cuda)));
	CUDA_SAFE_CALL(cudaMalloc((void**)&dLight, sizeof(sp4cuda)));
	CUDA_SAFE_CALL(cudaMalloc((void**)&dTask, taskIndex * sizeof(Task)));
//	CUDA_SAFE_CALL(cudaMalloc((void**)&dStack, sizeof(Stack)));

	CUDA_SAFE_CALL(cudaMemcpy(d_data, out, mem_size, cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(dMatrix, &m, sizeof(matrix4cuda), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(dLight, &light, sizeof(sp4cuda), cudaMemcpyHostToDevice));
	CUDA_SAFE_CALL(cudaMemcpy(dTask, task, taskIndex * sizeof(Task), cudaMemcpyHostToDevice));

    dim3 threads(16,16);
    dim3 grid(iDivUp(imageW, 16), iDivUp(imageH, 16));

	// execute the kernel
	kernel<<< grid, threads >>>(d_data, imageW, imageH, dMatrix, dLight, taskIndex, dTask);
	CUT_CHECK_ERROR("kernel failed.");

    // copy results from device to host
    CUDA_SAFE_CALL(cudaMemcpy(out, d_data, mem_size, cudaMemcpyDeviceToHost));

    // cleanup memory
    CUDA_SAFE_CALL(cudaFree(d_data));
    CUDA_SAFE_CALL(cudaFree(dMatrix));
    CUDA_SAFE_CALL(cudaFree(dLight));
    CUDA_SAFE_CALL(cudaFree(dTask));
//    CUDA_SAFE_CALL(cudaFree(dStack));
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
   		float rx = 20.0 * px / imageW - 10.0;
		float ry = 20.0 * py / imageH - 10.0;

		sp4cuda k = sp_sp(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
		sp4cuda l = sp_sp(rx, ry, -20);
	    
		k = sp_sp(matrix_minus(matrix_multiple(*pMatrix, matrix_matrix(sp_plus(k, l))), matrix_multiple(*pMatrix, matrix_matrix(l))));
		l = sp_sp(matrix_multiple(*pMatrix, matrix_matrix(l)));

		sp4cuda c = GetColor(k, l, *pLight);

		dst[px + py * imageW] = RGB(c.x, c.y, c.z);
	}
} // Mandelbrot0


// The host CPU Mandebrot thread spawner
void RunMandelbrot0(unsigned long* out, const int imageW, const int imageH, const int crunch, const float xOff, const float yOff, const float scale, const uchar4 colors, const int frame, const int animationFrame, const matrix4cuda* m, const sp4cuda* light)
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