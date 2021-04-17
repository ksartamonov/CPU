#include <iostream>
#include "Standart_Libriaries.h"
#include "commands.h"
#include "File_Operations.h"




//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int Command_Decoder(int cmd1, int cmd2, int cmd3, FILE* disassembled_cmds, int PC, int LabelsAmount, int* Labels);
int PrintLabel (int PC, FILE* disassembled_cmds, int LabelsAmount, int* Labels);
int  DisAssemble(int* Commands, FILE* disassembled_cmds, int NumberOfcommands, int* Labels, int Labels_Amount);
int ThisLabelExists(int* marks, int size, int pos);

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int  DisAssemble(int* Commands, FILE* disassembled_cmds, int NumberOfCommands, int* Labels, int Labels_Amount)
{
  int PC = 0;

  assert(Labels != NULL);
  assert(disassembled_cmds != NULL);

  while ( PC != NumberOfCommands)
  {
    PC = Command_Decoder(Commands[PC], Commands[PC+1], Commands[PC+2], disassembled_cmds, PC, Labels_Amount, Labels);
  }

  return DISASSEMBLED_SUCCESFULLY;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int main(int argc, char* argv [])
{
  if ( argc < 2 )
  {
    std::cout << _RED_ << "error: \x1b[0m" << _BOLD_ << "Not enough arguments!\n\x1b[0m";
    abort();
  }

  if (strcmp(argv[1],"assembled_cmds.aks") != 0)
  {
    std::cout << _RED_ << "error: \x1b[0m" << _BOLD_ << "Need the file \x1b[0m" << _YELLOW_ << "assembled_cmds.aks\x1b[0m" << _BOLD_ << " to disassemble!\n\x1b[0m";
    return WRONG_FILE;
  }
  FILE* f = fopen(argv[1], "r");

  long int SizeOfFile = GetSize(f);

  char* buf = ReadFile(argv[1]);

  int Lines_Amount = CounterOfLines(buf, SizeOfFile);
  //printf("Lines amount= %d\n", Lines_Amount);

  char** P_Lines = (char**) calloc (Lines_Amount, sizeof (char*));

  assert(P_Lines != NULL);

  int* cmds = (int*)calloc(Lines_Amount, sizeof(int));

  PutPointers (buf, SizeOfFile, P_Lines, Lines_Amount);

  for (int idx = 0; idx < Lines_Amount; idx++)
  {
    cmds[idx] = atoi(P_Lines[idx]);
  }

  int label_idx = 0;

  int* Labels = (int*) calloc( Lines_Amount, sizeof(int) );


  for (int l = 0; l < Lines_Amount; l++)
  {
    if ( (cmds[l] == CMD_JMP || cmds[l] == CMD_JB || cmds[l] == CMD_JBE || cmds[l] == CMD_JA || cmds[l] == CMD_JAE || cmds[l] == CMD_JE || cmds[l] == CMD_JNE || cmds[l] == CMD_CALL ) && ( !ThisLabelExists(Labels, Lines_Amount, cmds[l+1])) )
    {
      Labels[label_idx] = cmds[l + 1];
      label_idx++;
    }
  }

  int labels_amount = label_idx; // Присваиваем количество последнему значению счетчика
  //printf("LABELS amount = %d\n", labels_amount);
  FILE* DISASSEMBLED_CMDS = fopen("disassembled_cmds.aks", "w");

  if ( DisAssemble(cmds, DISASSEMBLED_CMDS, Lines_Amount , Labels, labels_amount) == DISASSEMBLED_SUCCESFULLY)
        std::cout << _GREEN_ <<"Disassembled successfully!\n" << _RESET_COLOUR << _LIGHT_BLUE_ << "FROM: "<< _RESET_COLOUR << argv[1] << _LIGHT_BLUE_ << "\nINTO:" << _RESET_COLOUR <<" disassembled_cmds.aks\n";

  fclose(DISASSEMBLED_CMDS);
  return 0;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int Command_Decoder(int cmd1, int cmd2, int cmd3, FILE* disassembled_cmds, int PC, int LabelsAmount, int* Labels) // По элементу массива int печатает дизассемблированный код
{

  PrintLabel(PC, disassembled_cmds, LabelsAmount, Labels); //  Если на этом положении PC есть метка, то пишем ее, а потом саму команду

  if ( cmd1 == CMD_PUSH )
  {
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
    fprintf(disassembled_cmds, "RET\n"); PC ++; return PC;
  }

  if ( cmd1 == CMD_MOV)
  {
    fprintf(disassembled_cmds, "MOV %d, [%d]\n", cmd2, cmd3); PC += 3; return PC;
  }

  if ( cmd1 == CMD_VISUAL)
  {
    fprintf(disassembled_cmds, "VISUALIZE\n"); PC ++; return PC;
  }

  if ( cmd1 == CMD_CIRCLE)
  {
    fprintf(disassembled_cmds, "CIRCLE\n"); PC ++; return PC;
  }

  if ( cmd1 == CMD_LINE)
  {
    fprintf(disassembled_cmds, "LINE\n"); PC ++; return PC;
  }

  return WRONG_COMMAND;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int PrintLabel (int PC, FILE* disassembled_cmds, int LabelsAmount, int* Labels)
{
  for (int i = 0; i < LabelsAmount; i ++)
  {
    if (Labels[i] == PC)
    {
      fprintf(disassembled_cmds, ":%d\n", Labels[i]);
      return i;
    }
  }
  return -1;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int ThisLabelExists(int* marks, int size, int pos)
{
  for (int i = 0 ; i < size; i ++)
  {
    if ( marks[i] == pos)
    return 1;
  }
  return 0;
}
