#include <stdio.h>
#include "foldingc.h"
#include <math.h>
#include <stdlib.h>

/**
 *  HELPERS START
 */

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

int min(int a, int b) {
  if (a < b) {
    return a;
  }
  return b;
}

int get_kth_bit(unsigned int v, int k) {
  return ((v & ( 1 << k )) >> k);
}

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

int get_group_row_index(int row, int group_size) {
  return (row / group_size);
}

int get_group_column_index(int column, int group_size) {
  return (column / group_size);
}

int get_left_most_group_right_bound(int row_index, int group_size) {
  return ((row_index - (row_index % group_size)) + group_size)-1;
}

int get_current_group_left_bound(int row_index, int column_index, int group_size) {
  int group_row_index = get_group_row_index(row_index, group_size);
  int group_column_index = get_group_column_index(column_index, group_size);
  if (group_row_index == group_column_index) {
    return row_index;
  }
  else {
    return group_column_index * group_size;
  }
}

int get_current_group_right_bound(int row_index, int column_index, int group_size, int sequence_length) {
  int group_column_index = get_group_column_index(column_index, group_size);
  int right_bound = (group_column_index * group_size) + (group_size - 1);
  return min(right_bound, sequence_length - 1);
}

/**
 *  HELPERS END
 */

int **initialize_square_table(int length) {
  int i;
  int **table = (int **) malloc(length * sizeof(int *));
  for (i = 0; i < length; i++) {
    table[i] = (int *) malloc(length * sizeof(int));
  }
  return table;
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

void free_preprocessed_table(cell ***table, int dimension) {
  int i, j;
  for (i = 0; i < dimension; i++) {
    for (j = 0; j < dimension; j++) {
      free(table[i][j]);
    }
    free(table[i]);
  }
  free(table);
}

void update_max(cell *preprocessing_cell, unsigned int horizontal_diff_vec, unsigned int vertical_diff_vec, int group_size) {
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
  unsigned int i, j;
  cell ***table = initialize_preprocessing_table(table_dimension);
  for (i = 0; i < table_dimension; i++) {
    for (j = 0; j < table_dimension; j++) {
      // here, index i and j act as difference vectors
      update_max(table[i][j], i, j, group_size);
    }
  }
  *preprocessed_table = table;
};

// assume this simple scoring scheme for now TODO
int get_binding_score(char *sequence, int i, int j) {
  if (sequence[i] == sequence[j]) {
    return 1;
  }
  return 0;
}


void two_vector(char *sequence, int sequence_length, int group_size, int ***traceback_table, int ***score_table) {
  cell ***preprocessed_table;
  int preprocessed_table_dimension = pow(2, group_size - 1);
  preprocess(group_size, &preprocessed_table);

  // test
  printf("test accessing preprocessed table value: %d\n", preprocessed_table[0][0]->max_value);
  printf("sequence: %s\n", sequence);
  printf("sequence length: %d\n", sequence_length);
  printf("group size: %d\n", group_size);
  
  int **traceback = initialize_square_table(sequence_length);
  int **score = initialize_square_table(sequence_length);
  
  int i, j;
  for (j = 0; j < sequence_length; j++) {
    score[j][j] = 0;
    score[min(j+1, sequence_length - 1)][j] = 0;
    for (i = j - 1; i >= 0; i--) {
      score[i][j] = get_binding_score(sequence, i, j) + score[i+1][j-1]; // assume score to be 1 for now 
      // TODO
    }   
  }  

  // free preprocessed table memory
  free_preprocessed_table(preprocessed_table, preprocessed_table_dimension);
}

/**
 *  TESTS
 */

void test_helpers() {
  // test get_kth_bit(unsigned int v, unsigned int k)
  if (get_kth_bit(64, 6) != 1) {
    printf("Expected get_kth_bit(0100 0000, 6) to produce 1, but given %d\n", get_kth_bit(64, 6));
  }
  else {
    printf("two vector helper, get_kth_bit, test 1: passed\n");
  }

  if (get_kth_bit(128, 6) != 0) {
    printf("Expected get_kth_bit(1000 0000, 6) to produce 0, but given %d\n", get_kth_bit(128, 6));
  }
  else {
    printf("two vector helper, get_kth_bit, test 2: passed\n");
  }

  if (get_group_row_index(17, 3) != 5) {
    printf("Expected get_group_row_index(17, 3) to produce 5, but given %d\n", get_group_row_index(17, 3));
  }
  else {
    printf("two vector helper, get_group_row_index, test 3: passed\n");
  }

  if (get_group_column_index(17, 18) != 0) {
    printf("Expected get_group_column_index(17, 18) to produce 0, but given %d\n", get_group_column_index(17, 18));
  }
  else {
    printf("two vector helper, get_group_column_index, test 4: passed\n");
  }

  if (get_current_group_left_bound(17, 19, 5) != 17) {
    printf("Expected get_current_group_left_bound(17, 19, 5) to produce 17, but given %d\n", get_current_group_left_bound(17, 19, 5));
  }
  else {
    printf("two vector helper, get_current_group_left_bound, test 5: passed\n");
  }
  
  if (get_current_group_left_bound(17, 25, 5) != 25) {
    printf("Expected get_current_group_left_bound(17, 25, 5) to produce 25, but given %d\n", get_current_group_left_bound(17, 25, 5));
  }
  else {
    printf("two vector helper, get_current_group_left_bound, test 6: passed\n");
  }

  if (get_current_group_right_bound(17, 19, 5, 40) != 19) {
    printf("Expected get_current_group_right_bound(17, 19, 5, 40) to produce 19, but given %d\n", get_current_group_right_bound(17, 19, 5, 40));
  }
  else {
    printf("two vector helper, get_current_group_right_bound, test 7: passed\n");
  }

  if (get_current_group_right_bound(17, 36, 5, 39) != 38) {
    printf("Expected get_current_group_right_bound(17, 36, 5, 39) to produce 38, but given %d\n", get_current_group_right_bound(17, 36, 5, 39));
  }
  else {
    printf("two vector helper, get_current_group_right_bound, test 8: passed\n");
  }
}
