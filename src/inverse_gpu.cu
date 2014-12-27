#include "field.hpp"

extern "C" {

__global__
void make_unit(int* M, int n){
	//int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	//int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
}

__global__
void find_nonzero(int* M, int n, int i, int* k){
	//int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;	
}

__global__
void swap(int* M, int n, int i, int k){
	//int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
}

__global__
void fix_row(int* M, int n, int i, field_element mul){
	//int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
}

__global__
void fix_column(int* M, int n, int i){
	//int thidX = (blockIdx.x * blockDim.x) + threadIdx.x;
	//int thidY = (blockIdx.y * blockDim.y) + threadIdx.y;
}

}
