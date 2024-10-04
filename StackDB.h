
#ifndef STACK_DB
#define STACK_DB

#include "stack.h"

#ifdef NDEBUG
#define ON_DEBUG(code) 
#else
#define ON_DEBUG(code) code
#endif



const size_t UNLIKELY_STACK_SIZE = 666666666666666666; // Very unlikely to appear, used as a sign of stack underflow.
const int StructCanaryGuardBotREF = 78;
const int StructCanaryGuardTopREF = 78;
const int DataCanaryGuardBotREF = 78;
const int DataCanaryGuardTopREF = 78;

void StackAssert(Stack_t* stk, const char* stkName, const char* file, const char* func, int line);
int doStackDump(Stack_t* stk, const char* stkName, const char* file, const char* func, int line);
int StackError(Stack_t* stk);
int VoidIntDump(Stack_t* stk);

#ifndef STACK_DUMP_INC
#define StackDump(stk) doStackDump((stk), #stk, __FILE__, __func__, __LINE__) 
#endif

#ifndef NDEBUG
#define $ fprintf(stderr, "%s:%d\n", __FILE__, __LINE__) // BIG MONEY
#define STACK_ASSERT(Stack) StackAssert((Stack), #Stack, __FILE__, __func__, __LINE__)
#define STACK_INIT(Stack, inCap) strcat(*Stack.stkName, #Stack); StackInit((Stack), inCap)
#else
#define STACK_ASSERT(Stack)
#endif

#endif



