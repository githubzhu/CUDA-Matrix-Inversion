#include "cuda_field.cu"

extern "C" {

__global__
void field_init(int m){
	cuda_field_init(m);
}

__global__
void make_unit(cuda_field_element* M, int n){
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
	if(thidX < n && thidY < n){
		if(thidX == thidY){
			M[thidX*n+thidY] = 1;
		}else{
			M[thidX*n+thidY] = 0;
		}
	}
}

__global__
void find_nonzero(cuda_field_element* M, int n, int i, int* k){
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	if(i < thidX && thidX < n)
		if(M[thidX*n+i] != 0)
			*k = thidX;
}

__global__
void swap(cuda_field_element* M, int n, int i, int k){
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	if(thidX < n){
		cuda_field_element v = M[i*n+thidX];
		M[i*n+thidX] = M[k*n+thidX];
		M[k*n+thidX] = v;
	}
}

__global__
void fix_row(cuda_field_element* M, int n, int i, cuda_field_element mul){
	//int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
}

__global__
void fix_column(cuda_field_element* M, int n, int i){
	//int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	//int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
}

}
