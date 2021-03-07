/*
В этом файле с помощью дефайна определить тип дефайна, включить stack.h
 После команды include  убираем дефайн #undef

Дальше опять дефайним другой тип и опять инклюдим stack.h, тогда получаем
двойную компилцию файла (при первой подстановке -- одного типа,
при второй -- другого)

*/

#include "commands.h"
#include "CPU.h"

void Walk (int* asm_cmds, int ArraySize, CPU_t* prc);
int Command_Performer (int cmd1, int cmd2, int pc, CPU_t* prc);

int main (int argc, char* argv[])  // ./CPU #FileName#
{
  if ( argc < 2 ) { printf("ERROR: Not enough arguments!\n"); return -1; }

  CPU_t* Processor = (CPU_t*)calloc(1, sizeof(CPU_t));

  Stack_First_Init(10, &(Processor->stk));

  FILE* f = fopen(argv[1], "r");

  long int SizeOfFile = GetSize(f);

  char* buf = ReadFile(argv[1]);

  int Lines_Amount = CounterOfLines(buf, SizeOfFile);

  printf("[debug]LINES AMOUNT: %d\n", Lines_Amount);

  char** P_Lines = (char**) calloc (Lines_Amount, sizeof (char*));

  assert(P_Lines != NULL);

  int* cmds = (int*)calloc(Lines_Amount, sizeof(int));

  PutPointers (buf, SizeOfFile, P_Lines, Lines_Amount);

  for (int idx = 0; idx < Lines_Amount; idx++)
  {
    cmds[idx] = atoi(P_Lines[idx]);
  }

  // for (int idx = 0; idx < Lines_Amount; idx++)
  // {
  //   printf("commands[%d] = %d\n", idx, cmds[idx]);

  Walk(cmds, Lines_Amount, Processor);

  return 0;
}


//------------------------------------------------------------------------------


void Walk (int* asm_cmds, int ArraySize, CPU_t* prc)
{
    int PC = 0, new_PC = 0, i = 0;

    assert(asm_cmds != NULL);

    while (asm_cmds[PC] != CMD_HLT && PC != ArraySize)
    {
      printf("PC = %d\n", PC);
      PC = Command_Performer (asm_cmds[PC], asm_cmds[PC+1], PC, prc);
    }

    printf("All operations are done! Processor stopped!\n");

}

//------------------------------------------------------------------------------

int Command_Performer (int cmd1, int cmd2, int pc, CPU_t* prc)
{
  if ( cmd1 == CMD_PUSH)
  {
    DO_PUSH(prc, cmd2);
    pc += 2;
  }

  if ( cmd1 == CMD_POP_RAX )
  {
   DO_REG_POP (prc, CMD_POP_RAX);
    pc ++;
  }

  if ( cmd1 == CMD_POP_RBX )
  {
   DO_REG_POP (prc, CMD_POP_RBX);
    pc ++;
  }

  if ( cmd1 == CMD_POP_RCX )
  {
   DO_REG_POP (prc, CMD_POP_RCX);
    pc ++;
  }

  if ( cmd1 == CMD_POP_RDX )
  {
   DO_REG_POP (prc, CMD_POP_RDX);
    pc ++;
  }

  if (cmd1 == CMD_ADD)
  {
   DO_ADD (prc);
    pc++;
  }

  if (cmd1 == CMD_SUB)
  {
   DO_SUB (prc);
    pc++;
  }

  if (cmd1 == CMD_DIV)
  {
   DO_DIV (prc);
    pc++;
  }

  if (cmd1 == CMD_IN)
  {
   DO_IN (prc);
    pc++;
  }

  if (cmd1 == CMD_MUL)
  {
   DO_MUL (prc);
    pc++;
  }

  if (cmd1 == CMD_FSQRT)
  {
   DO_FSQRT (prc);
    pc++;
  }

  if (cmd1 == CMD_OUT)
  {
   DO_OUT (prc);
    pc++;
  }

  if ( cmd1 == CMD_PUSH_RAX )
  {
   DO_REG_POP (prc, CMD_PUSH_RAX);
    pc++;
  }

  if ( cmd1 == CMD_PUSH_RBX )
  {
   DO_REG_POP (prc, CMD_PUSH_RBX);
    pc++;
  }

  if ( cmd1 == CMD_PUSH_RCX )
  {
   DO_REG_POP (prc, CMD_PUSH_RCX);
    pc++;
  }

  if ( cmd1 == CMD_PUSH_RDX )
  {
   DO_REG_POP (prc, CMD_PUSH_RDX);
    pc++;
  }

  if ( cmd1 == CMD_JMP || cmd1 == CMD_JB || cmd1 == CMD_JBE || cmd1 == CMD_JA || cmd1 == CMD_JAE || cmd1 == CMD_JE || cmd1 == CMD_JNE)
  {
    pc = DO_JUMP(cmd1, cmd2, prc, pc);
  }


  return pc;

}
