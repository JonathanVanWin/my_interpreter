//Hello I'm a Cat
#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 256

typdef unsigned char uchar;

//memory structure for emulator
typedef struct _Memory{
    char memory[MEM_SIZE];
    uchar index;
} MEM;

//initializing memory
void initiateMemory(MEM* m)
{
    for(uchar i = 0; i < MEM_SIZE-1; i++)
    {
        m->memory[i] = 0;
    }
    m->memory[MEM_SIZE-1] = 0;
    m->index = 0;
}

//get data from cell in memory
char getFromMemory(MEM* m)
{
    return m->memory[m->index];
}

//setting the cell in memory that the memory will act upon
void setMemoryIndex(MEM* m, uchar newIndex)
{
    m->index = newIndex;
}


