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

    assert(name != NULL);

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
    for (buf_count = 0; buf_count < size; buf_count++)
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

//------------------------------------------------------------------------------

#endif // FILE_OPERATIONS_H
