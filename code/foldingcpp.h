#ifndef _FOLDINGCPP_H_INCLUDED_
#define _FOLDINGCPP_H_INCLUDED_

// nussinov
/*
void init_score_table(int** score_table); // 4 diagonal rows initialized as 0
void init_traceback_table(int** traceback_table);	//-2 for traceback end point
void nussinov(string &sequence, int** score_table, int** traceback_table); // -1 for diagonal, store k for traceback
void trace_back(string &sequence, int** score_table, int** traceback_table, int* match_table);
*/

bool test_compare_two_tables(int **table, int **table_2, int sequence_length);

#endif