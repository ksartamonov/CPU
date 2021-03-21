
#include "commands.h"
#include "CPU.h"

void Walk (int* asm_cmds, int ArraySize, CPU_t* prc);
int Command_Performer (int cmd1, int cmd2, int pc, CPU_t* prc);

int main (int argc, char* argv[])  // ./CPU #FileName#
{
  if ( argc < 2 ) { printf("ERROR: Not enough arguments!\n"); return -1; }

  CPU_t* Processor = (CPU_t*)calloc(1, sizeof(CPU_t));

  Stack_First_Init(10, &(Processor->stk));
  Stack_First_Init(10, &(Processor->callstk));


  FILE* f = fopen(argv[1], "r");

  long int SizeOfFile = GetSize(f);

  char* buf = ReadFile(argv[1]);

  int Lines_Amount = CounterOfLines(buf, SizeOfFile);

  char** P_Lines = (char**) calloc (Lines_Amount, sizeof (char*));

  assert(P_Lines != NULL);

  int* cmds = (int*)calloc(Lines_Amount, sizeof(int));

  PutPointers (buf, SizeOfFile, P_Lines, Lines_Amount);

  for (int idx = 0; idx < Lines_Amount; idx++)
  {
    cmds[idx] = atoi(P_Lines[idx]);
  }

  // for (int i = 0; i < 2*Lines_Amount; i++)
  // {
  //   printf("CMD[%d] = %d\n", i, cmds[i]);
  // }

  Walk(cmds, Lines_Amount, Processor);

  printf("All operations from %s are done! Processor stopped!\n", argv[1]);
  return 0;
}


//------------------------------------------------------------------------------


void Walk (int* asm_cmds, int ArraySize, CPU_t* prc)
{
  assert (asm_cmds != NULL);
  assert (prc != NULL);

  int PC = 0, new_PC = 0;

  while (asm_cmds[PC] != CMD_HLT)
    {
      PC = Command_Performer (asm_cmds[PC], asm_cmds[PC+1], PC, prc);
      // printf("PC = %d ||||||||| cmd = %d\n", PC, asm_cmds[PC]);
    }

}

//------------------------------------------------------------------------------

int Command_Performer (int cmd1, int cmd2, int pc, CPU_t* prc)
{
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
    printf("JUMP ADRESS = %d\n",pc);
    return pc;
  }

  if ( cmd1 == CMD_CALL)
  {
    pc = DO_JUMP(cmd1, cmd2, prc, pc);
    printf("CALL ADRESS = %d\n",pc);
    DO_CALLSTACK_PUSH(prc, pc);
    return pc;
  }

  if ( cmd1 == CMD_RET)
  {
    pc = Stack_Pop(prc->callstk, &pc);
    printf("RET ADRESS: %d\n", pc);
    return pc;
  }

  pc++;
  return pc;

}
