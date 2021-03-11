

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

  Walk(cmds, Lines_Amount, Processor);

  return 0;
}


//------------------------------------------------------------------------------


void Walk (int* asm_cmds, int ArraySize, CPU_t* prc)
{
    int PC = 0, new_PC = 0, i = 0;

    assert(asm_cmds != NULL);

    while (asm_cmds[i] != CMD_HLT)
    {
      printf("PC = %d\n", PC);
      i++;
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

  if ( cmd1 == CMD_JMP || cmd1 == CMD_JB || cmd1 == CMD_JBE || cmd1 == CMD_JA || cmd1 == CMD_JAE || cmd1 == CMD_JE || cmd1 == CMD_JNE )
  {
    pc = DO_JUMP(cmd1, cmd2, prc, pc) + 1;
  }

  if ( cmd1 == CMD_CALL)
  {
    pc = DO_JUMP(cmd1, cmd2, prc, pc) + 1;
    DO_CALLSTACK_PUSH(prc, pc);
  }

  if ( cmd1 == CMD_RET)
  {
    Stack_Pop(prc->callstk, &pc);
  }
  return pc;

}
