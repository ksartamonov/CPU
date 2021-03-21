#include "Standart_Libriaries.h"
#include <iostream>
#include "File_Operations.h"

#define WRONG_COMMAND -69
#define COULD_NOT_DISASSEMBLE -777
#define ASSEMBLED_SUCCESFULLY -420
#define NEED_MORE_ARGUMENTS -13


typedef struct {
  int position;
  char* name;
} label;

// TODO:  структура "Таблица меток"
// typedef struct {
//   label* Labels;
//   int Labels_Amount
// } label_table;

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int Command_Coder     (char* command, int Length, int* Assembled, int PC, label* Marks, int LabelsAmount);
int CommandAssign     (int* Assembled, int CMD, int PC);
int CommandPush       (int* Assembled, int PC, int Length, char* command);
int RegPush           (int* Assembled, int PC, char* command);
int RegPop            (int* Assembled, int PC, char* command);
void AssembledDump    (int* array, FILE* assembled_cmds, int SizeOfArray);
int Labels_Amount     (char** P_Lines, int Lines_Amount);
label* PutMarks       (char* command, int* Assembled, int LabelsAmount, int LabelNum, label* Marks, int pc); //PC -- указатель на следующий пустой элемент кодированного массива
int LabelPosition     (int TypeOfJump, char* cmd, label* Labels, int LabelsAmount);
int Mod_StringCompare (const char* string1, const char* string2, int Comparing_Length);
label* Finding_Labels (char** P_Lines, int Lines_Amount, label* Labels, int Labels_Amount);

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int* Assemble(char** P_Lines, int Lines_Amount, int* Assembled, label* Labels, int NLabels)
{
  assert(P_Lines != NULL);
  assert(Assembled != NULL);
  assert(Labels != NULL);

  int PC = 0, Errors_amount = 0;

  for (int i = 0; i < Lines_Amount; i ++)
  {
    size_t Arr_Size = sizeof(P_Lines[i]);
    int new_PC = Command_Coder( P_Lines[i], Arr_Size, Assembled, PC, Labels, NLabels);

    if (new_PC == WRONG_COMMAND)
      {
        std::cout << "\x1b[31;1merror: \x1b[0m" << "\x1b[1mwrong command | \x1b[0m" << P_Lines[i] << "\x1b[1m | on the \x1b[0m" << i + 1<< "\x1b[1m line!\n\x1b[0m";
        Errors_amount++;
      }

    else
          PC = new_PC;

  }

  if (Errors_amount > 0)
  {
  if (Errors_amount == 1) printf("1 error generated.\n");
  if (Errors_amount >  1) printf("%d errors generated.\n", Errors_amount);
  abort();
  }
  return Assembled;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int main(int argc, char* argv[]) //console cmd: ./main ToAssemble.txt
{
  if ( argc < 2 )
  {
    std::cout << "\x1b[31;1merror: \x1b[0m" << "\x1b[1mNot enough arguments!\n\x1b[0m";
    abort();
  }

  FILE* f = fopen(argv[1], "r");

  long int SizeOfFile = GetSize(f);

  char* buf = ReadFile(argv[1]);

  int Lines_Amount = CounterOfLines(buf, SizeOfFile);

  char** P_Lines = (char**) calloc (Lines_Amount, sizeof (char*));
  assert(P_Lines != NULL);

  PutPointers (buf, SizeOfFile, P_Lines, Lines_Amount);

  int* Assembled = (int*)calloc(2*Lines_Amount, sizeof(int));
  assert(Assembled != NULL);

  int NumLabels = Labels_Amount(P_Lines, Lines_Amount);

  label* Labels = (label*)calloc(NumLabels, sizeof(label));
  assert (Labels != NULL);

  Labels = Finding_Labels(P_Lines, Lines_Amount, Labels, NumLabels);

  Assembled = Assemble(P_Lines, Lines_Amount, Assembled, Labels, NumLabels);


  FILE* ASSEMBLED_CMDS = fopen("assembled_cmds.aks", "w");

  AssembledDump (Assembled, ASSEMBLED_CMDS, Lines_Amount);

  free(P_Lines);
  free(Assembled);
  std::cout << "\x1b[32;1mAssembled successfully!\n\x1b[0m" << "\x1b[36;1mFROM: \x1b[0m"<< argv[1] << "\x1b[36;1m\nINTO:\x1b[0m" << " assembled_cmds.aks\n";
  return 0;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int Command_Coder (char* command, int Length, int* Assembled, int PC, label* Marks, int LabelsAmount) // По строке определяет команду и записывапет в массив
{
  assert(command != NULL);
  assert(Assembled != NULL);
  assert(Marks != NULL);

if ( Mod_StringCompare(command, ":", 1) == 1 )
  {
    //PC++;
    return PC;
  }

if ( Mod_StringCompare(command, "PUSH R", 6) == 1 )
  {
    PC = RegPush (Assembled, PC, command);
    return PC;
  }

if ( Mod_StringCompare(command, "POP R", 5) == 1 )
  {
    PC = RegPop (Assembled, PC, command);
    return PC;
  }


if ( Mod_StringCompare(command, "PUSH ", 5) == 1 )
  { //if (CheckPush(command) < 0) return NEED_MORE_ARGUMENTS;
    PC = CommandPush (Assembled, PC, Length, command);
    if (!isdigit(command[5])) return WRONG_COMMAND; //TODO: Fix the checking of wrong argument
    return PC;
  }

if ( Mod_StringCompare(command, "ADD", 3) == 1 )
  {   PC = CommandAssign(Assembled, CMD_ADD, PC); return PC;   }

if ( Mod_StringCompare(command, "SUB", 3) == 1 )
  {  PC = CommandAssign(Assembled, CMD_SUB, PC);  return PC;   }

if ( Mod_StringCompare(command, "DIV", 3) == 1 )
  {  PC = CommandAssign(Assembled, CMD_DIV, PC);   return PC;  }

if ( Mod_StringCompare(command, "MUL", 3) == 1 )
  {  PC = CommandAssign(Assembled, CMD_MUL, PC);   return PC;  }

if ( Mod_StringCompare(command, "FSQRT", 5) == 1 )
  {  PC = CommandAssign(Assembled, CMD_FSQRT, PC); return PC;  }

if ( Mod_StringCompare(command, "OUT", 3) == 1 )
  {  PC = CommandAssign(Assembled, CMD_OUT, PC);   return PC;  }

if ( Mod_StringCompare(command, "IN", 2) == 1 )
  {  PC = CommandAssign(Assembled, CMD_IN, PC);    return PC;  }

if ( Mod_StringCompare(command, "FSQRT", 5) == 1 )
  {  PC = CommandAssign(Assembled, CMD_FSQRT, PC); return PC;  }

if ( Mod_StringCompare(command, "OUT", 3) == 1 )
  {  PC = CommandAssign(Assembled, CMD_OUT, PC);   return PC;  }

if ( Mod_StringCompare(command, "HLT", 3) == 1 )
  { PC = CommandAssign(Assembled, CMD_HLT, PC);    return PC;  }

if ( Mod_StringCompare(command, "JMP ", 4) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_JMP, PC);
    int adress = LabelPosition(CMD_JMP, command, Marks, LabelsAmount);
    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "JB ", 3) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_JB, PC);
    int adress = LabelPosition(CMD_JB, command, Marks, LabelsAmount);
    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "JBE ", 4) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_JBE, PC);
    int adress = LabelPosition(CMD_JBE, command, Marks, LabelsAmount);

    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "JA ", 3) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_JA, PC);
    int adress = LabelPosition(CMD_JA, command, Marks, LabelsAmount);

    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "JAE ", 4) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_JAE, PC);
    int adress = LabelPosition(CMD_JAE, command, Marks, LabelsAmount);

    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "JE ", 3) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_JE, PC);
    int adress = LabelPosition(CMD_JE, command, Marks, LabelsAmount);

    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "JNE ", 4) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_JNE, PC);
    int adress = LabelPosition(CMD_JNE, command, Marks, LabelsAmount);

    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "CALL :", 5) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_CALL, PC);
    int adress = LabelPosition(CMD_CALL, command, Marks, LabelsAmount);

    *(Assembled+PC) = Marks[ adress ].position;
    PC ++;
    return PC;
  }

if ( Mod_StringCompare(command, "RET", 3) == 1 )
  {
    PC = CommandAssign(Assembled, CMD_RET, PC);
    return PC;
  }


return WRONG_COMMAND;

}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int Mod_StringCompare (const char* string1,const char* string2, int Comparing_Length) // Сравнивает строки до определенной длины (аналог strcnmp)
{
  if (Comparing_Length > strlen(string1) || Comparing_Length > strlen(string2))
  {
    return -1;
  }

  for ( int i = 0; i < Comparing_Length ; i ++)
  {
    if ( (string1[i]) != (string2[i]) )
        return  0;
  }
  return 1;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int CommandAssign (int* Assembled, int CMD, int PC)
{
  *(Assembled + PC) = CMD;
  PC++;
  return PC;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int CommandPush (int* Assembled, int PC, int Length, char* command) // Для записи команды PUSH
{
  int x = 0;
  char* val = (char*)calloc(Length - 5, sizeof(char));
  for (int p = 0; p < Length - 5; p++)
  {
    val[p] = command[p+5];
  }
  int a  = atoi (val);
  //printf("PUSHING VALUE: %d\n", a);
  *(Assembled + PC) = CMD_PUSH;
  *(Assembled + PC+1) = a;

  PC+=2;

  free (val);
  return PC;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int RegPush (int* Assembled, int PC, char* command) // Для записи PUSH регистров
{
  assert (Assembled != NULL);
  assert (command != NULL);


  if ( Mod_StringCompare(command, "PUSH RAX", 8) == 1 )
    {  *(Assembled + PC) = CMD_PUSH_RAX;  PC++; return PC; }
  if ( Mod_StringCompare(command, "PUSH RBX", 8) == 1 )
    {  *(Assembled + PC) = CMD_PUSH_RBX;  PC++; return PC; }
  if ( Mod_StringCompare(command, "PUSH RCX", 8) == 1 )
    {  *(Assembled + PC) = CMD_PUSH_RCX;   PC++; return PC; }
  if ( Mod_StringCompare(command, "PUSH RDX", 8) == 1 )
    {  *(Assembled + PC) = CMD_PUSH_RDX;   PC++; return PC; }

  return WRONG_COMMAND;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int RegPop (int* Assembled, int PC, char* command) // Для записи POP регистров
{
  assert(Assembled != NULL);
  assert(command != NULL);

  if ( Mod_StringCompare(command, "POP RAX", 7) == 1 )
    {  *(Assembled + PC) = CMD_POP_RAX; PC++; return PC; }
  if ( Mod_StringCompare(command, "POP RBX", 7) == 1 )
    {  *(Assembled + PC) = CMD_POP_RBX; PC++; return PC; }
  if ( Mod_StringCompare(command, "POP RCX", 7) == 1 )
    {  *(Assembled + PC) = CMD_POP_RCX; PC++; return PC; }
  if ( Mod_StringCompare(command, "POP RDX", 7) == 1 )
    {  *(Assembled + PC) = CMD_POP_RDX; PC++; return PC; }

  return WRONG_COMMAND;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

void AssembledDump (int* array, FILE* assembled_cmds, int SizeOfArray) // Печать ассемблерного кода в файл
{
  assert (array != NULL);

    for (int i = 0 ; i < 2*SizeOfArray; i ++)
    {
      if ( (array[i] != 0) || (array[i] == 0 && array[i+1] != 0 && array[i-1] != 0) )
      fprintf( assembled_cmds, "%d\n", array[i]);
    }

}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int LabelPosition(int TypeOfJump, char* cmd, label* Labels, int LabelsAmount)
{
  int length = sizeof(cmd);

  char* mark = (char*)calloc(length, sizeof(char));

  if ( TypeOfJump == CMD_JMP || TypeOfJump == CMD_JNE || TypeOfJump == CMD_JAE || TypeOfJump == CMD_JBE )
  {
    for (int i = 0; i < length; i ++)
    {
      mark[i] = cmd[i+4];
    }
  }

  if ( TypeOfJump == CMD_JE || TypeOfJump == CMD_JA || TypeOfJump == CMD_JB )
  {
    for (int i = 0; i < length; i ++)
    {
      mark[i] = cmd[i+3];
    }
  }

  if (TypeOfJump == CMD_CALL)
  {
    for (int i = 0; i < length; i ++)
    {
      mark[i] = cmd[i+5];
    }
  }

  for (int pos = 0; pos < LabelsAmount; pos ++)
  {
    if ( Mod_StringCompare(Labels[pos].name, mark, sizeof(cmd) - 4) == 1)
    {
      free(mark);
      return  pos;
    }
  }

  free(mark);
  printf("ERROR: Could not find a label of a cmd %s\n", cmd);

  return -1;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

label* Finding_Labels (char** P_Lines, int Lines_Amount, label* Labels, int Labels_Amount)
{
  int NumberOfLine = 0, Label_Num = 0, cmd_idx = 0;
  while (NumberOfLine != Lines_Amount)
  {
    if (Mod_StringCompare(P_Lines[NumberOfLine], "PUSH R", 6) == 1 ) //Если  PUSH регистра
    {
      cmd_idx++;
      NumberOfLine++;
      continue;
    }

    if ( Mod_StringCompare(P_Lines[NumberOfLine], ":", 1) == 1 )
      {
        Labels[Label_Num].position = cmd_idx;
        printf("label[%d] = %d NAME: %s\n", Label_Num, cmd_idx, P_Lines[NumberOfLine]);
        Labels[Label_Num].name     = P_Lines[NumberOfLine];
        Label_Num++;
      }

    else //Если команды двухаргументные
      {
        if ( Mod_StringCompare(P_Lines[NumberOfLine], "PUSH", 4) == 1 || Mod_StringCompare(P_Lines[NumberOfLine], "J", 1) == 1 || Mod_StringCompare(P_Lines[NumberOfLine], "CALL", 4) == 1 )
        { cmd_idx++; }

        cmd_idx++;
      }

    NumberOfLine++;
  }
  return Labels;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

int Labels_Amount ( char** P_Lines, int Lines_Amount) // Cчитает количество  меток
{
  int Labels_Amount = 0;

  for (int i = 0; i < Lines_Amount; i++)
  {
    if ( Mod_StringCompare(P_Lines[i], ":", 1) == 1 )
          Labels_Amount++;
  }

  return Labels_Amount;
}

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
