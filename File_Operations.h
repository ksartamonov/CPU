#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "commands.h"


long int GetSize(FILE* fp);
size_t CounterOfLines (char* buffer, long  size);
char* ReadFile(const char* name);
//void PutPointers (char* text, int size_text, char** P_Lines, int NumberOfLines);
int StringToNumber (char* string);
void PutPointers (char* buffer, int size, char** strings, int num_str);


//------------------------------------------------------------------------------

long int GetSize(FILE* fp)
{

    int seekResult = fseek(fp, 0L, SEEK_END);
    assert(seekResult == 0);
    long int filesize = ftell(fp);
    rewind(fp);
    return filesize;
}

//------------------------------------------------------------------------------

size_t CounterOfLines (char* buffer, long  size)
{
  assert (buffer);
  assert (size);

  size_t count = 0, flag = 0;

  for (size_t num_count = 0; num_count < size; num_count++)
  {
    if (num_count == size - 1 && (buffer[num_count] == '\n') && flag == 0)
      break;
    if ( (buffer[num_count] == '\n' || buffer[num_count] == '\t' || buffer[num_count] == ' ' ) && flag == 0)
    {
     continue;
    }
    if (buffer[num_count] == '\n')
    {
      count++;
      flag = 0;
      continue;
    }
    flag ++;
  }

  return count;
}

//------------------------------------------------------------------------------

char* ReadFile(const char* name)
{

    assert(name);

    FILE* fp = fopen(name, "r");
    char* buffer = nullptr;

    if (fp==nullptr){
        printf("Error: unable to open file\n");
        exit(1);
    }


    long int filesize = GetSize(fp);
    buffer = (char*)realloc(buffer, filesize + 1);
    int read = fread(buffer + 1, sizeof(char), filesize, fp);
    buffer = buffer + 1;
    fclose(fp);
    return buffer;
}

//------------------------------------------------------------------------------
/*
void PutPointers (char* text, int size_text, char** P_Lines, int NumberOfLines)
{
    int i = 0, IsNewLine = 0, k = 0;

    assert (text != NULL);

    for (i = 0; i < size_text + 1; i++)
    {
        if (!isspace(text[i]) && IsNewLine == 0 && isalpha(text [i]))
        {
	         P_Lines[k] = &text[i];
            k++;
            IsNewLine = 1;
        }

	      if (text[i] == '\n')
	          IsNewLine = 0;
    }
}
*/
void PutPointers (char* buffer, int size, char** strings, int num_str)
{
  assert (strings);
  assert (buffer);
  assert (size);
  assert (num_str);

  size_t buf_count = 0, move_ptr = 0, flag_2 = 0;

  for (size_t num_count = 0; num_count < num_str; num_count++)
  {
    move_ptr = 0;
    for (buf_count; buf_count < size; buf_count++)
    {
      if ( isspace(buffer[buf_count])  && flag_2 == 0)
      {
        buf_count += 1;
        num_count -= 1;
        break;
      }
      if (buffer[buf_count] == '\n')
      {
        buffer[buf_count] = '\0';
        strings[num_count] = &(buffer[buf_count]) - move_ptr;
        buf_count += 1;
        flag_2 = 0;
        break;
      }
      flag_2 ++;
      move_ptr ++;
    }
  }
}

/*
int ReadToIntArray (char* text, int* array, int CharsAmount)
{
  int j = 0, i = 0;
  char val[256] = {0};
  int value = 0;
  while (i < CharsAmount)
  {
    if ( text[i] == 1 )
    {
      if ( text[i+1] == CMD_PUSH - 10)
            {
              array[j] = CMD_PUSH;

              i = i + 2;
              while (!isspace(text[i]))
              {
                val[i - 2] = text[i];
                i++;
              }

              value = atoi(val);
            }

      if ( text[i+1] == CMD_PUSH_RAX - 10)
                array[j] = CMD_PUSH_RAX;

      if ( text[i+1] == CMD_PUSH_RBX - 10)
                array[j] = CMD_PUSH_RBX;

      if ( text[i+1] == CMD_PUSH_RCX - 10)
                array[j] = CMD_PUSH_RCX;

      if ( text[i+1] == CMD_PUSH_RDX - 10)
                array[j] = CMD_PUSH_RDX;

      if ( text[i+1] == CMD_POP_RAX - 10)
                array[j] = CMD_POP_RAX;

      if ( text[i+1] == CMD_POP_RBX - 10)
                array[j] = CMD_POP_RBX;

      if ( text[i+1] == CMD_POP_RCX - 10)
                array[j] = CMD_POP_RCX;

      if ( text[i+1] == CMD_POP_RDX - 10)
                array[j] = CMD_POP_RDX;

      i++;
    }
  }
}
//------------------------------------------------------------------------------
int Pusher(int CMD, )
//------------------------------------------------------------------------------
*/
int GetNumberOfLines(char* buffer){

    int counter = 0;
    for (int i = 0; buffer[i] != '\0'; i++){
        if (buffer[i] == '\n')
            counter++;
    }
    return counter;
}


#endif // FILE_OPERATIONS_H
