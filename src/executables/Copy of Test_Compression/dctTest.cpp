#include "math.h"
#include <iostream>

#define PI 3.141592653589

using namespace std;

//input: f, N; output: F
short dct_direct( short N, double* f, double* F){
	double a[32], sum, coef;
	short i, j, u, v;

	if ( N > 32 || N <= 0 ) {
		cout<<"\n inappropriate N\n"<<endl;
		return -1;
	}

	a[0] = sqrt ( 1.0 / N );
	for ( i = 1; i < N; ++i ) {
		a[i] = sqrt ( 2.0 / N );
	}

	for ( u = 0; u < N; ++u ) {
		for ( v = 0; v < N; ++v ) {
			sum = 0.0;
			for ( i = 0; i < N; ++i ) {
				for ( j = 0; j < N; ++j ) {
					coef = cos((2*i+1)*u*PI/(2*N))*cos((2*j+1)*v*PI/(2*N));
					sum +=*(f+i*N+j)*coef; //f[i][j]*coef
				} //for j
				*(F+u*N+v) = a[u]*a[v]*sum;
			} //for i
		} //for u
	} //for v
return 1;
}


//input: N, F; output f
short idct_direct( short N, double*F, double*f ){
	double a[32], sum, coef;
	short i, j, u, v;
	if ( N > 32 || N <= 0 ) {
		cout<< ("\ninappropriate N\n")<<endl;
		return -1;
	}
	a[0] = sqrt ( 1.0 / N );
	for ( i = 1; i < N; ++i ) {
		a[i] = sqrt ( 2.0 / N );
	}
	for ( i = 0; i < N; ++i ) {
		for ( j = 0; j < N; ++j ) {
			sum = 0.0;
			for ( u = 0; u < N; ++u ) {
				for ( v = 0; v < N; ++v ) {
					coef = cos((2*j+1)*v*PI/(2*N))*cos ((2*i+1)*u*PI/(2*N));
					sum+=a[u]*a[v]*(*(F+u*N+v))*coef;	//a[u]*a[v]*F[u][v]*coef
					} //for j
				*(f+i*N+j) = sum;
			} //for i
		} //for u
	} //for v

	return 1;
}

/*
change values from short to double and vice versa.
*/
short dct ( short N, short* f, short* F ){
	double tempx[1024], tempy[1024];
	int total, i;
	if ( N > 32 || N <= 0 ) {
		cout<< ("\ninappropriate N\n")<<endl;
		return -1;
	}

	total = N*N;
	for ( i = 0; i < total; ++i ) {
		tempx[i] = (double)*(f+i);
	}

	dct_direct ( N, tempx, tempy ); //DCT operation
	for ( i = 0; i < total; ++i ) {
		*(F+i) = (short ) ( floor (tempy[i]+0.5) ); //rounding
	}

return 1;
}


/*
change values from short to doulbe, and vice versa.
*/
short idct ( short N, short* F, short* f ){
	double tempx[1024], tempy[1024];
	int total, i;
	if ( N > 32 || N <= 0 ) {
		cout<< ("\ninappropriate N\n")<<endl;
		return -1;
	}

	total = N*N;
	for ( i = 0; i < total; ++i ) {
		tempy[i] = (double)*(F+i);
	}

	idct_direct ( N, tempy, tempx ); //IDCT operation
	for ( i = 0; i < total; ++i ) {
		*(f+i) = (short ) ( floor (tempx[i]+0.5) ); //rounding
	}

return 1;
}


void print_elements ( short N, short* f ){
	short i, j;
	for ( i = 0; i < N; ++i ){
		cout<<("\n");
		for ( j = 0; j < N; ++j ) {
			cout<< ("%4d, ",*(f+N*i+j) );
		}
	}
}

//int main(){
//	short f[8][8], F[8][8];
//	int i, j, N;
//	char temp[8][8];
//	N = 8;
//
//	//try some values for testing
//	for ( i = 0; i < N; ++i ) {
//		for ( j = 0; j < N; ++j ) {
//			f[i][j] = i + j;
//		}
//	}
//	cout<<("\nOriginal sample values");
//	print_elements ( N, &f[0][0] );
//	cout<<("\n--------------------\n");
//
//	dct ( N, &f[0][0], &F[0][0] ); //performing DCT
//	cout<<("\nCoefficients of DCT:");
//	print_elements ( N, &F[0][0] );
//	cout<<("\n--------------------\n");
//
//	idct ( N, &F[0][0], &f[0][0] ); //performing IDCT
//	cout<<("\nValues recovered by IDCT:");
//	print_elements ( N, &f[0][0] );
//	cout<<("\n");
//}
