
#ifndef CPU_H
#define CPU_H


#include "File_Operations.h"
#include "Standart_Libriaries.h"
#include "Stack.h"

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

const int RAM_SIZE         = 5005;
// В ячейках 5001, 5002, 5003, 5004 лежат данные для построения фигур в видеопамяти
// Окружность: (5001;5002) - центр 5003 - радиус
// Прямая (5001;5002) и (5003;5004) - 2 точки окружности

const int VIDEOMEM_SIZE    = 3000;
const int VIDEOMEM_LENGTH  = 100;
const int VIDEOMEM_START   = 1000;

//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

typedef struct CPU {
  Stack* stk;


  int top;
  long int FileSize;

  int rax;
  int rbx;
  int rcx;
  int rdx;

  Stack* callstk;
} CPU_t;
//-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

// Рисование линии (по координатам)
// Другие рисунки (окружностьБ квадрат)

// Начать на плюсы переписывать

#endif // CPU_H
