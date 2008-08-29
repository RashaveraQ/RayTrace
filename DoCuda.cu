#include <cutil.h>
#include <cuda_runtime.h>
#include <d3dx9.h>
#include <atltypes.h>

#define TARGET __device__
#include "matrix.h"

// The dimensions of the thread block
#define BLOCKDIM_X 16
#define BLOCKDIM_Y 16

__device__ sp GetColor(void* target, const sp* pK, const sp* pL, int nest)
{
	sp c(255, 255 * sin(pL->x), 255 * sin(pL->y));
	return c;
}

__global__ void kernel(unsigned long* dst, const int imageW, const int imageH, void* root, const matrix* m)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;
   	double rx = 20.0 * px / imageW - 10.0;
	double ry = 20.0 * py / imageH - 10.0;

	sp k(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
	sp l(rx, ry, -20);
    
	k = *m * (k + l) - *m * l;
	l = *m * l;

	sp c = GetColor(root, &k, &l, 0);
	
	if (px <= imageW && py <= imageH) {
		dst[px + py * imageW] = RGB(c.x, c.y, c.z);
	}
}

void DoCuda(unsigned long* out, class Node* root, const int imageW, const int imageH, const matrix* m)
{
	cudaError_t err;

	unsigned long* d_data;
    unsigned int mem_size = imageW * imageH * sizeof(unsigned long);
	if (cudaSuccess != (err = cudaMalloc((void**)&d_data, mem_size))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMalloc:1", MB_OK);
		return;
	}

	matrix* d_m;
	if (cudaSuccess != (err = cudaMalloc((void**)&d_m, sizeof(matrix)))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMalloc:2", MB_OK);
		return;
	}	
    if (cudaSuccess != (err = cudaMemcpy(d_m, m, sizeof(matrix), cudaMemcpyHostToDevice))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy:1", MB_OK);
	}
	
    dim3 threads(16,16);
    dim3 grid(16,16);
 
	// execute the kernel
	kernel<<< grid, threads >>>(d_data, imageW, imageH, root, d_m);

    if (cudaSuccess != cudaGetLastError()) {
		MessageBox(0, cudaGetErrorString(err), "cudaGetLastError", MB_OK);
		return;
	}
	
    if (cudaSuccess != (err = cudaThreadSynchronize())) {
		MessageBox(0, cudaGetErrorString(err), "cudaThreadSynchronize", MB_OK);
    	return;
    }

    // copy results from device to host
    if (cudaSuccess != (err = cudaMemcpy(out, d_data, mem_size, cudaMemcpyDeviceToHost))) {
		MessageBox(0, cudaGetErrorString(err), "cudaMemcpy:2", MB_OK);
	}

    // cleanup memory
    if (cudaSuccess != (err = cudaFree(d_data))) {
		MessageBox(0, cudaGetErrorString(err), "cudaFree", MB_OK);
		return;
	}
}

