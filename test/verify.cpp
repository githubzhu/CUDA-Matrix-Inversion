#include <bits/stdc++.h>
#include "field.hpp"

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

using namespace std;

bool check(field_element* A, field_element* B, int n){
	field_element* C = new field_element[n*n];
	FWD(i,0,n*n) C[i] = 0;
	FWD(i,0,n)
		FWD(k,0,n)
			FWD(j,0,n)
				C[i*n+j] += A[i*n+k] * B[k*n+j];
	bool correct = 1;
	FWD(i,0,n)
		FWD(j,0,n)
			if(C[i*n+j] != (i == j ? 1 : 0))
				correct = 0;
	delete[] C;
	return correct;
}

void read_matrix(field_element* in, int n){
	FWD(i,0,n)
		FWD(j,0,n){
			int a;
			assert(scanf("%d", &a) == 1);
			in[i*n+j] = a;
		}
}

int main(){
	int status = 0;

	int mod;
	assert(scanf("%d", &mod) == 1);
	field_init(mod);

	int n;
	assert(scanf("%d", &n) == 1);

	field_element* in = new field_element[n*n];
	read_matrix(in, n);

	int cpu_res;
	field_element* cpu = new field_element[n*n];
	assert(scanf("%d", &cpu_res) == 1);
	if(cpu_res)
		read_matrix(cpu, n);

	int gpu_res;
	field_element* gpu = new field_element[n*n];
	assert(scanf("%d", &gpu_res) == 1);
	if(gpu_res)
		read_matrix(gpu, n);

	if(cpu_res != gpu_res){
		if(cpu_res){
			fprintf(stderr, "CPU found inversion, GPU has not.\n");
		}else{
			fprintf(stderr, "GPU found inversion, CPU has not.\n");
		}
		status = 1;
	}

	if(cpu_res && !check(in, cpu, n)){
		fprintf(stderr, "CPU inversion is not correct.\n");
		status = 1;
	}

	if(gpu_res && !check(in, gpu, n)){
		fprintf(stderr, "GPU inversion is not correct.\n");
		status = 1;
	}

	delete[] in;
	delete[] cpu;
	delete[] gpu;

	if(status){
		fprintf(stderr, "\033[0;31mProblems detected\033[0m\n");
	}else{
		fprintf(stderr, "\033[0;32mAll correct\033[0m\n");
	}

	return status;
}