#include "cpybytes.h"
#include <stdio.h> //DEBUG


void cpybytes(void* dest, void* source, size_t size)
{
    uint64_t buf8 = 0;
    uint32_t buf4 = 0;
    uint16_t buf2 = 0;
    uint8_t buf1 = 0;
    
    //fprintf(stderr, "%c, %c \n\n", *(char*)dest, *(char*)source);

    size_t i = 0;
    while(size - i > 0)
    {
        if(size - i >= sizeof(buf8))
        {
            memcpy((char*)dest + i, (char*)source + i, sizeof(buf8));
            i += sizeof(buf8); 
        }
        else if(size - i >= sizeof(buf4))
        {
            memcpy((char*)dest + i, (char*)source+ i, sizeof(buf4));
            i += sizeof(buf4); 
        }
        else if(size - i >= sizeof(buf2))
        {
            memcpy((char*)dest + i, (char*)source+ i, sizeof(buf2));
            i += sizeof(buf2); 
        }
        else if(size - i >= sizeof(buf1))
        {
            
            memcpy((char*)dest + i, (char*)source + i, sizeof(buf1));
            i += sizeof(buf1); 
        }
    }
}