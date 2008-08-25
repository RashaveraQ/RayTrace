#include <cutil.h>
#include <cuda_runtime.h>

#include <d3dx9.h>
#include <atltypes.h>

#define TARGET __device__
#include "NodeBase.h"
#include "NodeBase_kernel.cpp"

// The dimensions of the thread block
#define BLOCKDIM_X 16
#define BLOCKDIM_Y 16

__global__ void kernel(unsigned long* dst, NodeBase* root, const sp& light, const int imageW, const int imageH, const matrix* m)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;
    
   	double rx = 20.0 * px / imageW - 10.0;
	double ry = 20.0 * py / imageH - 10.0;

	sp k(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
	sp l(rx, ry, -20);

	k = *m * (k + l) - *m * l;
	l = *m * l;
 
	sp c = root->GetColor(root, light, k, l, 0);
	
	dst[px + py * imageW] = RGB(c.x, c.y, c.z);
}

// Increase the grid size by 1 if the image width or height does not divide evenly
// by the thread block dimensions
inline int iDivUp(int a, int b)
{
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
} // iDivUp

void DoCuda(unsigned long* out, NodeBase* root, const sp& light, const int imageW, const int imageH, const matrix* m)
{
	unsigned long* d_data;
    const unsigned int mem_size = imageW * imageH * sizeof(unsigned long);

	CUDA_SAFE_CALL(cudaMalloc((void**)&d_data, mem_size));
	
    dim3 threads(BLOCKDIM_X, BLOCKDIM_Y);
    dim3 grid(iDivUp(imageW, BLOCKDIM_X), iDivUp(imageH, BLOCKDIM_Y));
 
	// execute the kernel
	kernel<<< grid, threads >>>(d_data, root, light, imageW, imageH, m);

	// check if kernel execution generated and error
	CUT_CHECK_ERROR("Kernel execution failed");

    // copy results from device to host
    CUDA_SAFE_CALL(cudaMemcpy(out, d_data, mem_size, cudaMemcpyDeviceToHost));

    // cleanup memory
    CUDA_SAFE_CALL(cudaFree(d_data));
}

