#ifndef _FOLDINGCPP_H_INCLUDED_
#define _FOLDINGCPP_H_INCLUDED_

//various c++ function declarations

// nussinov
void init_score_table(int** score_table); // 4 diagonal rows initialized as 0
void init_traceback_table(int** traceback_table);	//-2 for traceback end point
void nussinov(string sequence, int** score_table, int** traceback_table); // -1 for diagonal, store k for traceback
void traceback(string sequence, int** score_table, int** traceback_table, int* match_table);


#endif