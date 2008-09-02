#include <cutil.h>
#include <cuda_runtime.h>
#include <d3dx9.h>
#include <atltypes.h>

#define TARGET __device__
#include "matrix.h"
#include "info.h"

#ifndef M_PI
#define M_PI (4.0*atan(1.0))
#endif

#include "BaseNode.h"
#include "BaseNode.cpp"


__device__ matrix		Matrix;
__device__ BaseNode*	Root;
__device__ sp			Light;

// 視線ベクトル(Kt+L)から色を返す。
TARGET sp BaseNode::GetColor(const sp* K, const sp* L, int nest)
{
	Info	info;

	// 再帰数が１０を越える又は、交点が存在しない場合、
	if (nest > 10 || !GetInfo2(K, L, &info))
		return sp(127, 127, 127);

	sp k = K->e();
	sp v = info.Vertical.e();

	// 反射率がある場合、
	if (info.pNode->m_Reflect > 0) {
		sp k2 = k - 2 * (v * k) * v;
		sp l2 = info.Cross + 1E-05 * k2;
		// 反射した視線ベクトルから色を取得。
		sp c = Root->GetColor(&k2, &l2, nest + 1);
		// 反射率で色を混ぜる。
		info.Material = (info.pNode->m_Reflect * c + (1 - info.pNode->m_Reflect) * sp(info.Material)).getMaterial();
	}

	// 透過率がある場合、
	if (info.pNode->m_Through > 0) {
		double r = info.Refractive;
		double i = k * v;
		sp k2 = r * (k -i * v - sqrt(r * r - 1.0 + i * i) * v);
		sp l2 = info.Cross + 1E-05 * k2;
		// 屈折した視線ベクトルから色を取得。
		sp c = Root->GetColor(&k2, &l2, nest + 1);
		// 透過率で色を混ぜる。
		info.Material = (info.pNode->m_Through * c + (1 - info.pNode->m_Through) * sp(info.Material)).getMaterial();
	}

	// 光源より色を補正。
	double	x = -Light.e() * info.Vertical.e();
	x = (x > 0.0) ? x : 0.0;
	double t = 64 + 191 * sin(M_PI / 2 * x);
	double b = 191 * (1 - cos(M_PI / 2 * x));

	return (t - b) * sp(info.Material) / 255 + sp(b,b,b);
}

__global__ void kernel(unsigned long* dst, const int imageW, const int imageH)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;
   	double rx = 20.0 * px / imageW - 10.0;
	double ry = 20.0 * py / imageH - 10.0;

	sp k(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
	sp l(rx, ry, -20);
    
	k = Matrix * (k + l) - Matrix * l;
	l = Matrix * l;

	sp c = Root->GetColor(&k, &l, 0);
	
	if (px <= imageW && py <= imageH) {
		dst[px + py * imageW] = RGB(c.x, c.y, c.z);
	}
}

void DoCuda(unsigned long* out, class BaseNode* root, const int imageW, const int imageH, const matrix* m, const sp* light)
{
	cudaError_t err;

	unsigned long* d_data;
    unsigned int mem_size = imageW * imageH * sizeof(unsigned long);
	if (cudaSuccess != (err = cudaMalloc((void**)&d_data, mem_size))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMalloc:1", MB_OK);
		RaiseException(0,0,0,0);
		return;
	}

    if (cudaSuccess != (err = cudaMemcpy(&Root, &root, sizeof(BaseNode*), cudaMemcpyHostToDevice))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy:1", MB_OK);
		RaiseException(0,0,0,0);
	}

    if (cudaSuccess != (err = cudaMemcpy(&Matrix, m, sizeof(matrix), cudaMemcpyHostToDevice))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy:1", MB_OK);
		RaiseException(0,0,0,0);
	}

    if (cudaSuccess != (err = cudaMemcpy(&Light, light, sizeof(sp), cudaMemcpyHostToDevice))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy:2", MB_OK);
		RaiseException(0,0,0,0);
	}
	
    dim3 threads(16,16);
    dim3 grid(16,16);
 
	// execute the kernel
	kernel<<< grid, threads >>>(d_data, imageW, imageH);

    if (cudaSuccess != cudaGetLastError()) {
		MessageBox(0, cudaGetErrorString(err), "cudaGetLastError", MB_OK);
		RaiseException(0,0,0,0);
		return;
	}
	
    if (cudaSuccess != (err = cudaThreadSynchronize())) {
		MessageBox(0, cudaGetErrorString(err), "cudaThreadSynchronize", MB_OK);
		RaiseException(0,0,0,0);
    	return;
    }

    // copy results from device to host
    if (cudaSuccess != (err = cudaMemcpy(out, d_data, mem_size, cudaMemcpyDeviceToHost))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy:2", MB_OK);
		RaiseException(0,0,0,0);
	}

    // cleanup memory
    if (cudaSuccess != (err = cudaFree(d_data))) {
		MessageBox(0, cudaGetErrorString(err), "cudaFree", MB_OK);
		RaiseException(0,0,0,0);
		return;
	}
}

