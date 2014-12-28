#include <bits/stdc++.h>
#include <cuda.h>
#include "field.hpp"
#include "utils.hpp"

#define CHECK(r) \
	if(r != CUDA_SUCCESS){ \
		fprintf(stderr, "%s:%d CUDA error: %s\n", __FILE__, __LINE__, cuda_error_string(r)); \
		exit(1); \
	}

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

int ceil(int a, int b){
	return (a+b-1) / b;
}

template<typename T>
void set_device_variable(CUdeviceptr var, T val){
	CHECK(cuMemcpyHtoD(var, &val, sizeof(T)));
}

template<typename T>
void get_device_variable(CUdeviceptr var, T& val){
	CHECK(cuMemcpyDtoH(&val, var, sizeof(T)));
}

bool inverse_gpu(field_element *A, field_element *B, int n){
	//fprintf(stderr, "current_matrix\n");
	CHECK(cuInit(0));
	
	CUdevice cuda_device;
	CHECK(cuDeviceGet(&cuda_device, 0));

	CUcontext cuda_context;
	CHECK(cuCtxCreate(&cuda_context, 0, cuda_device));

	CUmodule cuda_module = (CUmodule)0;
	CHECK(cuModuleLoad(&cuda_module, "obj/inverse_gpu.ptx"));

	CUfunction field_init, make_unit, find_nonzero, swap, fix_row, update_column, fix_column;
	CHECK(cuModuleGetFunction(&field_init, cuda_module, "field_init"));
	CHECK(cuModuleGetFunction(&make_unit, cuda_module, "make_unit"));
	CHECK(cuModuleGetFunction(&find_nonzero, cuda_module, "find_nonzero"));
	CHECK(cuModuleGetFunction(&swap, cuda_module, "swap"));
	CHECK(cuModuleGetFunction(&fix_row, cuda_module, "fix_row"));
	CHECK(cuModuleGetFunction(&update_column, cuda_module, "update_column"));
	CHECK(cuModuleGetFunction(&fix_column, cuda_module, "fix_column"));

	CUdeviceptr devA, devB, dev_to_process, row_with_one, i_th_column;
	int field_modulus, current_index, row_to_swap;
	field_element mul;
	void* args_field_init[] = {&field_modulus};
	void* args_make_unit[] = {&devB, &n};
	void* args_find_nonzero[] = {&devA, &n, &current_index, &row_with_one};
	void* args_swap[] = {&dev_to_process, &n, &current_index, &row_to_swap};
	void* args_fix_row[] = {&dev_to_process, &n, &current_index, &mul};
	void* args_update_column[] = {&devA, &i_th_column, &n, &current_index};
	void* args_fix_column[] = {&dev_to_process, &i_th_column, &n, &current_index};
	
	int tX_oneD = 1024;
	int bX_oneD = ceil(n, tX_oneD);
	
	int tX_twoD = 32, tY_twoD = 32;
	int bX_twoD = ceil(n, tX_twoD);
	int bY_twoD = ceil(n, tY_twoD);
	
	int bytes = n*n*sizeof(field_element);
	CHECK(cuMemHostRegister((void*)A, bytes, CU_MEMHOSTREGISTER_PORTABLE));
	CHECK(cuMemAlloc(&devA, bytes));
	CHECK(cuMemcpyHtoD(devA, A, bytes)); 

	CHECK(cuMemAlloc(&devB, bytes));
	CHECK(cuMemAlloc(&i_th_column, n*sizeof(field_element)));

	field_modulus = field_element::get_modulus();
	CHECK(cuLaunchKernel(field_init, 1, 1, 1, 1, 1, 1, 0, 0, args_field_init, 0));
	CHECK(cuCtxSynchronize());

	CHECK(cuLaunchKernel(make_unit, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, args_make_unit, 0));
	CHECK(cuCtxSynchronize());

	CHECK(cuMemAlloc(&row_with_one, sizeof(int)));
	for(current_index = 0; current_index < n; ++current_index){
		//fprintf(stderr, "current_matrix\n");
		// CHECK(cuMemcpyDtoH(A, devA, bytes));
		// FWD(i,0,n) FWD(j,0,n) fprintf(stderr, "%d%c", A[i*n+j].get_value(), " \n"[j==n-1]);

		// fprintf(stderr, "current_index = %d\n", current_index);

		int cell = current_index * n + current_index;

		// finding row with non-zero value on i-th column
		CHECK(cuMemcpyDtoH(A+cell, devA+cell*sizeof(field_element), sizeof(field_element)));
		if(A[cell] != 1){
			set_device_variable(row_with_one, -1);
			CHECK(cuLaunchKernel(find_nonzero, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, args_find_nonzero, 0));
			get_device_variable(row_with_one, row_to_swap);
			// fprintf(stderr, "row_to_swap = %d\n", row_to_swap);
			if(row_to_swap == -1) return 0;

			dev_to_process = devA;
			CHECK(cuLaunchKernel(swap, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, args_swap, 0));
			CHECK(cuCtxSynchronize()); 
			
			dev_to_process = devB;
			CHECK(cuLaunchKernel(swap, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, args_swap, 0));
			CHECK(cuCtxSynchronize()); 
		}
		// multiplying row to get one in i-th diagonal cell
		CHECK(cuMemcpyDtoH(A+cell, devA+cell*sizeof(field_element), sizeof(field_element)));
		if(A[cell] != 1){
			mul = ~A[cell];
			
			dev_to_process = devA;
			CHECK(cuLaunchKernel(fix_row, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, args_fix_row, 0));
			CHECK(cuCtxSynchronize()); 
		
			dev_to_process = devB;
			CHECK(cuLaunchKernel(fix_row, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, args_fix_row, 0));
			CHECK(cuCtxSynchronize());
		}	
		// reducing all the other elements of the i-th column to zero	
		CHECK(cuLaunchKernel(update_column, bX_oneD, 1, 1, tX_oneD, 1, 1, 0, 0, args_update_column, 0));
		CHECK(cuCtxSynchronize()); 	
		
		dev_to_process = devA;
		CHECK(cuLaunchKernel(fix_column, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, args_fix_column, 0));
		CHECK(cuCtxSynchronize()); 

		dev_to_process = devB;
		CHECK(cuLaunchKernel(fix_column, bX_twoD, bY_twoD, 1, tX_twoD, tY_twoD, 1, 0, 0, args_fix_column, 0));
		CHECK(cuCtxSynchronize());

	}

	CHECK(cuMemHostRegister((void*)B, bytes, CU_MEMHOSTREGISTER_PORTABLE));
	CHECK(cuMemcpyDtoH(B, devB, bytes)); 

	// fprintf(stderr, "success\n");

	return 1;
}

