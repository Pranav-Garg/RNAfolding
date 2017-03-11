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

/**
 * e.g. i == 01110 (dec: 14), j == 00100 (dec: 4)
 *  (1) k == 0, sum_1 == 0,         sum_2 == 0,         max_val == 0, max_index == 0 
 *  (2) k == 1, sum_1 == 0+1,       sum_2 == 0+0,       max_val == 1, max_index == 1
 *  (3) k == 2, sum_1 == 0+1+1,     sum_2 == 0+0-1,     max_val == 1, max_index == 1
 *  (4) k == 3, sum_1 == 0+1+1+1,   sum_2 == 0+0-1+0,   max_val == 2, max_index == 3
 *  (5) k == 4, sum_1 == 0+1+1+1+0, sum_2 == 0+0-1+0+0, max_val == 2, max_index == 3
 */
void update_max(cell *preprocessing_cell, int horizontal_diff_vec, int vertical_diff_vec, int group_size) {
  int max_value = 0; 
  int max_index = 0;
  int sum_1 = 0;
  int sum_2 = 0;
  int k;
  for (k = 0; k < group_size; k++) {
    sum_1 += get_kth_bit(horizontal_diff_vec, group_size - k - 1);
    sum_2 += get_kth_bit(vertical_diff_vec, group_size - k - 1) * -1;
    if ((sum_1 + sum_2) > max_value) {
      max_value = sum_1 + sum_2;
      max_index = k;
    }
  }
  preprocessing_cell->max_value = max_value;
  preprocessing_cell->max_index = max_index;
}

void preprocess(int group_size, cell ****preprocessed_table) {
  // group_size - 1 because the first elements of vertical difference vector and
  // horizontal difference vector are always zero
  int table_dimension = pow(2, group_size - 1);
  int i, j;
  cell ***table = initialize_preprocessing_table(table_dimension);
  for (i = 0; i < table_dimension; i++) {
    for (j = 0; j < table_dimension; j++) {
      // here, index i and j act as difference vectors
      update_max(table[i][j], i, j, group_size);
    }
  }
  *preprocessed_table = table;
};

void print_preprocessing_table(cell ***table, int dimension) {
  int i, j;
  printf("%s\n", "------ debug ------");
  for (i = 0; i < dimension; i++) {
    printf("row %d", i);
    for (j = 0; j < dimension; j++) {
      if (j % 5 == 0) {
        printf("\n  ");
      }
      printf("(column:%d, max_val:%d, max_idx:%d) ", j, table[i][j]->max_value, table[i][j]->max_index);
    }
    printf("\n");
  }
}
