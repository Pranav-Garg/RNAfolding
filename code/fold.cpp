#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

using namespace std;

extern "C" {
	#include "foldingc.h"	//This library will contain all C functions
}

#include "foldingcpp.h"		//This library will contain all C++ functions

const int EXPECTED_ARGUMENT_COUNT = 3;

void print_usage() {
  printf("expected arguments:\n");
  printf("* first argument is\n");
  printf("  -> 1: Nussinov\n");
  printf("  -> 2: Two Vector\n");
  printf("* second argument is\n");
  printf("  -> path to file containing the RNA sequence\n");
}

void test_preprocessed_table() {
  cell ***preprocessed_table;
  int group_size = 5;
  preprocess(group_size, &preprocessed_table);  
  // a couple tests
  /**
  * e.g. i == 01110 (dec: 14), j == 00100 (dec: 4)
  *  (1) k == 0, sum_1 == 0,         sum_2 == 0,         max_val == 0, max_index == 0 
  *  (2) k == 1, sum_1 == 0+1,       sum_2 == 0+0,       max_val == 1, max_index == 1
  *  (3) k == 2, sum_1 == 0+1+1,     sum_2 == 0+0-1,     max_val == 1, max_index == 1
  *  (4) k == 3, sum_1 == 0+1+1+1,   sum_2 == 0+0-1+0,   max_val == 2, max_index == 3
  *  (5) k == 4, sum_1 == 0+1+1+1+0, sum_2 == 0+0-1+0+0, max_val == 2, max_index == 3
  */
  if (preprocessed_table[14][4]->max_value != 2 || preprocessed_table[14][4]->max_index != 3) {
    printf("ERROR: expected (max_value == 2, max_index == 3)\n");
    printf("       but received (max_value == %d, max_index == %d\n", preprocessed_table[14][4]->max_value, preprocessed_table[14][4]->max_index);
  }
  else {
    printf("preprocessd table test 1: passed\n");
  }
  /**
  * e.g. i == 00111 (dec: 7), j == 01100 (dec: 12)
  *  (1) k == 0, sum_1 == 0,         sum_2 == 0,         max_val == 0, max_index == 0 
  *  (2) k == 1, sum_1 == 0+0,       sum_2 == 0-1,       max_val == 0, max_index == 0
  *  (3) k == 2, sum_1 == 0+0+1,     sum_2 == 0-1-1,     max_val == 0, max_index == 0
  *  (4) k == 3, sum_1 == 0+0+1+1,   sum_2 == 0-1-1+0,   max_val == 0, max_index == 0
  *  (5) k == 4, sum_1 == 0+0+1+1+1, sum_2 == 0-1-1+0+0, max_val == 1, max_index == 4
  */
  if (preprocessed_table[7][12]->max_value != 1 || preprocessed_table[7][12]->max_index != 4) {
    printf("ERROR: expected (max_value == 1, max_index == 4)\n");
    printf("       but received (max_value == %d, max_index == %d\n", preprocessed_table[7][12]->max_value, preprocessed_table[7][12]->max_index);
  }
  else {
    printf("preprocessd table test 2: passed\n");
  }
}

void get_rna_file_dimension(char *first_line, int *row_count, int *column_count) {
  sscanf(first_line, "%d %d", row_count, column_count);
  printf("reading RNA Sequence file, row count: %d, column count: %d\n", *row_count, *column_count);
}

void read_sequence_from_file(char **sequence, char *file_path) {
  FILE* file = fopen(file_path, "r");
  int line_count;
  int line_size;
  char line[256];
  int index;
  fgets(line, sizeof(line), file);
  get_rna_file_dimension(line, &line_count, &line_size);
  char *read_buffer = (char *) malloc(line_count * line_size * sizeof(char));
  for (index = 0; index < (line_count * line_size); index += line_size) {
    fgets(line, sizeof(line), file);
    strcpy(&read_buffer[index], line);
  }
  fclose(file);
  *sequence = read_buffer;
}

/*
  expect two command line arguments, 
  - first argument is
    -> 1: Nussinov
    -> 2: Two Vector
    -> 3: Testing
  - second argument is
    -> path to file containing the RNA sequence
*/
int main(int argument_count, char *arguments[]) //Let the program be run with input arguments. 1=Nussinov 2=Two Vector no arguments=ask the user
{
  if (argument_count != EXPECTED_ARGUMENT_COUNT) {
    print_usage();
  }
  else {
    int first_argument = atoi(arguments[1]);
    char *second_argument = arguments[2];
    if (first_argument == 3) {
      test_preprocessed_table();
    }
    char *sequence;
    read_sequence_from_file(&sequence, second_argument);
    printf("read sequence: %s", sequence);
  }
}
