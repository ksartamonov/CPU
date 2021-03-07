#include "Standart_Libriaries.h"

#include "File_Operations.h"
//#include "commands.h"

#define WRONG_COMMAND -69

#define ASSEMBLED_SUCCESFULLY -420

typedef struct {
  int position;
  char* Label_Name;
} label;


int Command_Coder (char* command, int Length, int* Assembled, int PC, label* Marks, int LabelsAmount);
int CommandAssign (int* Assembled, int CMD, int PC);
int CommandPush (int* Assembled, int PC, int Length, char* command);
int RegPush (int* Assembled, int PC, char* command);
int RegPop (int* Assembled, int PC, char* command);
void AssembledDump (int* array, FILE* assembled_cmds, int SizeOfArray);
int IsMark (char* commandl, int LabelsAmount);
label* PutMarks (char* command, int* Assembled, int LabelsAmount, int LabelNum, label* Marks, int pc); //PC -- указатель на следующий пустой элемент кодированного массива
int LabelPosition (int TypeOfJump, char* cmd, label* Labels, int LabelsAmount);
int Mod_StringCompare (char* string1, char* string2, int Comparing_Length);
label* Labels_Finding(char** P_Lines, int Lines_Amount, int* Assembled, label* Labels, int NLabels);
int Labels_Amount(char** P_Lines, int Lines_Amount);


int* Assemble(char** P_Lines, int Lines_Amount, int* Assembled, label* Labels, int NLabels)
{
  int PC = 0;

  for (int i = 0; i < Lines_Amount; i ++)
  {
    size_t Arr_Size = sizeof(P_Lines[i]);
    int new_PC = Command_Coder( P_Lines[i], Arr_Size, Assembled, PC, Labels, NLabels);
    if (new_PC == WRONG_COMMAND)
          printf("Wrong command on the %d position: %d\n", PC, P_Lines[i]);
    else{
    //else PC = Command_Coder( P_Lines[i], Arr_Size, Assembled, PC, Labels, NLabels);
      PC = new_PC;
      //printf("%d\n", PC);  // pc = PC + 1;
    }
  }
  return Assembled;
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[]) //console cmd: ./main ToAssemble.txt
{
  //printf("argv[1] = %s\n", argv[1]);
  //return 1;
  if ( argc < 2 ) printf("ERROR: Not enough arguments!\n");

  FILE* f = fopen(argv[1], "r");

  long int SizeOfFile = GetSize(f);

  char* buf = ReadFile(argv[1]);

  int Lines_Amount = CounterOfLines(buf, SizeOfFile);

   printf("[debug]LINES AMOUNT: %d\n", Lines_Amount);

  char** P_Lines = (char**) calloc (Lines_Amount, sizeof (char*));

  assert(P_Lines != NULL);

  //printf("[debug]P_LINES size = %d\n", sizeof(P_Lines));


  PutPointers (buf, SizeOfFile, P_Lines, Lines_Amount);

  int* Assembled = (int*)calloc(2*Lines_Amount, sizeof(int));

  assert(Assembled != NULL);



  int NumLabels = Labels_Amount(P_Lines, Lines_Amount);

  //printf("[       debug        ] NumLabels = %d\n", NumLabels);

  //if ( NumLabels > 0 )
  //{
  label* Labels = (label*)calloc(NumLabels, sizeof(label));
  assert (Labels != NULL);

  // for (int i = 0; i < NumLabels ; i ++)
  // {
  //   printf("[debug] Label #%d\n position: %d\nname: %s\n", i, Labels[i].position, Labels[i].Label_Name);
  // }

  Labels = Labels_Finding(P_Lines, Lines_Amount, Assembled, Labels, NumLabels);

  // for (int i = 0; i < NumLabels ; i ++)
  // {
  //   printf("[debug] Label #%d\n position: %d\nname: %s\n", i, Labels[i].position, Labels[i].Label_Name);
  // }

  assert(Assembled != NULL);

  Assembled = Assemble(P_Lines, Lines_Amount, Assembled, Labels, NumLabels);

  printf("[debug] Assembled:\n");
  for (int i = 0; i < 2*Lines_Amount; i ++)
      printf("%d \n", Assembled[i]);

  FILE* ASSEMBLED_CMDS = fopen("assembled_cmds.txt", "w");

  int Arr_Size = sizeof(Assembled);

  //printf("[debug] ARRAY_SIZE: %d\n", Arr_Size);

  AssembledDump (Assembled, ASSEMBLED_CMDS, Arr_Size);

  free(P_Lines);
  free(Assembled);

  printf("Assembled successfully\n FROM: %s \n INTO: assembled_cmds.txt\n", argv[1]);
  return 0;
}

//------------------------------------------------------------------------------

int Command_Coder (char* command, int Length, int* Assembled, int PC, label* Marks, int LabelsAmount) //PC -- указатель на следующий пустой элемент кодированного массива
{
if ( Mod_StringCompare(command, "PUSH r", 6) == 1 )
  {
    PC = RegPush (Assembled, PC, command);
    return PC;
  }

if ( Mod_StringCompare(command, "POP r", 5) == 1 )
  {
    PC = RegPop (Assembled, PC, command);
    return PC;
  }

if ( Mod_StringCompare(command, ":", 1) == 1 )
  {
    //PC++;
    return PC;
  }

if ( Mod_StringCompare(command, "PUSH ", 5) == 1 )
  {   PC = CommandPush (Assembled, PC, Length, command); return PC; }

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
    //printf("ADRESS: %d\n", adress);
    //printf("POS: %d\n", Marks[ adress ].position);
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

return WRONG_COMMAND;

}

/*if ( Mod_StringCompare(command, "IN", 2) == 1 )
{
*(Assembled + PC) = CMD_IN;
PC++;
printf("Enter the in value:\n");
int x = scanf("%d", &x);
*(Assembled + PC + 1) = x;
PC++;
return PC;
}
*/

//------------------------------------------------------------------------------

int Mod_StringCompare (char* string1, char* string2, int Comparing_Length)
{

  //printf("Comparing %s and %s up to %d:\n", string1, string2, Comparing_Length);
  if (Comparing_Length > strlen(string1) || Comparing_Length > strlen(string2))
  {
    //printf("Troubles with length\n");
    return -1;
  }

  for ( int i = 0; i < Comparing_Length ; i ++)
  {
    if ( tolower(string1[i]) != tolower(string2[i]) )
    {
      //printf("%s != %s up to %d\n", string1, string2, Comparing_Length);
      return  0;
    }
  }
  //printf("%s == %s up to %d !!!!!!!!!!!!!!!!!!\n", string1, string2, Comparing_Length);
  return 1;
}

//------------------------------------------------------------------------------

int CommandAssign (int* Assembled, int CMD, int PC)
{
  *(Assembled + PC) = CMD;
  PC++;
  return PC;
}

//------------------------------------------------------------------------------

int CommandPush (int* Assembled, int PC, int Length, char* command)
{
  int x = 0;
  char* val = (char*)calloc(Length - 5, sizeof(char));
  for (int p = 0; p < Length - 5; p++)
  {
    val[p] = command[p+5];
  }
  int a  = atoi (val);

  //printf("[debug] Pushing_val = %d\n", a);

  *(Assembled + PC) = CMD_PUSH;
  *(Assembled + PC+1) = a;

PC+=2;

free (val);
return PC;
}

//------------------------------------------------------------------------------

int RegPush (int* Assembled, int PC, char* command)
{
  if ( Mod_StringCompare(command, "PUSH rax", 8) == 1 )
      *(Assembled + PC) = CMD_PUSH_RAX;
  if ( Mod_StringCompare(command, "PUSH rbx", 8) == 1 )
      *(Assembled + PC) = CMD_PUSH_RBX;
  if ( Mod_StringCompare(command, "PUSH rcx", 8) == 1 )
      *(Assembled + PC) = CMD_PUSH_RCX;
  if ( Mod_StringCompare(command, "PUSH rdx", 8) == 1 )
      *(Assembled + PC) = CMD_PUSH_RDX;

  PC++;
  return PC;
}

//------------------------------------------------------------------------------

int RegPop (int* Assembled, int PC, char* command)
{
  if ( Mod_StringCompare(command, "POP rax", 7) == 1 )
      *(Assembled + PC) = CMD_POP_RAX;
  if ( Mod_StringCompare(command, "POP rbx", 7) == 1 )
      *(Assembled + PC) = CMD_POP_RBX;
  if ( Mod_StringCompare(command, "POP rcx", 7) == 1 )
      *(Assembled + PC) = CMD_POP_RCX;
  if ( Mod_StringCompare(command, "POP rdx", 7) == 1 )
      *(Assembled + PC) = CMD_POP_RDX;

  PC++;
  return PC;
}

//------------------------------------------------------------------------------

void AssembledDump (int* array, FILE* assembled_cmds, int SizeOfArray)
{
  //printf("ASS %d: ", sizeof(array));
    for (int i = 0 ; i < 2*SizeOfArray; i ++)
    {
    if ( array[i] != 0)
    fprintf( assembled_cmds, "%d\n", array[i]);
    }
}

//------------------------------------------------------------------------------

label* PutMarks (char* command, int* Assembled, int LabelsAmount, int LabelNum, label* Marks, int pc) //PC -- указатель на следующий пустой элемент кодированного массива
{
  int length = sizeof(command);
  //int flag = 1
  if ( Mod_StringCompare(command, ":", 1) == 1 )
    {
      for ( int i = 0; i < length; i ++)
          Marks[LabelNum].Label_Name = command;

      //printf("fucking slave = %p\n", Marks[LabelNum]);
      Marks[LabelNum].position      = pc;

    }
  // else
  //     printf("blyat\n");
  return Marks;
}

//------------------------------------------------------------------------------

int IsMark (char* command, int LabelsAmount)
{
  int length = sizeof(command);
  if ( Mod_StringCompare(command, ":", 1) == 1 )
  LabelsAmount++;
  return LabelsAmount;
}

//------------------------------------------------------------------------------

int LabelPosition(int TypeOfJump, char* cmd, label* Labels, int LabelsAmount)
{
  int length = sizeof(cmd);
  // printf("COMMAND: %s\n", cmd);
  char* mark = (char*)calloc(length - 4, sizeof(char));
if ( TypeOfJump == CMD_JMP || TypeOfJump == CMD_JNE || TypeOfJump == CMD_JAE || TypeOfJump == CMD_JBE )
{
  for (int i = 0; i < length; i ++)
  {
    mark[i] = cmd[i+4];     //JMP :ASS
  }                           //012345
}
if ( TypeOfJump == CMD_JE || TypeOfJump == CMD_JA || TypeOfJump == CMD_JB )
{
  for (int i = 0; i < length; i ++)
  {
    mark[i] = cmd[i+3];     //JMP :ASS
  }
}
  // printf("Label Name  %s\n", mark);
  for (int pos = 0; pos < LabelsAmount; pos ++)
  {
    if ( Mod_StringCompare(Labels[pos].Label_Name, mark, sizeof(cmd) - 4) == 1)
    {
      return  pos;
    }
  }

  free(mark);
  printf("ERROR: Could not find a label\n");
  return -1;
}

//------------------------------------------------------------------------------

label* Labels_Finding(char** P_Lines, int Lines_Amount, int* Assembled, label* Labels, int NLabels)
{

  int LabelNum = 0;

  for (int l = 0; l < Lines_Amount; l ++)
  {
    Labels = PutMarks(P_Lines[l], Assembled, NLabels, LabelNum, Labels, l);
    if (Labels[LabelNum].position > 0) LabelNum++;
  }
  return Labels;
}

//------------------------------------------------------------------------------

int Labels_Amount(char** P_Lines, int Lines_Amount)
{
  int NLabels = 0;
  for (int l = 0; l < Lines_Amount; l ++)
  {
    NLabels = IsMark (P_Lines[l], NLabels);
  }
  return NLabels;
}
