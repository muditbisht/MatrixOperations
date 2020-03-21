#include <iostream>
using namespace std;

void del(int** A, int N){
	for(int i=0;i<N;i++)
		delete[] A[i];
	delete[] A;
}
	

void input(int **A, int N, string S){
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			cin >> A[i][j];
}

void output(int **A, int N, string S){
	cout << "\n Matrix " << S << "-\n";

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)
			cout << A[i][j] << " ";
		cout << endl;
	}
	cout << endl << endl;
}

void add(int **C,  int N, int **A, int **B, int Ci=0, int Cj=0, int Ai=0, int Aj=0, int Bi=0, int Bj=0, bool subtract=false){
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			C[Ci+i][Cj+j] = A[Ai+i][Aj+j] + (subtract?-B[Bi+i][Bj+j]:B[Bi+i][Bj+j]);
}

int giveNearest(int N){
	int T = 0x40000000;
	while(T>N)
		T>>=1;
	if(T==N)
		return N;
	else
		return T<<1;
}

int **simpleRecursion(int **A, int **B, int N, int Ai=0, int Aj=0, int Bi=0, int Bj=0){ // takes O(N^3)
	int **C = new int*[N];
	for(int i=0;i<N;i++)
		C[i] = new int[N];

	if(N==1){
		C[0][0] = A[Ai][Aj] * B[Bi][Bj];
		return C;
	}
	
	int **C11a = simpleRecursion(A,B,N/2,Ai,Aj,Bi,Bj);
	
	int **C11b = simpleRecursion(A,B,N/2,Ai,Aj+N/2,Bi+N/2,Bj);
	
	int **C12a = simpleRecursion(A,B,N/2,Ai,Aj,Bi,Bj+N/2);
	
	int **C12b = simpleRecursion(A,B,N/2,Ai,Aj+N/2,Bi+N/2,Bj+N/2);
	
	int **C21a = simpleRecursion(A,B,N/2,Ai+N/2,Aj,Bi,Bj);
	
	int **C21b = simpleRecursion(A,B,N/2,Ai+N/2,Aj+N/2,Bi+N/2,Bj);
	
	int **C22a = simpleRecursion(A,B,N/2,Ai+N/2,Aj,Bi,Bj+N/2); 
	
	int **C22b = simpleRecursion(A,B,N/2,Ai+N/2,Aj+N/2,Bi+N/2,Bj+N/2);
	
	add(C, N/2, C11a, C11b, 0, 0);
	add(C, N/2, C12a, C12b, 0, N/2);
	add(C, N/2, C21a, C21b, N/2, 0);
	add(C, N/2, C22a, C22b, N/2, N/2);
	
	//output(C,N,"C");
	
	del( C11a, N/2);
	del( C11b, N/2);
	del( C12a, N/2);
	del( C12b, N/2);
	del( C21a, N/2);
	del( C21b, N/2);
	del( C22a, N/2);
	del( C22b, N/2);
	
	return C;
	
}


int **strassensAlgo(int **A, int **B, int N, int Ai=0, int Aj=0, int Bi=0, int Bj=0){
	int **C = new int*[N];
	for(int i=0;i<N;i++){
		C[i] = new int[N];
		for(int j=0;j<N;j++)
			C[i][j] = 0;
	}
	
	
	if(N==1){
			C[0][0] = A[Ai+0][Aj+0] * B[Bi+0][Bj+0];
			return C;
	}
	
	int ***S = new int** [11];
	for(int i=1;i<=10;i++){
		S[i] = new int* [N/2];
		for(int j=0;j<N/2;j++)
			S[i][j] = new int[N/2];
	}
		
	// S1 = B12 - B22
	add(S[1], N/2, B, B, 0, 0, Bi+0, Bj+N/2, Bi+N/2, Bj+N/2, true);
	
	// S2 = A11 + A12
	add(S[2], N/2, A, A, 0, 0, Ai+0, Aj+0, Ai+0, Aj+N/2);
	
	// S3 = A21 + A22
	add(S[3], N/2, A, A, 0, 0, Ai+N/2, Aj+0, Ai+N/2, Aj+N/2);

	// S4 = B21 - B11
	add(S[4], N/2, B, B, 0, 0, Bi+N/2, Bj+0, Bi+0, Bj+0, true);
	
	// S5 = A11 + A22
	add(S[5], N/2, A, A, 0, 0, Ai+0, Aj+0, Ai+N/2, Aj+N/2);	
	
	// S6 = B11 + B22
	add(S[6], N/2, B, B, 0, 0, Bi+0, Bj+0, Bi+N/2, Bj+N/2);
	
	// S7 = A12 - A22
	add(S[7], N/2, A, A, 0, 0, Ai+0, Aj+N/2, Ai+N/2, Aj+N/2, true);
	
	// S8 = B21 + B22
	add(S[8], N/2, B, B, 0, 0, Bi+N/2, Bj+0, Bi+N/2, Bj+N/2);
	
	// S9 = A11 - A21
	add(S[9], N/2, A, A, 0, 0, Ai+0, Aj+0, Ai+N/2, Aj+0, true);
	
	// S10 = B11 + B12
	add(S[10],N/2, B, B, 0, 0, Bi+0, Bj+0, Bi+0, Bj+N/2);
	
	
	int ***P = new int** [8];
	
	// P1 = A11 * S1
	P[1] = strassensAlgo(A, S[1], N/2, Ai, Aj);
	
	// P2 = S2 * B22
	P[2] = strassensAlgo(S[2],B, N/2, 0, 0, Bi+N/2, Bj+N/2);
	
	// P3 = S3 * B11
	P[3] = strassensAlgo(S[3], B, N/2, 0, 0, Bi, Bj);
	
	// P4 = A22 * S4
	P[4] = strassensAlgo(A, S[4], N/2, Aj+N/2, Aj+N/2);
	
	// P5 = S5 * S6
	P[5] = strassensAlgo(S[5], S[6], N/2);
	
	// P6 = S7 * S8
	P[6] = strassensAlgo(S[7], S[8], N/2);
	
	// P7 = S9 * S10
	P[7] = strassensAlgo(S[9], S[10], N/2);
	
	
	//  C11 = P5 + P4 - P2 + P6 
	add(C, N/2, P[5] , P[4]);
	add(P[6], N/2, P[6], P[2], 0, 0, 0, 0, 0, 0, true);
	add(C, N/2, C, P[6]);
	
	// C12 = P1 + P2
	add(C, N/2, P[1], P[2], 0, N/2);
	
	// C21 = P3 + P4
	add(C, N/2, P[3], P[4], N/2, 0);
	
	// C22 = P5 + P1 - P3 -P7 
	add(P[3], N/2, P[3], P[7]);
	add(P[5], N/2, P[5], P[1]);
	add(C, N/2, P[5], P[3], N/2, N/2, 0, 0, 0, 0, true);
	
	
	for(int i=1;i<=10;i++)
		del(S[i],N/2);
	delete[] S;
		
	for(int i=1;i<=7;i++)
		del(P[i],N/2);
	delete[] P;
	
	return C;
}


int** square_matrix_recursive(int **A, int **B, int N, int** (*f)(int**, int**, int, int, int, int, int), int Ai=0, int Aj=0, int Bi=0, int Bj=0){
	
	int newN = giveNearest(N);
	if(newN!=N){
	
		int **C = new int*[N];
		for(int i=0;i<N;i++)
			C[i] = new int[N];
		
		int **tmpA = new int* [newN];
		int **tmpB = new int* [newN];
		
		for(int i=0;i<newN;i++){
			tmpA[i] = new int[newN];
			tmpB[i] = new int[newN];
		}
		for(int i=0;i<newN;i++)
			for(int j=0;j<newN;j++){
				if(i<N && j<N){
					tmpA[i][j] = A[i][j];
					tmpB[i][j] = B[i][j];
				}
				else
					tmpB[i][j] = tmpA[i][j] = 0;
			}
		
		
		int **tmpC = f(tmpA,tmpB,newN,0,0,0,0);
		
		output(tmpC,newN,"C");
		
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)
				C[i][j] = tmpC[i][j];

		del(tmpA,newN);
		del(tmpB,newN);
		del(tmpC,newN);

		return C;
	}
	return f(A,B,N,0,0,0,0);
	
	
}

int main(){
	int N;
	cout << "Enter N - ";
	cin >> N;
	
	int **A, **B, **C;

	A = new int*[N];
	B = new int*[N];

	for(int i=0;i<N;i++){
		A[i] = new int[N];
		B[i] = new int[N];
	}

	input(A,N,"A");
	input(B,N,"B");

	output(A,N,"A");
	output(B,N,"B");

	// for simple recursive method O(N^3)
	//C = square_matrix_recursive(A,B,N,simpleRecursion);
	
	// using strassen's Algo. O(N^lg7)
	C = square_matrix_recursive(A,B,N,strassensAlgo);

	output(C,N,"C");

	del(A,N);
	del(B,N);
	del(C,N);

	return 0;
}
