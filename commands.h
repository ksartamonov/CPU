

#ifndef COMMANDS_H
#define COMMANDS_H


#include "CPU.h"
#include "Stack.h"

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

enum Commands {
  CMD_PUSH     = 10,  // Pushes into a stack                                        (needs 2 arguments)
  CMD_ADD      = 2,  // Summs pre-last and last element of stack
  CMD_SUB      = 3,  // Subtracts the last element from the pre-last
  CMD_DIV      = 4,  // Divides pre-last number on the last
  CMD_IN       = 5,  // keyboard input command
  CMD_MUL      = 6,  // Multiplies pre-last and the last elements
  CMD_FSQRT    = 7,  // calculates the square root of the last element
  CMD_OUT      = 8,  // prints the stack's top element
  CMD_JMP      = 30, // To loop the operations
  CMD_HLT      = 64, // stops preprocessor
  CMD_PUSH_RAX = 12, // pushes rax into stack
  CMD_PUSH_RBX = 13, // pushes rbx into stack
  CMD_PUSH_RCX = 14, // pushes rcx into stack
  CMD_PUSH_RDX = 15, // // pushes rdx into stack
  CMD_POP_RAX  = 16, // pops to rax from stack
  CMD_POP_RBX  = 17, // pops to rbx from stack
  CMD_POP_RCX  = 18, // pops to rcx from stack
  CMD_POP_RDX  = 19, // pops to rdx from stack
  CMD_JB       = 31, // jump if below
  CMD_JBE      = 32, // jump if equal or below
  CMD_JA       = 33, //
  CMD_JAE      = 34, //
  CMD_JE       = 35, //
  CMD_JNE      = 36, //
  CMD_CALL     = 40, // Jumps to fuction and uses callstack
  CMD_RET      = 50,  // returns to position from callstack
  CMD_MOV      = 70, // uses RAM
  CMD_VISUAL   = 128
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
  assert(proc != NULL);

  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack to ADD\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Last    = Stack_Pop((proc->stk), &Last);
  PreLast = Stack_Pop((proc->stk), &PreLast);

  int sum = Last + PreLast;

  Stack_Push((proc->stk), sum);
  return 1;
}

//------------------------------------------------------------------------------

int DO_SUB (CPU_t* proc)
{
  assert(proc != NULL);

  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack to SUB\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Last    = Stack_Pop((proc->stk), &Last);
  PreLast = Stack_Pop((proc->stk), &PreLast);

  int sub = PreLast - Last;

  Stack_Push((proc->stk), sub);
  return 1;
}

//------------------------------------------------------------------------------

int DO_DIV (CPU_t* proc)
{
  assert(proc != NULL);

  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack to DIV\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Last = Stack_Pop((proc->stk), &Last);
  PreLast = Stack_Pop((proc->stk), &PreLast);

  int div = PreLast / Last;
  int rem = PreLast % Last;

  Stack_Push((proc->stk), div);
  Stack_Push((proc->stk), rem);

  return 1;
}

//------------------------------------------------------------------------------

int DO_MUL (CPU_t* proc)
{
  assert(proc != NULL);

  if ((proc->stk->stack_size) < 2 )
  {
  printf("ERROR: Not enough elements in stack to MUL\n");
  return -1;
  }

  int Last = 0;
  int PreLast = 0;

  Last    = Stack_Pop((proc->stk), &Last);
  PreLast = Stack_Pop((proc->stk), &PreLast);

  int mul = PreLast * Last;

  Stack_Push((proc->stk), mul);
  return 1;
}

//------------------------------------------------------------------------------

int DO_IN (CPU_t* proc)
{
  assert(proc != NULL);

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
  assert(proc != NULL);

  if ((proc->stk->stack_size) < 1 )
  {
  printf("ERROR: Not enough elements in stack to FSQRT\n");
  return -1;
  }

  int Last = 0;
  Last = Stack_Pop((proc->stk), &Last);

  Last = sqrt(Last);

  Stack_Push((proc->stk), Last);

  return 1;
}

//------------------------------------------------------------------------------

int DO_OUT (CPU_t* proc)
{
  assert(proc != NULL);

  if ((proc->stk->stack_size) < 1 )
  {
  printf("ERROR: Not enough elements in stack to OUT\n");
  return -1;
  }

  int Last = 0;

  Last = Stack_Pop((proc->stk), &Last);
  printf("Top element: %d\n", Last);

  //Stack_Push((proc->stk), Last);

  return 1;
}

//------------------------------------------------------------------------------


int DO_REG_PUSH (CPU_t* proc, int rx)
{
  assert(proc != NULL);

  switch(rx){

  case CMD_PUSH_RAX:
        {
          Stack_Push((proc->stk), (proc->rax));
          printf("PUSHING RAX = %d\n", proc->rax);
          return 1;
        }

  case CMD_PUSH_RBX:
        {
          Stack_Push((proc->stk), (proc->rbx));
          printf("PUSHING RBX = %d\n", proc->rbx);
          return 1;
        }

  case CMD_PUSH_RCX:
        {
          Stack_Push((proc->stk), (proc->rcx));
          printf("PUSHING RCX = %d\n", proc->rcx);
          return 1;
        }

  case CMD_PUSH_RDX:
        {
          Stack_Push((proc->stk), (proc->rdx));
          printf("PUSHING RDX = %d\n", proc->rdx);
          return 1;
        }
  default:
          return -1;
  }
}

//------------------------------------------------------------------------------

int DO_REG_POP (CPU_t* proc, int rx)
{
  assert(proc != NULL);

  if ((proc->stk->stack_size) < 1 )
  {
  printf("ERROR: Not enough elements in stack to POP\n");
  return -1;
  }

  switch(rx){

  case CMD_POP_RAX:
        {
          proc->rax = Stack_Pop (proc->stk, &(proc->rax) );
          printf("POPING RAX = %d\n", proc->rax);
          return 1;
        }

  case CMD_POP_RBX:
        {
          proc->rbx = Stack_Pop((proc->stk), (&(proc->rbx)));
          printf("POPING RBX = %d\n", proc->rbx);
          return 1;
        }

  case CMD_POP_RCX:
        {
          proc->rcx = Stack_Pop((proc->stk), (&(proc->rbx)));
          printf("POPING RCX = %d\n", proc->rcx);
          return 1;
        }

  case CMD_POP_RDX:
        {
          proc->rdx = Stack_Pop((proc->stk), (&(proc->rdx)));
          printf("POPING RDX = %d\n", proc->rdx);
          return 1;
        }
  default:
          return -1;
  }
}

//------------------------------------------------------------------------------

int DO_JUMP (int JUMP_type, int JumpAdress, CPU_t* proc, int IP)
{
  assert(proc != NULL);

  int El_1 = 0, El_2 = 0;

  switch(JUMP_type){

    case CMD_JMP:
        {
          IP = JumpAdress;
          return IP;
        }

    case CMD_JB:
        {
          if (proc->stk->stack_size < 2 ) printf("ERROR: Not enough elements in stack!\n");
          El_1 = Stack_Pop(proc->stk, &(El_1));
          El_2 = Stack_Pop(proc->stk, &(El_2));
          if (El_1 < El_2)
                IP = JumpAdress;

          return IP;
        }

    case CMD_JBE:
        {
          if (proc->stk->stack_size < 2 ) printf("ERROR: Not enough elements in stack!\n");
          El_1 = Stack_Pop(proc->stk, &(El_1));
          El_2 = Stack_Pop(proc->stk, &(El_2));
          if (El_1 <= El_2)
                IP = JumpAdress;

          return IP;
        }

    case CMD_JA:
          {
            if (proc->stk->stack_size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            El_1 = Stack_Pop(proc->stk, &(El_1));
            El_2 = Stack_Pop(proc->stk, &(El_2));
            if (El_1 > El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_JAE:
          {
            if (proc->stk->stack_size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            El_1 = Stack_Pop(proc->stk, &(El_1));
            El_2 = Stack_Pop(proc->stk, &(El_2));
            if (El_1 >= El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_JE:
          {
            if (proc->stk->stack_size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            El_1 = Stack_Pop(proc->stk, &(El_1));
            El_2 = Stack_Pop(proc->stk, &(El_2));
            if (El_1 == El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_JNE:
          {
            if (proc->stk->stack_size < 2 ) printf("ERROR: Not enough elements in stack!\n");
            El_1 = Stack_Pop(proc->stk, &(El_1));
            El_2 = Stack_Pop(proc->stk, &(El_2));
            if (El_1 != El_2)
                  IP = JumpAdress;

            return IP;
          }

    case CMD_CALL:
          {
            IP = JumpAdress;
            //printf("JUMP ADRESS: %d\n", IP);
            return IP;
          }
  }
  return IP;
}

//------------------------------------------------------------------------------

int DO_CALLSTACK_PUSH (CPU_t* proc, int IP)
{
  assert(proc != NULL);
  assert(proc->callstk != NULL);

  Stack_Push(proc->callstk, IP);
  return 1;
}

//------------------------------------------------------------------------------

int DO_MOV(int* RAM, int value, int adress)
{

  assert(RAM);

  RAM[adress] = value;

  return 1;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int DO_DRAW (int cmd);

int VIDEO_MEM (int* RAM)
{

  int cur_length = 0;

  for (int i = 0 ; i < VIDEOMEM_SIZE  ; i ++)
  {
    DO_DRAW( RAM[i + VIDEOMEM_START] );
    cur_length ++;

    if (cur_length == VIDEOMEM_LENGTH)
    {
      std::cout << "\n";
      cur_length = 0;
    }
  }

  return 0;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int DO_DRAW (int cmd)
{
  if ( cmd == 0 )
  {
    std::cout <<  _BOLD_ << " " << _RESET_COLOUR;
    return 1;
  }

  if ( cmd == 1 )
  {
    std::cout <<  _RED_ << "█" << _RESET_COLOUR;
    return 1;
  }

  if ( cmd == 2 )
  {
    std::cout <<  _PINK_ << "█" << _RESET_COLOUR;
    return 1;
  }

  if ( cmd == 3)
  {
    std::cout <<  _BLUE_ << "█" << _RESET_COLOUR;
    return 1;
  }
}
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

#endif  // COMMANDS_H
