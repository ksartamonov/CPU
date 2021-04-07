#include <iostream>
#include "commands.h"
#include "CPU.h"

void Walk (int* asm_cmds, int ArraySize, CPU_t* prc);
int Command_Performer (int* asm_cmds, int pc, CPU_t* prc);

int RAM[1024] = {};

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int main (int argc, char* argv[])  // ./CPU #FileName#
{
  if ( argc < 2 ) { printf("ERROR: Not enough arguments!\n"); return -1; }

  CPU_t* Processor = (CPU_t*)calloc(1, sizeof(CPU_t));

  Stack_First_Init(10, &(Processor->stk));
  Stack_First_Init(10, &(Processor->callstk));


  FILE* f = fopen(argv[1], "r");

  if (f == NULL)
  {
    {
      std::cout << _RED_ << "error: \x1b[0m" << _BOLD_ << "Could not find the file \x1b[0m" << _RED_ << argv[1] << "\n\x1b[0m"; //TODO: CORRECT COLOURS
      return NEED_NAME_OF_FILE;
    }
  }

  long int SizeOfFile = GetSize(f);

  char* buf = ReadFile(argv[1]);

  int Lines_Amount = CounterOfLines(buf, SizeOfFile);

  char** P_Lines = (char**) calloc (Lines_Amount, sizeof (char*));

  assert(P_Lines != NULL);

  //int* RAM = (int*)calloc(Lines_Amount, sizeof(int));

  PutPointers (buf, SizeOfFile, P_Lines, Lines_Amount);

  for (int idx = 0; idx < Lines_Amount; idx++)
  {
    RAM[idx] = atoi(P_Lines[idx]);
  }

  Walk(RAM, Lines_Amount, Processor);


  std::cout << _BOLD_ << "All operations from \x1b[0m" << _LIGHT_BLUE_ << argv[1] << " \x1b[0m" << _BOLD_ << "are done!" << "\n\x1b[0m";

  free(RAM);
  return 0;
}


//------------------------------------------------------------------------------


void Walk (int* RAM, int ArraySize, CPU_t* prc)
{
  assert (RAM != NULL);
  assert (prc != NULL);

  int PC = 0, new_PC = 0;

  while (RAM[PC] != CMD_HLT)
    {
      PC = Command_Performer (RAM, PC, prc);
    }

  for (int i = 0 ; i < ArraySize ; i ++)
  {
    printf("ARR[%d] = %d\n", i, RAM[i]);
  }
}

//------------------------------------------------------------------------------

int Command_Performer (int* asm_cmds, int pc, CPU_t* prc)
{
  int cmd1 = asm_cmds[pc];
  int cmd2 = asm_cmds[pc+1];
  int cmd3 = asm_cmds[pc+2];

  assert (prc != NULL);

  if ( cmd1 == CMD_PUSH)
  {
    DO_PUSH(prc, cmd2);
    pc ++;
  }

  if ( cmd1 == CMD_POP_RAX || cmd1 == CMD_POP_RBX || cmd1 == CMD_POP_RCX  || cmd1 == CMD_POP_RDX )
        DO_REG_POP (prc, cmd1);

  if (cmd1 == CMD_ADD)
        DO_ADD (prc);

  if (cmd1 == CMD_SUB)
        DO_SUB (prc);

  if (cmd1 == CMD_DIV)
        DO_DIV (prc);

  if (cmd1 == CMD_IN)
        DO_IN (prc);

  if (cmd1 == CMD_MUL)
        DO_MUL (prc);

  if (cmd1 == CMD_FSQRT)
        DO_FSQRT (prc);

  if (cmd1 == CMD_OUT)
        DO_OUT (prc);

  if ( cmd1 == CMD_PUSH_RAX || cmd1 == CMD_PUSH_RBX || cmd1 == CMD_PUSH_RCX || CMD_PUSH_RDX)
        DO_REG_PUSH (prc, cmd1);

  if ( cmd1 == CMD_JMP || cmd1 == CMD_JB || cmd1 == CMD_JBE || cmd1 == CMD_JA || cmd1 == CMD_JAE || cmd1 == CMD_JE || cmd1 == CMD_JNE )
  {
    pc = DO_JUMP(cmd1, cmd2, prc, pc);
    //printf("JUMP ADRESS = %d\n",pc);
    return pc;
  }

  if ( cmd1 == CMD_CALL)
  {
    DO_CALLSTACK_PUSH(prc, pc+2);
    pc = DO_JUMP(cmd1, cmd2, prc, pc);
    //printf("CALL ADRESS = %d\n",pc);
    return pc;
  }

  if ( cmd1 == CMD_RET)
  {
    pc = Stack_Pop(prc->callstk, &pc);
    //printf("RET ADRESS: %d\n", pc);
    return pc;
  }

  if ( cmd1 == CMD_MOV)
  {
    // cmd2 = value, cmd3 = adress
    DO_MOV(asm_cmds, cmd2, cmd3);
    pc += 2;
  }

  pc++;
  return pc;

}
