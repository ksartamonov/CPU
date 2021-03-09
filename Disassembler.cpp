
#include "Standart_Libriaries.h"
#include "commands.h"
#include "File_Operations.h"

#define WRONG_COMMAND -69
#define DISASSEMBLED_SUCCESFULLY -2002

int Command_Decoder(int cmd1, int cmd2, FILE* disassembled_cmds, int PC, int LabelsAmount, int* Labels, int cmd_nmbr);
int IsLabel (int PC, FILE* disassembled_cmds, int LabelsAmount, int* Labels, int cmd_nmbr);

//------------------------------------------------------------------------------

int  DisAssemble(int* Commands, FILE* disassembled_cmds, int NumberOfcommands)
{
  int PC = 0, label_idx = 0;

  int* Labels = (int*)calloc( NumberOfcommands, sizeof(int) );

  assert(Labels != NULL);
  assert(disassembled_cmds != NULL);
  //printf("Commands amount = %d\n", NumberOfcommands);

  for (int com_num = 0; com_num < NumberOfcommands; com_num++)
  {
    if ( Commands[com_num] == CMD_JMP || Commands[com_num] == CMD_JB || Commands[com_num] == CMD_JBE || Commands[com_num] == CMD_JA || Commands[com_num] == CMD_JAE || Commands[com_num] == CMD_JE || Commands[com_num] == CMD_JNE || Commands[com_num] == CMD_CALL)
    {
      Labels[label_idx] = Commands[com_num + 1];
      label_idx++;
    }
  }

  for (int i = 0 ; i < NumberOfcommands; i ++)
  {
    printf("Labels[%d] = %d\n", i , Labels[i]);
  }

  //return 0;
  // int Commands_Counter = 0; // TODO: счетчик идет и считает команды, так удобно вставить ссылку
  int LabelsNum = label_idx; //  Присваиваем последнему значению счетчика

  int com_num = 0;
  while (PC < NumberOfcommands)
  {
    // printf("PC = %d\n", PC);

    int new_PC = Command_Decoder(Commands[PC], Commands[PC+1], disassembled_cmds, PC, LabelsNum, Labels, com_num);

    //printf("NEW PC = %d\n", new_PC);
    com_num++;

    if (new_PC == WRONG_COMMAND)
          {
          printf("Wrong command on the %d position: %d\n", PC, Commands[PC]);
          PC++;
          // printf("PC = %d\n", PC);
        }
    else
    {
      PC = new_PC;
    }
  }
  return DISASSEMBLED_SUCCESFULLY;
}


//------------------------------------------------------------------------------

int main(int argc, char* argv [])
{
  if ( argc < 2 ) { printf("ERROR: Not enough arguments!\n"); return -1; }

  FILE* f = fopen(argv[1], "r");

  long int SizeOfFile = GetSize(f);

  char* buf = ReadFile(argv[1]);

  int Lines_Amount = CounterOfLines(buf, SizeOfFile);

  //printf("[debug]LINES AMOUNT: %d\n", Lines_Amount);

  char** P_Lines = (char**) calloc (Lines_Amount, sizeof (char*));

  assert(P_Lines != NULL);

  int* cmds = (int*)calloc(Lines_Amount, sizeof(int));

  PutPointers (buf, SizeOfFile, P_Lines, Lines_Amount);

  for (int idx = 0; idx < Lines_Amount; idx++)
  {
    cmds[idx] = atoi(P_Lines[idx]);
  }

  // for (int i = 0 ; i < Lines_Amount; i ++)
  // {
  //   printf("cmds[%d] = %d\n", i , cmds[i]);
  // }

  FILE* DISASSEMBLED_CMDS = fopen("disassembled_cmds.txt", "w");

  if ( DisAssemble(cmds, DISASSEMBLED_CMDS, Lines_Amount) == DISASSEMBLED_SUCCESFULLY)
        printf("Disassembled well!\n");

  return 0;
}

//------------------------------------------------------------------------------

int Command_Decoder(int cmd1, int cmd2, FILE* disassembled_cmds, int PC, int LabelsAmount, int* Labels, int cmd_nmbr)
{
int cur_PC = PC;
// printf("PC: %d\n", PC);
int lab_num = IsLabel(PC, disassembled_cmds, LabelsAmount, Labels, cmd_nmbr);
printf("LABNUM: %d\n", lab_num);
// if (lab_num >= 0) { fprintf(disassembled_cmds, ":%d\n", Labels[lab_num]); PC ++; return PC; }
// if (lab_num >= 0)
// {
// PC++;
// }

//printf("AFTER IsLabel PC: %d\n", PC);


  if ( cmd1 == CMD_PUSH )
  {
    // printf("DOING PUSH!\n");
    // printf("PC: %d\n", PC+1);
    fprintf(disassembled_cmds, "PUSH %d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_ADD )
  {
    fprintf(disassembled_cmds, "ADD\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_SUB )
  {
    fprintf(disassembled_cmds, "SUB\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_DIV )
  {
    fprintf(disassembled_cmds, "DIV\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_IN )
  {
    fprintf(disassembled_cmds, "IN\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_MUL )
  {
    fprintf(disassembled_cmds, "MUL\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_FSQRT )
  {
    fprintf(disassembled_cmds, "FSQRT\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_OUT )
  {
    fprintf(disassembled_cmds, "OUT\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_HLT)
  {
    fprintf(disassembled_cmds, "HLT\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_PUSH_RAX )
  {
    fprintf(disassembled_cmds, "PUSH RAX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_PUSH_RBX )
  {
    fprintf(disassembled_cmds, "PUSH RBX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_PUSH_RCX )
  {
    fprintf(disassembled_cmds, "PUSH RCX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_PUSH_RDX )
  {
    fprintf(disassembled_cmds, "PUSH RDX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_POP_RAX )
  {
    fprintf(disassembled_cmds, "POP RAX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_POP_RBX )
  {
    fprintf(disassembled_cmds, "POP RBX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_POP_RCX )
  {
    fprintf(disassembled_cmds, "POP RCX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_POP_RDX )
  {
    fprintf(disassembled_cmds, "POP RDX\n"); PC++; return PC;
  }

  if ( cmd1 == CMD_JMP )
  {
    fprintf(disassembled_cmds, "JMP :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_JB )
  {
    fprintf(disassembled_cmds, "JB :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_JBE )
  {
    fprintf(disassembled_cmds, "JBE :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_JA )
  {
    fprintf(disassembled_cmds, "JA :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_JAE )
  {
    fprintf(disassembled_cmds, "JAE :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_JE )
  {
    fprintf(disassembled_cmds, "JE :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_JNE )
  {
    fprintf(disassembled_cmds, "JNE :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_CALL)
  {
    fprintf(disassembled_cmds, "CALL :%d\n", cmd2); PC += 2; return PC;
  }

  if ( cmd1 == CMD_RET)
  {
    fprintf(disassembled_cmds, "RET\n", cmd2); PC ++; return PC;
  }

  return WRONG_COMMAND;
}

//------------------------------------------------------------------------------

int IsLabel (int PC, FILE* disassembled_cmds, int LabelsAmount, int* Labels, int cmd_nmbr)
{
  // if ( cmd == CMD_PUSH || cmd == CMD_JMP || cmd == CMD_JB || cmd == CMD_JBE || cmd == CMD_JA || cmd == CMD_JAE || cmd == CMD_JE || cmd == CMD_JNE )
  // PC++;
  printf("НА ВХОД ПОЛУЧИЛ PC = %d\n", PC);
  for (int i = 0; i < LabelsAmount; i ++)
  {
    if (Labels[i] == cmd_nmbr)
    {
      printf("МЕТКА НАШЛАСЬ\n");
      fprintf(disassembled_cmds, ":%d\n", Labels[i]);
      return i;
    }
  }
  return -1;
}
