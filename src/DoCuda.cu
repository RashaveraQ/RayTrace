#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "GetVectorFromPoint.cuh"
#include "sp.cuh"
#include "matrix.cuh"
#include "Node.cuh"
#include "Plus.cuh"
#include <d3d9.h>
#include <stdio.h>

int numSMs = -1;

__global__
void deletePoint(DevNode** out)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		delete *out;
}

bool mallocDev(DevNode*** out)
{
	if (!DoCuda_Init())
		return false;

	cudaError_t cudaStatus = cudaMalloc(out, sizeof(void*));
	if (cudaStatus != cudaSuccess)
		return false;

	return true;
}

bool freeDevicePointer(DevNode** pNode)
{
	deletePoint<<<1, 1>>>(pNode);

	// Check for any errors launching the kernel
	cudaError_t cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	cudaStatus = cudaFree(pNode);
	if (cudaStatus != cudaSuccess)
		return false;

	return true;
}

bool DoCuda_Init()
{
	if (numSMs > 0)
		return true;

	CUcontext context;
	CUdevice cuDevice;

	cudaDeviceProp deviceProp;
	if (cudaSuccess != cudaSetDevice(0) ||
	    cudaSuccess != cudaGetDeviceProperties(&deviceProp, 0))
		return false;

	numSMs = deviceProp.multiProcessorCount;

	size_t value = 1024 * 16;

	// Get handle for device 0
	if (cudaSuccess != cuDeviceGet(&cuDevice, 0) ||
	    cudaSuccess != cuDevicePrimaryCtxRetain(&context, cuDevice) ||
	    cudaSuccess != cuCtxSetLimit(CU_LIMIT_STACK_SIZE, value))
		return false;

	return true;
}

// clamp x to range [a, b]
__device__ float clamp(float x, float a, float b)
{
	return max(a, min(b, x));
}

__device__ int clamp(int x, int a, int b)
{
	return max(a, min(b, x));
}

// convert floating point rgb color to 8-bit integer
__device__ int rgbToInt(float r, float g, float b)
{
	r = clamp(r, 0.0f, 255.0f);
	g = clamp(g, 0.0f, 255.0f);
	b = clamp(b, 0.0f, 255.0f);
	return (int(b) << 16) | (int(g) << 8) | int(r);
}

__global__
void RayTrace(unsigned int* dst, const int imageW, const int imageH, DevNode** root, const int gridWidth, const int numBlocks, const fsize* pView, const matrix* pMatrix)
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
			sp k, l;
			GetVectorFromPoint(k, l, ix, iy, pView, imageW, imageH, pMatrix);
			sp c = (*root)->GetColor(k, l, 0, NULL, true);
			int pixel = imageW * (imageH - iy - 1) + ix;
			dst[pixel] = rgbToInt(c.x, c.y, c.z);
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

bool DoCuda_OnDraw(unsigned int* out, class DevNode** root, const int imageW, const int imageH, const fsize* pView, const matrix* pMatrix)
{
	dim3 threads(BLOCKDIM_X, BLOCKDIM_Y);
	dim3 grid(iDivUp(imageW, BLOCKDIM_X), iDivUp(imageH, BLOCKDIM_Y));

	int numWorkerBlocks = numSMs;

	matrix m(pMatrix->get_width(), pMatrix->get_height());
	for (int w = 1; w <= m.get_width(); w++)
		for (int h = 1; h <= m.get_height(); h++)
			m.set_data(w, h, pMatrix->get_data(w, h));

	matrix* dev_Matrix = 0;
	cudaError_t cudaStatus;
	size_t szMatrix = sizeof(matrix);

	cudaStatus = cudaMalloc((void**)&dev_Matrix, szMatrix);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	cudaStatus = cudaMemcpy(dev_Matrix, &m, szMatrix, cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	fsize* dev_view = 0;
	size_t szFsize = sizeof(fsize);

	cudaStatus = cudaMalloc((void**)&dev_view, szFsize);
	if (cudaStatus != cudaSuccess) {
		return false;
	}
	cudaStatus = cudaMemcpy(dev_view, pView, szFsize, cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	//size_t stackSize = checkStackSize();

	RayTrace<<<numWorkerBlocks, threads>>>((unsigned int*)out, imageW, imageH, root, grid.x, grid.x * grid.y, dev_view, dev_Matrix);

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	cudaStatus = cudaFree(dev_Matrix);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	cudaStatus = cudaFree(dev_view);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}

__global__
void updateMatrix(DevNode** out, const matrix* pMatrix)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		(*out)->m_Matrix = *pMatrix;
}

bool DoCuda_updateMatrix(DevNode** devNode, const struct matrix* pMatrix)
{
	if (!DoCuda_Init())
		return false;

	matrix m(pMatrix->get_width(), pMatrix->get_height());
	for (int w = 1; w <= m.get_width(); w++)
		for (int h = 1; h <= m.get_height(); h++)
			m.set_data(w, h, pMatrix->get_data(w, h));

	matrix* dev_Matrix = 0;
	cudaError_t cudaStatus;
	size_t szMatrix = sizeof(matrix);

	cudaStatus = cudaMalloc((void**)&dev_Matrix, szMatrix);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	cudaStatus = cudaMemcpy(dev_Matrix, &m, szMatrix, cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	updateMatrix<<<1, 1>>>(devNode, dev_Matrix);

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	cudaStatus = cudaFree(dev_Matrix);
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}

__global__
void updateColor(DevNode** out, float r, float g, float b)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
		(*out)->m_Material.Diffuse.r = (*out)->m_Material.Ambient.r = r;
		(*out)->m_Material.Diffuse.g = (*out)->m_Material.Ambient.g = g;
		(*out)->m_Material.Diffuse.b = (*out)->m_Material.Ambient.b = b;
	}
}

bool DoCuda_updateColor(DevNode** devNode, float r, float g, float b)
{
	if (!DoCuda_Init())
		return false;

	cudaError_t cudaStatus;

	updateColor<<<1, 1>>>(devNode, r, g, b);

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}

__global__
void updateMaterial(DevNode** out, float reflect, float refractive, float through)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0) {
		(*out)->m_Reflect = reflect;
		(*out)->m_Refractive = refractive;
		(*out)->m_Through = through;
	}
}

bool DoCuda_updateMaterial(DevNode** devNode, float reflect, float refractive, float through)
{
	if (!DoCuda_Init())
		return false;

	cudaError_t cudaStatus;

	updateMaterial<<<1, 1>>>(devNode, reflect, refractive, through);

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}

	return true;
}

__global__
void SetRoot(DevNode** out, DevNode** root)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		(*out)->SetRoot(root);
}

bool DoCuda_SetRoot(DevNode** devNode, DevNode** devRoot)
{
	if (!DoCuda_Init())
		return false;

	SetRoot << <1, 1 >> >(devNode, devRoot);

	cudaError_t cudaStatus;
	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}
	return true;
}

__global__
void AddNode(DevGathering **out, DevNode** node)
{
	if (blockIdx.x == 0 && blockIdx.y == 0 && blockIdx.z == 0 && threadIdx.x == 0 && threadIdx.y == 0 && threadIdx.z == 0)
		(*out)->AddNode(node);
}

bool DoCuda_AddNode(DevGathering** devGathering, DevNode** devNode)
{
	if (!DoCuda_Init())
		return false;

	AddNode<<<1, 1>>>(devGathering, devNode);

	cudaError_t cudaStatus;
	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		return false;
	}
	return true;
}