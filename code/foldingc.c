#include <stdio.h>
#include "foldingc.h"
#include <math.h>
#include <stdlib.h>

void nussinov(int n, char seq[], int structure[]){
	if(0==0);
}

cell ***initialize_preprocessing_table(int table_dimension) {
  int i, j;
  cell ***table = (cell ***) malloc(table_dimension * sizeof(cell **));
  for (i = 0; i < table_dimension; i++) {
    table[i] = (cell **) malloc(table_dimension * sizeof(cell *));
  }
  for (i = 0; i < table_dimension; i++) {
    for (j = 0; j < table_dimension; j++) {
      table[i][j] = (cell *) malloc(table_dimension * sizeof(cell));
    }
  }
  return table;
}

int get_kth_bit(int v, int k) {
  return ((v & ( 1 << k )) >> k);
}

void update_max(cell *preprocessing_cell, int horizontal_diff_vec, int vertical_diff_vec, int table_dimension) {
  int max_value = 0; 
  int max_index = 0;
  int sum_1 = 0;
  int sum_2 = 0;
  int k;
  for (k = 0; k < table_dimension; k++) {
    sum_1 += get_kth_bit(horizontal_diff_vec, k);
    sum_2 += get_kth_bit(vertical_diff_vec, k) * -1;
    if ((sum_1 + sum_2) > max_value) {
      max_value = sum_1 + sum_2;
      max_index = k;
    }
  }
  preprocessing_cell->max_value = max_value;
  preprocessing_cell->max_index = max_index;
}

void preprocess(int group_size, cell ****preprocessed_table) {
  // with table_dimension being an int, this effectively restricts group_size to be 1 to 64
  int table_dimension = pow(2, group_size);
  int i, j;
  cell ***table = initialize_preprocessing_table(table_dimension);
  for (i = 0; i < table_dimension; i++) {
    for (j = 0; j < table_dimension; j++) {
      update_max(table[i][j], i, j, table_dimension);
    }
  }
  *preprocessed_table = table;
};
