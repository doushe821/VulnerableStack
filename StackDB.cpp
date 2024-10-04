#include "StackDB.h"

static unsigned int POISON = 2122219134;
static unsigned int* POISON_PTR = &POISON; 

void StackAssert(Stack_t* stk, const char* stkName, const char* file, const char* func, int line)
{
    
    //fprintf(stderr, "rofl ASSERT START\n");
    if(stk == NULL)
    {
        
        fprintf(stderr, "Stack pointer is NULL");
        assert(0);
    }

    if((stk->Error = StackError(stk)) != 0)
    {
        
        //fprintf(stderr, "rofl\n");
        //fprintf(stderr, "%hu\n", stk->Error);
        
        VoidIntDump(stk);
        
        
        assert(0 && "Emergency exit, stack dumped into log.txt");
    }
    //fprintf(stderr, "rofl ASSERT END\n");
}

int StackError(Stack_t* stk)
{
    size_t errFlag = 0;
    if(stk == NULL)
    {
        fprintf(stderr, "Stack pointer is NULL\n");
        errFlag += STACK_PTR_IS_NULL;
    }

    if(stk->data == NULL)
    {
        fprintf(stderr, "Data pointer is NULL\n");
        errFlag += DATA_PTR_IS_NULL;
    }

    if(stk->size > stk->capacity)
    {
        fprintf(stderr, "Stack overflow\n");
        errFlag += STACK_OVERFLOW;
    }

    if(stk->size > UNLIKELY_STACK_SIZE)
    {
        fprintf(stderr, "Stack has negative size (stack underflow)\n");
        errFlag += STACK_UNDERFLOW;
    }

    if(*(int*)((char*)stk->data) != DataCanaryGuardBotREF)
    {
        errFlag += BOT_DATA_CANARY_CORRUPTED;
    }

    if(*(int*)((char*)stk->data + (stk->capacity + 1)*stk->elSize) != DataCanaryGuardTopREF)
    {
        errFlag += TOP_DATA_CANARY_CORRUPTED;
    }

    if(stk->StructCanaryGuardBot != StructCanaryGuardBotREF)
    {
        errFlag += BOT_STACK_CANARY_CORRUPTED;
    }

    if(stk->SructCanaryGuardTop != StructCanaryGuardTopREF)
    {
        errFlag += TOP_STACK_CANARY_CORRUPTED;
    }

    if(stk->HashSum != hash(stk->data, stk->capacity + 2)) 
    {
        errFlag += DATA_CORRUPTED;
    }

    return errFlag;
}

int VoidIntDump(Stack_t* stk)
{   
    FILE* log = fopen("log.txt", "a+b");
    
    if(log == NULL)
    {
        
        //fprintf(stderr, "roflreading error\n");
        stk->Error = FILE_CREATION_ERROR;
        
        fclose(log);
        
        return FILE_CREATION_ERROR;
        
    }

    //fprintf(stderr, "rofl FILE READ SUCCES\n");
    
    fprintf(log, 
    "########################## STACK INFO ##########################\n"
    "##CALLER STACK NAME: %s\n"
    "## CALLER STACK POINTER: %p\n"
    "## CALLER FILE    : %s\n"
    "## CALLER FUNCTION: %s\n"
    "## LINE          = %d\n"
    "## STACK SIZE    = %zu\n"
    "## STACK CAPACIY = %zu\n"
    "## STACK DATA: %p\n", stk->stkName, stk, stk->file, stk->func, stk->line, stk->size, stk->capacity, (char*)stk->data);
    
    //fprintf(stderr, "god end me\n");
    
    fprintf(log, "## DATA BUFFER: \n");
    
    fprintf(log, "### BOTTOM CANARY: %d\n", *(int*)((char*)stk->data));
    
    for(size_t i = 1; i < stk->capacity + 1; i++)
    {
        if(*(int*)((char*)stk->data + i*stk->elSize) == POISON)
        {
            fprintf(log, "# [%d]  (%zu) (POISON)\n", *(int*)((char*)stk->data + i*stk->elSize), i);
        }
        else
        {
            fprintf(log, "# [%d]  (%zu)\n", *(int*)((char*)stk->data + i*stk->elSize), i);
        }
        
    }
    
    fprintf(log, "### TOP CANARY: %d\n", *(int*)((char*)stk->data + (stk->capacity + 1)*stk->elSize));
    

    //fprintf(stderr, "rofl general read cussec\n");
    
    fprintf(log, "## HASH SUM: %zu\n", stk->HashSum);
    fprintf(log, "## ERRORS: ");
    
    uint16_t byte = 2; 
    
    for(size_t i = 0; i < sizeof(uint16_t)*8; i++)
    {
        
        fprintf(log, "%hu", (stk->Error & byte) ? 1 : 0);
        
        byte *= 2;
        
    }
    
    //fprintf(stderr, "rofl errprint succes\n");

    

    fprintf(log, "\n################################################################\n\n\n\n");
    
    fclose(log);
    
    return NO_ERRORS;
}
