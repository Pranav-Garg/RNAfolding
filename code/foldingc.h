#ifndef _FOLDINGC_H_
#define _FOLDINGC_H_

#ifndef MAXLINE
#define MAXLINE 1024
#endif

typedef struct cell {
  int max_value;
  int max_index;
} cell;

void two_vector(char *sequence, int sequence_length, int group_size, int ***traceback_table, int ***score_table);

/*
  group_size: the size of the preprocessing group 
  preprocessed_table: the resulting preprocessing table
*/
void preprocess(int group_size, cell ****preprocessed_table);

void print_preprocessing_table(cell ***table, int dimension);

void test_helpers();
#endif /* !defined(_FOLDINGC_H_) */
