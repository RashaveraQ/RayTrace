
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"

#include <stdio.h>
/*
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);

__global__ void addKernel(int *c, const int *a, const int *b)
{
	int i = threadIdx.x;
	c[i] = a[i] + b[i];
}

int main()
{
	const int arraySize = 5;
	const int a[arraySize] = { 1, 2, 3, 4, 5 };
	const int b[arraySize] = { 10, 20, 30, 40, 50 };
	int c[arraySize] = { 0 };

	// Add vectors in parallel.
	cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}

	printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
		c[0], c[1], c[2], c[3], c[4]);

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	return 0;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
	int *dev_a = 0;
	int *dev_b = 0;
	int *dev_c = 0;
	cudaError_t cudaStatus;

	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}

	// Allocate GPU buffers for three vectors (two input, one output)    .
	cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	// Copy input vectors from host memory to GPU buffers.
	cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

	cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

	// Launch a kernel on the GPU with one thread for each element.
	addKernel << <1, size >> >(dev_c, dev_a, dev_b);

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto Error;
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
		goto Error;
	}

Error:
	cudaFree(dev_c);
	cudaFree(dev_a);
	cudaFree(dev_b);

	return cudaStatus;
}
*/

int numSMs = -1;

__global__
void deletePoint(DevNode* out)
{
	if (threadIdx.x == 0)
		delete out;
}

DevNode* mallocDevicePointer()
{
	if (!DoCuda_Init())
		return 0;

	cudaError_t cudaStatus;

	DevNode* devPtr;

	cudaStatus = cudaMalloc(&devPtr, sizeof(void*));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	return devPtr;
Error:
	return 0;
}

bool freeDevicePointer(DevNode* pNode)
{
	deletePoint<<<1, 1 >>>(pNode);
	cudaFree(pNode);
	return true;
}

bool DoCuda_Init()
{
	if (numSMs > 0)
		return true;

	cudaDeviceProp deviceProp;
	if (cudaSuccess != cudaSetDevice(0))
		return false;

	if (cudaSuccess != cudaGetDeviceProperties(&deviceProp, 0))
		return false;

	numSMs = deviceProp.multiProcessorCount;

	return true;
}

bool DoCuda_OnSize(void** dst, const int imageW, const int imageH)
{
	return cudaSuccess == cudaMalloc(dst, imageW * imageH * sizeof(unsigned long));
}

__global__
void RayTrace(unsigned long* dst, const int imageW, const int imageH, DevNode* root, const int gridWidth, const int numBlocks, const fsize* pView, const matrix* pMatrix)
{
	// loop until all blocks completed
	for (unsigned int blockIndex = blockIdx.x; blockIndex < numBlocks; blockIndex += gridDim.x)
	{
		unsigned int blockX = blockIndex % gridWidth;
		unsigned int blockY = blockIndex / gridWidth;

		// process this block
		const int ix = blockDim.x * blockX + threadIdx.x;
		const int iy = blockDim.y * blockY + threadIdx.y;

		if ((ix < imageW) && (iy < imageH))
		{
			// Output the pixel
			int pixel = imageW * iy + ix;
			dst[pixel] = pixel;
		}
	}
}

// The dimensions of the thread block
#define BLOCKDIM_X 16
#define BLOCKDIM_Y 16

// Increase the grid size by 1 if the image width or height does not divide evenly
// by the thread block dimensions
inline int iDivUp(int a, int b)
{
	return ((a % b) != 0) ? (a / b + 1) : (a / b);
} // iDivUp

bool DoCuda_OnDraw(unsigned long* out, void* d_dst, class DevNode* root, const int imageW, const int imageH, const fsize* pView, const matrix* pMatrix)
{
	dim3 threads(BLOCKDIM_X, BLOCKDIM_Y);
	dim3 grid(iDivUp(imageW, BLOCKDIM_X), iDivUp(imageH, BLOCKDIM_Y));

	int numWorkerBlocks = numSMs;

	RayTrace<<<numWorkerBlocks, threads>>>((unsigned long*)d_dst, imageW, imageH, root, grid.x, grid.x * grid.y, pView, pMatrix);

	// Copy output vector from GPU buffer to host memory.
	return cudaSuccess == cudaMemcpy(out, d_dst, imageW * imageH * sizeof(unsigned long), cudaMemcpyDeviceToHost);
}

bool DoCuda_Free(void* dst)
{
	return (dst) ? cudaSuccess == cudaFree(dst) : false;
}

