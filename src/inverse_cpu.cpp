#include <bits/stdc++.h>
#include "field.hpp"

#define FWD(a,b,c) for(int a=(b); a<(c); ++a)

bool inverse_cpu(field_element *A, field_element *B, int n){

	FWD(i,0,n)
		FWD(j,0,n)
			B[i*n+j] = (i == j ? 1 : 0);

	FWD(i,0,n){

		// finding row with non-zero value on i-th column
		if(A[i*n+i] == 0){
			int k = -1;
			FWD(j,i+1,n)
				if(A[j*n+i] != 0){
					k = j;
					break;
				}
			if(k == -1) return 0;
			FWD(j,0,n) std::swap(A[i*n+j], A[k*n+j]);
			FWD(j,0,n) std::swap(B[i*n+j], B[k*n+j]);
		}
		
		if(A[i*n+i] != 1){
			field_element inv = ~A[i*n+i];
			FWD(j,0,n) A[i*n+j] *= inv;
			FWD(j,0,n) B[i*n+j] *= inv;
		}

		FWD(k,0,n)
			if(i != k && A[k*n+i] != 0){
				field_element m = A[k*n+i];
				FWD(j,0,n) A[k*n+j] -= A[i*n+j] * m;
				FWD(j,0,n) B[k*n+j] -= B[i*n+j] * m;
			}

	}

	return 1;
}
