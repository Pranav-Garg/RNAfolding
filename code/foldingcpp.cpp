#include <iostream>

#include <stdio.h>
#include "foldingcpp.h"
using namespace std;

extern "C" {
	#include "foldingc.h"	//This library will contain all C functions
}

bool test_compare_two_tables(int **table, int **table_2, int sequence_length){
  int i,j, k;
  
  for(j=0; j<sequence_length; j++){
    k = min(j+1, sequence_length-1);
    for(i=k; i>=0; i--){
      if(table[i][j] != table_2[i][j])
        return false;
    }
  }
  return true;
  
}