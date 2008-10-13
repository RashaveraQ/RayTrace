#include <stdio.h>
#include "cutil4win.h"
#include <d3dx9.h>
#include <atltypes.h>

#define TARGET __device__
#include "task.h"
#include "matrix.h"
//#include "info.h"

__constant__ Task cTask[100];
__constant__ int  cTaskIndex;
static int sTaskIndex = 0;

__global__ void kernel(unsigned long* dst, int imageW, int imageH, matrix* pMatrix, sp* pLight)
{
    const int px = blockDim.x * blockIdx.x + threadIdx.x;
    const int py = blockDim.y * blockIdx.y + threadIdx.y;
   	double rx = 20.0 * px / imageW - 10.0;
	double ry = 20.0 * py / imageH - 10.0;

	sp k(0.01 * rx / 20.0, 0.01 * ry / 20.0, 0.01);
	sp l(rx, ry, -20);
    
	k = *pMatrix * (k + l) - *pMatrix * l;
	l = *pMatrix * l;

//	sp c = Root->GetColor(&k, &l, 0);
	sp c((imageW * cTaskIndex) % 256, (imageH * cTaskIndex) % 256, 0);
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
	CUDA_SAFE_CALL(cudaMemcpyToSymbol(&cTaskIndex, &sTaskIndex, sizeof(int)));

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
	CUDA_SAFE_CALL(cudaMemcpyToSymbol(&cTaskIndex, &sTaskIndex, sizeof(int)));
}

void AddTask(const Task& task)
{
	CUDA_SAFE_CALL(cudaMemcpyToSymbol(cTask + sTaskIndex, &task, sizeof(Task)));
	sTaskIndex++;
}
