#include <bits/stdc++.h>

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

bool inverse_gpu(int *A, int *B, int n){

	FWD(i,0,n)
		FWD(j,0,n)
			B[i*n+j] = (i == j);

	FWD(i,0,n){

		// finding row with 1 on i-th column
		if(!A[i*n+i]){
			int k = -1;
			FWD(j,i+1,n)
				if(A[j*n+i]){
					k = j;
					break;
				}
			if(k == -1) return 0;
			FWD(j,0,n) std::swap(A[i*n+j], A[k*n+j]);
			FWD(j,0,n) std::swap(B[i*n+j], B[k*n+j]);
		}
		
		// removing 1's in this column from other rows
		FWD(k,0,n)
			if(i != k && A[k*n+i]){
				FWD(j,0,n) A[k*n+j] ^= A[i*n+j];
				FWD(j,0,n) B[k*n+j] ^= B[i*n+j];	
			}

	}

	return 1;
}
