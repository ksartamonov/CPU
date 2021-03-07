
#ifndef CPU_H
#define CPU_H


#include "File_Operations.h"
#include "Standart_Libriaries.h"
#include "Stack.h"


typedef struct CPU {
  Stack* stk;
  char* code;
  int* RAM; //array with commands

  int top;
  long int FileSize;

  int rax;
  int rbx;
  int rcx;
  int rdx;

} CPU_t;

#endif // CPU_H
