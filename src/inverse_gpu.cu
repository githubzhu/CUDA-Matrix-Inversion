#include "cuda_field.cu"

extern "C" {

__global__
void field_init(int m){
	cuda_field_init(m);
}

__global__
void make_unit(cuda_field_element* B, int n){
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
	if(thidX < n && thidY < n){
		if(thidX == thidY){
			B[thidX*n+thidY] = 1;
		}else{
			B[thidX*n+thidY] = 0;
		}
	}
}

__global__
void find_nonzero(cuda_field_element* A, int n, int i, int* k){
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	if(i < thidX && thidX < n)
		if(A[thidX*n+i] != 0)
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
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	if(thidX < n){
		M[i*n+thidX] = M[i*n+thidX] * mul;
	}
}

__global__
void update_column(cuda_field_element* A, cuda_field_element* i_th_column, int n, int i){
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	if(thidX < n){
		i_th_column[thidX] = A[thidX*n + i];
	}
}

__global__
void fix_column(cuda_field_element* M, cuda_field_element* i_th_column, int n, int i){
	int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
	__shared__ cuda_field_element P[32];
	if(thidY > i && thidY < n){
		P[threadIdx.x] = M[(i-1)*n+thidX];
		M[thidY*n+thidX] -= i_th_column[thidY]*P[threadIdx.x];
	}
}

}

