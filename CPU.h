
#ifndef CPU_H
#define CPU_H


#include "File_Operations.h"
#include "Standart_Libriaries.h"
#include "Stack.h"

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

const int RAM_SIZE         = 5000;
const int VIDEOMEM_SIZE    = 3000;
const int VIDEOMEM_LENGTH  = 100;
const int VIDEOMEM_START   = 1000;

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

typedef struct CPU {
  Stack* stk;


  int top;
  long int FileSize;

  int rax;
  int rbx;
  int rcx;
  int rdx;

  Stack* callstk;
} CPU_t;

#endif // CPU_H
