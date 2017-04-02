#include <stdio.h>
#include "foldingc.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

int binary_to_decimal(int *a, int length){
  int dec = 0;
  int i;
  for(i=0; i<length; i++){
    dec = dec | (a[length -1 -i] << i);
  }
  
  return dec;
}

void print_preprocessing_table(cell *table, int dimension) {
  int i, j;
  printf("%s\n", "------ debug ------");
  for (i = 0; i < dimension; i++) {
    printf("row %d", i);
    for (j = 0; j < dimension; j++) {
      if (j % 5 == 0) {
        printf("\n  ");
      }
      printf("(column:%d, max_val:%d, max_idx:%d) ", j, table[i * dimension + j].max_value, table[i * dimension + j].max_index);
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

char complement(char c){
  if(c=='A')
    return 'U';
  else if(c=='C')
    return 'G';
  else if(c=='G')
    return 'C';
  else if(c=='U')
    return 'A';
  else
    return 'A'; //should never happen with a proper sequence. Maybe add a function to check the input.
}

void compare(int *list, int length, int *value, int *argument){
  int i;
  *value = list[0];
  *argument = 0;
  for(i=1; i<length; i++){
    if(list[i]>*value){
      *value = list[i];
      *argument = i;
    }
  }
}

/**
 *  HELPERS END
 */
unsigned int *initialize_table(int row_dimension, int column_dimension) {
  unsigned int *table = (unsigned int *) malloc (row_dimension * column_dimension * sizeof(int));
  return table;
}

int *initialize_square_table(int length) {
  int *table = (int *) malloc (length * length * sizeof(int));
  return table;
}

cell *initialize_preprocessing_table(int table_dimension) {
  cell *table = (cell *) malloc(table_dimension * table_dimension * sizeof(cell));
  return table;
}

void free_preprocessed_table(cell *table) {
  free(table);
}

void update_max(cell *table, int table_dimension, unsigned int horizontal_diff_vec, unsigned int vertical_diff_vec, int group_size) {
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
  /* here horizontal_diff_vec is i index, vertical_diff_vec is j index, preprocessing_table is linearized */
  table[table_dimension * horizontal_diff_vec + vertical_diff_vec].max_value = max_value; 
  table[table_dimension * horizontal_diff_vec + vertical_diff_vec].max_index = max_index;
}

void preprocess(int group_size, cell **preprocessed_table) {
  // group_size - 1 because the first elements of vertical difference vector and
  // horizontal difference vector are always zero
  int table_dimension = pow(2, group_size - 1);
  unsigned int i, j;
  cell *table = initialize_preprocessing_table(table_dimension);
  for (i = 0; i < table_dimension; i++) {
    for (j = 0; j < table_dimension; j++) {
      // here, index i and j act as difference vectors
      update_max(table, table_dimension, i, j, group_size);
    }
  }
  *preprocessed_table = table;
}

// assume this simple scoring scheme for now TODO
int get_binding_score(char *sequence, int i, int j) {
  if (sequence[i] == complement(sequence[j])) {
    return 1;
  }
  return 0;
}

void print_aligned_output(int sequence_length, char *sequence, int *folded_pairs){
  int i;
  printf("\n");
  for(i=0; i<sequence_length; i++){
    printf("%d %c %d\n", i+1, sequence[i], folded_pairs[i]);
    
  }
  printf("\n");
}

void print_aligned_output_to_file(int sequence_length, char *sequence, int *folded_pairs, char *output_file_path){
  FILE *f_out = fopen(output_file_path, "w");
  int i;
  
  for(i=0; i<sequence_length; i++){
    fprintf(f_out, "%d %c %d\n", i+1, sequence[i], folded_pairs[i]);
    
  }
  fclose(f_out);
}

void read_sequence_from_file_noheaders(char **sequence, int *sequence_length, char *file_path) {
  char *sequence_buffer;
  FILE *file;
  unsigned long file_size;
  //Open file
  file = fopen(file_path, "rb");
  //Get file length
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  sequence_buffer = (char *) malloc(file_size);
  /* read and send buffer */
  fread(sequence_buffer, file_size, 1, file);
  fclose(file);
  int i;
  for (i = 0; i < file_size; i++) {
    if (sequence_buffer[i] == '\r' || sequence_buffer[i] == '\n') {
      sequence_buffer[i] = '\0';
    }
  }
  *sequence = sequence_buffer;
  *sequence_length = i;
}

void two_vector(char *sequence, int sequence_length, int group_size, int **traceback_table, int **score_table) {
  cell *preprocessed_table;
  int preprocessed_table_dimension = pow(2, group_size - 1);
  int maximum_group_count = (sequence_length / group_size) + 1;
  preprocess(group_size, &preprocessed_table);

  unsigned int group_bit_mask = 1;
  unsigned int left_most_bit_mask = 0;
  if (group_size > 0) {
    group_bit_mask = ((group_bit_mask << group_size) - 1);
    left_most_bit_mask = (group_bit_mask + 1) >> 1;
  }
  else {
    group_bit_mask = 0;
    left_most_bit_mask = 0;
  }
  unsigned int *horizontal_diff = initialize_table(sequence_length, maximum_group_count);
  unsigned int *vertical_diff = initialize_table(sequence_length, maximum_group_count);
  int *traceback = initialize_square_table(sequence_length);
  int *score = initialize_square_table(sequence_length);
  
  int i, j, k, m;
  
  int *left_most_difference_vector = (int *)malloc(sequence_length*sizeof(int));
  
  int straggler_number, group_number;  //straggler_number is number of cells that don't form groups in each row/column. 
                                       //group_number is number of groups
  int cells_to_compare;    //for each cell [i][j], this indicates how far it is from the diagonal
  int *comparison_list = (int *)malloc(sequence_length*sizeof(int));
  int *traceback_list = (int *)malloc(sequence_length*sizeof(int));
  int add_to_rowvect, add_to_colvect;  //These contain the first entry in each row/col vector before making it binary
  cell best_cell;  //stores answer from preprocessing table
  int *arg = (int *)malloc(sizeof(int));
  int *val = (int *)malloc(sizeof(int));    //stores maximum value and argument index from comparing
  
  printf("with group size %d, bit mast is %u, left mos bit mask is %u\n", group_size, group_bit_mask, left_most_bit_mask);
  for (j = 0; j < sequence_length; j++) {
    score[j * sequence_length + j] = 0;
    score[min(j+1, sequence_length - 1) * sequence_length + j] = 0;
    for(i = 0; i<MIN_LOOP_SIZE; i++){
      score[max(0, j-1-i) * sequence_length + j] = 0;
      traceback[max(0, j-1-i) * sequence_length + j] = 0;
    }
    traceback[min(j+1, sequence_length - 1) * sequence_length + j] = -3;
    traceback[j * sequence_length + j] = -2;
    
    for (i = j - 1 - MIN_LOOP_SIZE; i >= 0; i--) {
      score[i * sequence_length + j] = get_binding_score(sequence, i, j) + score[(i+1) * sequence_length + (j-1)];
      cells_to_compare = (j-i);
      straggler_number = cells_to_compare % group_size;
      group_number = cells_to_compare / group_size;
      for(k=1; k <= straggler_number; k++){
        //k is the index used to traverse the row and column simultaneously. k=1 is just next to the diagonal
        comparison_list[k] = score[i * sequence_length + (i+k-1)] + score[(i+k) * sequence_length + j];
        traceback_list[k] = k;
      }
      for(m = 0; m< group_number; m++){  
        add_to_rowvect = score[i * sequence_length + (i+k-1)];
    	add_to_colvect = score[(i+k) * sequence_length + j];
        
        //for(l=0; l<group_size-1; l++){
          // TODO: update this to use stack instead of heap memory
          //if (i == 0 && j < 100) 
          //  printf("score(%d,%d) = %d, score(%d,%d) = %d\n", i, (i+k+l), score[i * sequence_length + (i+k+l)], i, i+k+l-1, score[i * sequence_length + (i+k+l-1)]);
          //row_difference_vector[l] = score[i * sequence_length + (i+k+l)]-score[i * sequence_length + (i+k+l-1)];
          //col_difference_vector[l] = (score[(i+k+1+l) * sequence_length + j]-score[(i+k+l) * sequence_length + j])*-1;
    	//}
/*
        if (binary_to_decimal(col_difference_vector, group_size -1) != (vertical_diff[j * maximum_group_count + m])) {
          printf("--> max group count = %d\n", maximum_group_count);
          printf("--> sequence length = %d\n", sequence_length);
          printf("--> group size = %d\n", group_size);
          printf("--> (%d, %d) - at group %d col_difference_vector: %u\n", i, j, m, binary_to_decimal(col_difference_vector, group_size -1));    	
          printf("--> (%d, %d) - at group %d col_difference_vector constant lookup: %u\n", i, j, m, (vertical_diff[j * maximum_group_count + m]));    	
        }
*/
        best_cell = preprocessed_table[preprocessed_table_dimension * 
          (horizontal_diff[i * maximum_group_count + m] & (group_bit_mask >> 1)) + vertical_diff[j * maximum_group_count + m]];
        comparison_list[straggler_number + 1+m] = best_cell.max_value + add_to_rowvect + add_to_colvect;
        traceback_list[ straggler_number + 1+m] = best_cell.max_index + k;
    	
        k+=group_size;
      }
      comparison_list[0] = score[i * sequence_length + j];
      traceback_list[0] = -1;
      compare(comparison_list, straggler_number+group_number+1, val, arg);
      score[i * sequence_length + j] = *val;
      unsigned int this_row_difference = *val - score[i * sequence_length + (j - 1)];
      if (group_number == 0) {
        left_most_difference_vector[i] = (left_most_difference_vector[i] << 1) + this_row_difference;
        if (straggler_number + 1 == group_size) {
          horizontal_diff[i * maximum_group_count + group_number] = left_most_difference_vector[i];
        }
      }
      else {
        int group;
        unsigned int carry = this_row_difference;
        //printf("---> carry = %u\n", carry);
        unsigned int next_carry;
        for (group = group_number - 1; group >= 0; group--) {
          next_carry = ((horizontal_diff[i * maximum_group_count + group] & left_most_bit_mask) >> (group_size - 1));
          //printf("---> next_carry - %u\n", next_carry);
          //printf("---> applying carry - %u\n", carry);
          //printf("---> horizontal_diff[%d,%d] before: %u\n", i, group, horizontal_diff[i * maximum_group_count + group]);
          horizontal_diff[i * maximum_group_count + group] = ((horizontal_diff[i * maximum_group_count + group] << 1) + carry) & group_bit_mask;
          //printf("---> horizontal_diff[%d,%d] after: %u\n", i, group, horizontal_diff[i * maximum_group_count + group]);
          carry = next_carry;
        }
        if (straggler_number + 1 == group_size) {
          for (group = group_number; group > 0; group--) {
            horizontal_diff[i * maximum_group_count + group] = horizontal_diff[i * maximum_group_count + (group - 1)];
            //printf("--> updating horizontal diff vector i=%d, j=%d, g=%d: %u\n", i, j, group, horizontal_diff[i * maximum_group_count + (group - 1)]);
          }
          horizontal_diff[i * maximum_group_count + 0] = left_most_difference_vector[i];
          //printf("--> updating horizontal diff vector i=%d, j=%d, g=%d: to %u\n", i, j, group, left_most_difference_vector[i]);
        }
/*
        int j_prime;
        for (j_prime = 0; j_prime <= j; j_prime++) {
          printf("score[%d][%d]=%d  ", i, j_prime, score[i * sequence_length + j_prime]);
        }
        printf("\n");
        for (group = 0; group < 5; group++) {
          printf("--> horizontal diff vector i=%d, j=%d, g=%d: %u\n", i, j, group, horizontal_diff[i * maximum_group_count + group]);
        }
*/
      }
      unsigned int this_col_difference;
      /* column diff */
      if (straggler_number + 1 == group_size) {
        /* calculte col difference vector */
        unsigned int new_col_diff_vec = 0;
        this_col_difference = 0;
        int i_prime;
        for (i_prime = i; i_prime < i + group_size - 1; i_prime++) {
          this_col_difference = (score[(i_prime + 1) * sequence_length + j] - score[i_prime * sequence_length + j]) == -1 ? 1 : 0 ;
          new_col_diff_vec = (new_col_diff_vec << 1) | this_col_difference;
        }
        /* add the new col diff vec to the front */
        int group;
        for (group = group_number; group > 0; group--) {
          vertical_diff[j * maximum_group_count + group] = vertical_diff[j * maximum_group_count + (group - 1)];
          //printf("--> at (i, j) = (%d, %d), vertical_diff[%d][%d] = %u\n", i, j, j, group, vertical_diff[j * maximum_group_count + group]);
        }
        vertical_diff[j * maximum_group_count + 0] = new_col_diff_vec;
        //printf("--> at (i, j) = (%d, %d), vertical_diff[%d][%d] = %u\n", i, j, j, 0, vertical_diff[j * maximum_group_count + 0]);
      }
/*
      if (group_number == 0 && j == 100) {
        bottom_most_difference_vector[j] = (straggler_number == 1 ? this_col_difference : (this_col_difference << (straggler_number - 1))) | bottom_most_difference_vector[j];
        printf("this_col_difference: %u\n", this_col_difference);
        printf("j: %d\n", j);
        printf("bottom_most_difference_vector[%d]: %d\n", i, bottom_most_difference_vector[j]); 
        if (straggler_number + 1 == group_size) {
          printf("group_number: %d\n", group_number);
          vertical_diff[j * maximum_group_count + group_number] = bottom_most_difference_vector[j];
          printf("vertical diff vector i=%d, j=%d: %u\n", i, j, vertical_diff[j * maximum_group_count + group_number]);
        }
      }
      else if (j == 100) {
        int group;
        unsigned int carry = this_col_difference << (group_size - 1);
        unsigned int next_carry;
        for (group = 0; group < group_number; group++) {
          next_carry = ((vertical_diff[j * maximum_group_count + group] & 1) << (group_size - 1));
          vertical_diff[j * maximum_group_count + group] = carry | (vertical_diff[j * maximum_group_count + group] >> 1);
          carry = next_carry;
        }
        if (straggler_number + 1 == group_size) {
          vertical_diff[j * maximum_group_count + group_number] = bottom_most_difference_vector[j];
        }
      }
*/
      if(*arg == 0 && comparison_list[0] ==0){
        traceback[i * sequence_length + j] = 0;
      }
      else{
        traceback[i * sequence_length + j] = traceback_list[*arg];
      }
    
    }   
  }  
  *score_table = score;
  *traceback_table = traceback;
  // free preprocessed table memory
  free_preprocessed_table(preprocessed_table);
}

void nussinov(char *sequence, int sequence_length, int **traceback_table, int **score_table) {
  int *traceback = initialize_square_table(sequence_length);
  int *score = initialize_square_table(sequence_length);
  
  int i, j, k;
  int cells_to_compare;    //for each cell [i][j], this indicates how far it is from the diagonal
  int *comparison_list = (int *)malloc(sequence_length*sizeof(int));
  int *arg = (int *)malloc(sizeof(int));
  int *val = (int *)malloc(sizeof(int));    //stores maximum value and argument index from comparing
  
  for (j = 0; j < sequence_length; j++) {
    score[j * sequence_length + j] = 0;
    score[min(j+1, sequence_length - 1) * sequence_length + j] = 0;
    for(i = 0; i<MIN_LOOP_SIZE; i++){
      score[max(0, j-1-i) * sequence_length + j] = 0;
      traceback[max(0, j-1-i) * sequence_length + j] = 0;
    }
    traceback[min(j+1, sequence_length - 1) * sequence_length + j] = -3;
    traceback[j * sequence_length + j] = -2;
    
    for (i = j - 1 - MIN_LOOP_SIZE; i >= 0; i--) {
      score[i * sequence_length + j] = get_binding_score(sequence, i, j) + score[(i+1) * sequence_length + (j-1)];
      cells_to_compare = (j-i);
      for(k=1; k<=cells_to_compare; k++){
        //k is the index used to traverse the row and column simultaneously. k=1 is just below the current cell / diagonal of the row.
        comparison_list[k] = score[i * sequence_length + (i+k-1)] + score[(i+k) * sequence_length + j];
      }
      comparison_list[0] = score[i * sequence_length + j];
      compare(comparison_list, cells_to_compare+1, val, arg);
      score[i * sequence_length + j] = *val;
      traceback[i * sequence_length + j] = *arg;
      if(*arg ==0){
        if(comparison_list[0] == 0)
          traceback[i * sequence_length + j] = 0;
        else
          traceback[i * sequence_length + j] = -1;
      }
    }   
  }  
  *score_table = score;
  *traceback_table = traceback;
}

void traceback(char *sequence, int sequence_length, int *traceback_table, int **folded_pairs, int i_start, int j_start){
  int k = traceback_table[i_start * sequence_length + j_start];
  
  if (k==-2){
    (*folded_pairs)[i_start] = 0;
    return;
  }
  
  else if (k==-3){
    return;
  }
  
  else if (k == -1){
    (*folded_pairs)[i_start] = j_start+1;
    (*folded_pairs)[j_start] = i_start+1;
    traceback(sequence, sequence_length, traceback_table, folded_pairs, i_start+1, j_start-1);
  }
  
  else if(k==0){
    (*folded_pairs)[i_start] = 0;
    (*folded_pairs)[j_start] = 0;
    traceback(sequence, sequence_length, traceback_table, folded_pairs, i_start+1, j_start-1);
  }
  else{
    
    traceback(sequence, sequence_length, traceback_table, folded_pairs, i_start, i_start + k -1);
    traceback(sequence, sequence_length, traceback_table, folded_pairs, i_start+k, j_start);
  }
}

/**
 *  TESTS
 */


void print_tables(int sequence_length, int *score_table, int *traceback_table){
  printf("\n----Score Table----\n");
  int i,j;
  for(i=0; i<sequence_length; i++){
    for(j=0; j<sequence_length; j++){
      printf("%d\t", score_table[i * sequence_length + j]);
    }
    printf("\n");
  }
  printf("\n----End of Score Table----\n");
  printf("\n----Traceback Table----\n");
  
  for(i=0; i<sequence_length; i++){
    for(j=0; j<sequence_length; j++){
      printf("%d\t", traceback_table[i * sequence_length + j]);
    }
    printf("\n");
  }
  
  printf("\n----End of Traceback Table----\n");
  
}

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


