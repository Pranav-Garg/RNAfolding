#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

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
  - second argument is
    -> path to file containing the RNA sequence
*/
int main(int argument_count, char *arguments[]) //Let the program be run with input arguments. 1=Nussinov 2=Two Vector no arguments=ask the user
{
  if (argument_count != EXPECTED_ARGUMENT_COUNT) {
    print_usage();
  }
  else {
    char *sequence;
    read_sequence_from_file(&sequence, arguments[2]);
    printf("read sequence: %s", sequence);
  }
}
