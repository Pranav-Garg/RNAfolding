#include <iostream>

extern "C" {
	#include "foldingc.h"	//This library will contain all C functions
}

#include "foldingcpp.h"		//This library will contain all C++ functions

int main() //Let the program be run with input arguments. 1=Nussinov 2=Two Vector no arguments=ask the user
{
  std::cout << "Hello World!\n";
  
  //Read input sequence
  //if input==1, call Nussinov functions using input sequence.
  //if input==2, call Two-Vector functions using input sequence.
  //Write output to file
  //Output run-time in hh:mm:ss.sss
}