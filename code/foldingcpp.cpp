#include "foldingcpp.h"
#include <string>

using namespace std;

void init_score_table(int** score_table, int size){
	// 4 diagonal rows initialized as 0
	int i;
	for (i=0; i<size; i++){
		score_table[i][i] = 0;
	}

	for (i=0; i<size-1; i++){
		score_table[i][i+1] = 0;
	}

	for (i=0; i<size-2; i++){
		score_table[i][i+2] = 0;
	}

	for (i=0; i<size-3; i++){
		score_table[i][i+3] = 0;
	}
};

void init_traceback_table(int** traceback_table, int size){
	//-2 for traceback end point
	int i;
	for (i=0; i<size; i++){
		traceback_table[i][i] = -2;
	}

	for (i=0; i<size-1; i++){
		traceback_table[i][i+1] = -2;
	}

	for (i=0; i<size-2; i++){
		traceback_table[i][i+2] = -2;
	}

	for (i=0; i<size-3; i++){
		traceback_table[i][i+3] = -2;
	}
};


void nussinov(string &sequence, int** score_table, int** traceback_table){
	 // -1 for diagonal, store k for traceback
};
void trace_back(string &sequence, int** score_table, int** traceback_table, int* match_table){
};
