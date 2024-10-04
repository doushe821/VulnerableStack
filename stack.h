#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

#include "hash.h"
#include "cpybytes.h"

#ifndef NDEBUG
#define ON_DEBUG(code) code
#endif

typedef int StackElem_t;

enum errc
{
    UNDEFINED_ERROR = 2,
    ALLOC_ERROR = 4,
    DESTRUCTOR_ERROR = 8,
    FILE_CREATION_ERROR = 16,
    MEMSET_FAILURE = 32,
    STACK_UNDERFLOW = 64,
    STACK_PTR_IS_NULL = 128,
    DATA_PTR_IS_NULL = 256,
    STACK_OVERFLOW = 512,
    BOT_DATA_CANARY_CORRUPTED = 1024,
    TOP_DATA_CANARY_CORRUPTED = 2048,
    BOT_STACK_CANARY_CORRUPTED = 4096,
    TOP_STACK_CANARY_CORRUPTED = 8192,
    DATA_CORRUPTED = 16384,
    NO_ERRORS = 0,
};

const int STACK_NAME_MAX = 16;
const int FUNC_NAME_MAX= 32;



struct Stack_t
{

    #ifndef NDEBUG
    int StructCanaryGuardBot;

    hash_t HashSum;

    char stkName[STACK_NAME_MAX];
    char file[FILENAME_MAX];
    const char func[FUNC_NAME_MAX];
    const int line;
    #endif

    void* data;
    size_t size;
    size_t capacity;
    size_t elSize;

    u_int16_t Error;

    #ifndef NDEBUG
    int SructCanaryGuardTop;
    #endif
};

const size_t REALLOC_COEF = 2; // USED WHILE REALLOCATING MEMORY, CHANGE IF NEEDED.



int StackPush(Stack_t* stk, void* elem);
int StackPop(Stack_t* stk, void* eslem);

int StackInit(Stack_t* stk, size_t InitCapacity);
int StackDtor(Stack_t* stk);

int StackResize(Stack_t* stk, bool downSizeFlag);
void* wrecalloc(void* ptr, size_t num, size_t size, size_t PrevSize);

void errParse(int e);

#endif