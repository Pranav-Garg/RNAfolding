#ifndef _FOLDINGCPP_H_INCLUDED_
#define _FOLDINGCPP_H_INCLUDED_

#include <string>
using namespace std;

// nussinov
void init_score_table(int** score_table, int size); // 4 diagonal rows initialized as 0
void init_traceback_table(int** traceback_table, int size);	//-2 for traceback end point
void nussinov(string &sequence, int** score_table, int** traceback_table); // -1 for diagonal, store k for traceback
void trace_back(string &sequence, int** score_table, int** traceback_table, int* match_table);


#endif