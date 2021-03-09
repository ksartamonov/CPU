

#ifndef COMMANDS_H
#define COMMANDS_H

#include "CPU.h"
#include "Stack.h"

enum Commands {
  CMD_PUSH     = 10,  // Pushes into a stack                                        (needs 2 arguments)
  CMD_ADD      = 2,  // Summs pre-last and last element of stack
  CMD_SUB      = 3,  // Subtracts the last element from the pre-last
  CMD_DIV      = 4,  // Divides pre-last number on the last
  CMD_IN       = 5,  // keyboard input command
  CMD_MUL      = 6,  // Multiplies pre-last and the last elements
  CMD_FSQRT    = 7,  // calculates the square root of the last element
  CMD_OUT      = 8,  // prints the stack's top element
              CMD_JMP     = 30,  // To loop the operations
  CMD_HLT      = 64, // stops preprocessor
  //CMD_END      = -1,  // quits the program
  //CMD_REG_PUSH = 11,
  CMD_PUSH_RAX = 12,
  CMD_PUSH_RBX = 13,
  CMD_PUSH_RCX = 14,
  CMD_PUSH_RDX = 15,
  CMD_POP_RAX  = 16,
  CMD_POP_RBX  = 17,
  CMD_POP_RCX  = 18,
  CMD_POP_RDX  = 19,
  CMD_JB       = 31,
  CMD_JBE      = 32,
  CMD_JA       = 33,
  CMD_JAE      = 34,
  CMD_JE       = 35,
  CMD_JNE      = 36,
  CMD_CALL     = 40,
  CMD_RET      = 50
};


int DO_PUSH (CPU_t* proc, int value) // PUSH x
{
  assert(proc != NULL);
  Stack_Push((proc->stk), value);
  return 1;
}

//------------------------------------------------------------------------------

int DO_ADD (CPU_t* proc)
{
  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Stack_Pop((proc->stk), &Last);
  Stack_Pop((proc->stk), &PreLast);

  int sum = Last + PreLast;

  Stack_Push((proc->stk), sum);
  return 1;
}

//------------------------------------------------------------------------------

int DO_SUB (CPU_t* proc)
{
  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Stack_Pop((proc->stk), &Last);
  Stack_Pop((proc->stk), &PreLast);

  int sub = PreLast - Last;

  Stack_Push((proc->stk), sub);
  return 1;
}

//------------------------------------------------------------------------------

int DO_DIV (CPU_t* proc)
{
  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Stack_Pop((proc->stk), &Last);
  Stack_Pop((proc->stk), &PreLast);

  int div = PreLast / Last;
  int rem = PreLast % Last;

  Stack_Push((proc->stk), div);
  Stack_Push((proc->stk), rem);

  return 1;
}

//------------------------------------------------------------------------------

int DO_MUL (CPU_t* proc)
{
  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Stack_Pop((proc->stk), &Last);
  Stack_Pop((proc->stk), &PreLast);

  int mul = PreLast / Last;

  Stack_Push((proc->stk), mul);
  return 1;
}

//------------------------------------------------------------------------------

int DO_IN (CPU_t* proc)
{
  printf("Enter the value in console:\n");
  int x = 0;
  if (scanf("%d", &x) != 1)
      {
        printf("ERROR: Could not scan value!\n");
        return -1;
      }

  Stack_Push((proc->stk), x);
  return 1;
}

//------------------------------------------------------------------------------

int DO_FSQRT(CPU_t* proc)
{
  if ((proc->stk->stack_size) < 1 )
  {
  printf("ERROR: Not enough elements in stack\n");
  return -1;
  }

  int Last = 0;
  Stack_Pop((proc->stk), &Last);

  Last = sqrt(Last);

  Stack_Push((proc->stk), Last);

  return 1;
}

//------------------------------------------------------------------------------

int DO_OUT (CPU_t* proc)
{
  if ((proc->stk->stack_size) < 1 )
  {
  printf("ERROR: Not enough elements in stack\n");
  return -1;
  }

  int Last = 0;
  Stack_Pop((proc->stk), &Last);

  printf("Top element: %d\n", Last);

  Stack_Push((proc->stk), Last);

  return 1;
}

//------------------------------------------------------------------------------

// int DO_HLT (CPU_t* proc)
// {
//   printf("Programm stopped!\n");
//   //abort();
// }

//------------------------------------------------------------------------------

int DO_REG_PUSH (CPU_t* proc, int rx)
{
  switch(rx){

  case CMD_PUSH_RAX:
        {
          Stack_Push((proc->stk), (proc->rax));
          return 1;
        }

  case CMD_PUSH_RBX:
        {
          Stack_Push((proc->stk), (proc->rbx));
          return 1;
        }

  case CMD_PUSH_RCX:
        {
          Stack_Push((proc->stk), (proc->rcx));
          return 1;
        }

  case CMD_PUSH_RDX:
        {
          Stack_Push((proc->stk), (proc->rdx));
          return 1;
        }
  default:
          return -1;
  }
}

//------------------------------------------------------------------------------

int DO_REG_POP (CPU_t* proc, int rx)
{

  if ((proc->stk->stack_size) < 1 )
  {
  printf("ERROR: Not enough elements in stack\n");
  return -1;
  }

  switch(rx){

  case CMD_POP_RAX:
        {
          Stack_Pop (proc->stk, &(proc->rax) );
          return 1;
        }

  case CMD_POP_RBX:
        {
          Stack_Pop((proc->stk), (&(proc->rbx)));
          return 1;
        }

  case CMD_POP_RCX:
        {
          Stack_Pop((proc->stk), (&(proc->rbx)));
          return 1;
        }

  case CMD_POP_RDX:
        {
          Stack_Pop((proc->stk), (&(proc->rdx)));
          return 1;
        }
  default:
          return -1;
  }
}

//------------------------------------------------------------------------------

int DO_JUMP (int JUMP_type, int JumpAdress, CPU_t* proc, int IP)
{

  int El_1 = 0, El_2 = 0;

  switch(JUMP_type){

    case CMD_JMP:
        {
          IP = JumpAdress;
          return IP;
        }

    case CMD_JB:
        {
          if (proc->stk->size < 2 ) printf("ERROR: Not enough elements in stack!\n");
          Stack_Pop(proc->stk, &(El_1));
          Stack_Pop(proc->stk, &(El_2));
          if (El_1 < El_2)
                IP = JumpAdress;

          return IP;
        }

    case CMD_JBE:
        {
          if (proc->stk->size < 2 ) printf("ERROR: Not enough elements in stack!\n");
          Stack_Pop(proc->stk, &(El_1));
          Stack_Pop(proc->stk, &(El_2));
          if (El_1 <= El_2)
                IP = JumpAdress;

          return IP;
        }

    case CMD_JA:
          {
            if (proc->stk->size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            Stack_Pop(proc->stk, &(El_1));
            Stack_Pop(proc->stk, &(El_2));
            if (El_1 > El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_JAE:
          {
            if (proc->stk->size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            Stack_Pop(proc->stk, &(El_1));
            Stack_Pop(proc->stk, &(El_2));
            if (El_1 >= El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_JE:
          {
            if (proc->stk->size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            Stack_Pop(proc->stk, &(El_1));
            Stack_Pop(proc->stk, &(El_2));
            if (El_1 == El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_JNE:
          {
            if (proc->stk->size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            Stack_Pop(proc->stk, &(El_1));
            Stack_Pop(proc->stk, &(El_2));
            if (El_1 != El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_CALL:
          {
            IP = JumpAdress;
            return IP;
          }
  }
  return IP;
}

int DO_CALLSTACK_PUSH (CPU_t* proc, int IP)
{
  assert(proc->callstk != NULL);
  Stack_Push(proc->callstk, IP);
  return 1;
}
//------------------------------------------------------------------------------


#endif  // COMMANDS_H
