#include <cutil.h>
#include <cuda_runtime.h>
#include <d3dx9.h>
#include <atltypes.h>
#include "sp.h"

// The dimensions of the thread block
#define BLOCKDIM_X 16
#define BLOCKDIM_Y 16

__global__ void kernel(unsigned long* dst, void* root, const int imageW, const int imageH)
{
    const int ix = blockDim.x * blockIdx.x + threadIdx.x;
    const int iy = blockDim.y * blockIdx.y + threadIdx.y;
}

// Increase the grid size by 1 if the image width or height does not divide evenly
// by the thread block dimensions
inline int iDivUp(int a, int b)
{
    return ((a % b) != 0) ? (a / b + 1) : (a / b);
} // iDivUp

void DoCuda(unsigned long* out, class Node* root, const int imageW, const int imageH)
{
	unsigned long* d_data;
    const unsigned int mem_size = imageW * imageH * sizeof(unsigned long);

	CUDA_SAFE_CALL(cudaMalloc((void**)&d_data, mem_size));
	
    dim3 threads(BLOCKDIM_X, BLOCKDIM_Y);
    dim3 grid(iDivUp(imageW, BLOCKDIM_X), iDivUp(imageH, BLOCKDIM_Y));
 
	// execute the kernel
	kernel<<< grid, threads >>>(d_data, (void*)root, imageW, imageH);

	// check if kernel execution generated and error
	CUT_CHECK_ERROR("Kernel execution failed");

    // copy results from device to host
    CUDA_SAFE_CALL(cudaMemcpy(out, d_data, mem_size, cudaMemcpyDeviceToHost));

    // cleanup memory
    CUDA_SAFE_CALL(cudaFree(d_data));
}

