#include "stack.h"
#include "StackDB.h"


static unsigned int POISON = 2122219134;
static unsigned int* POISON_PTR = &POISON; 

int StackInit(Stack_t* stk, size_t InitCapacity)
{

    if(stk == NULL)
    {
        return STACK_PTR_IS_NULL;
    }

    if((stk->data = calloc(InitCapacity ON_DEBUG( + 2), stk->elSize)) == NULL)
    {
        fprintf(stderr, "Dynamic Memory dead\n");
        stk->Error += ALLOC_ERROR;
        return ALLOC_ERROR; 
    }

    stk->size = 0;
    stk->capacity = InitCapacity;

    #ifndef NDEBUG
    memset((char*)stk->data + 1*stk->elSize, POISON, stk->capacity*stk->elSize);
    stk->SructCanaryGuardTop = StructCanaryGuardTopREF;
    stk->StructCanaryGuardBot = StructCanaryGuardBotREF;
    *(int*)stk->data = DataCanaryGuardBotREF;
    *(int*)((char*)stk->data + (stk->capacity + 1)*stk->elSize) = DataCanaryGuardTopREF;
    stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif

    return NO_ERRORS;
}
                   
int StackDtor(Stack_t* stk)
{
    STACK_ASSERT(stk);
    void* ptr = memset(stk->data, 0, stk->elSize*stk->capacity);

    if(ptr == NULL)
    {
        stk->Error += MEMSET_FAILURE;
        free(stk->data);
        return MEMSET_FAILURE;
    }

    free(stk->data);

    return NO_ERRORS; 
}

int StackPush(Stack_t* stk, void* elem)
{
    //STACK_ASSERT(stk);

    if(stk->size >= stk->capacity)
    {
        if(StackResize(stk, false) != 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
    }
    
    cpybytes((char*)stk->data + (stk->size ON_DEBUG( + 1))*stk->elSize, elem, stk->elSize);
    stk->size++;
    
    #ifndef NDEBUG
    stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif
    STACK_ASSERT(stk);

    return NO_ERRORS;
}


int StackPop(Stack_t* stk, void* elem)
{
    
    STACK_ASSERT(stk);

    if(stk->size <= 0)
    {
        stk->Error = STACK_UNDERFLOW;
    }
    
    if(stk->size <= stk->capacity/4)
    {
        
        if(StackResize(stk, true) != 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            return ALLOC_ERROR;
        }
        
    }

    cpybytes(elem, (char*)stk->data + stk->size*stk->elSize, stk->elSize);
    
    cpybytes((char*)stk->data + stk->size*stk->elSize, POISON_PTR, stk->elSize);
    
    stk->size--;


    #ifndef NDEBUG
    stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif
    STACK_ASSERT(stk);
    
    return NO_ERRORS;
}

int StackResize(Stack_t* stk, bool downSizeFlag)
{

    STACK_ASSERT(stk);

    if(downSizeFlag)
    {
        if((stk->data = wrecalloc(stk->data, stk->capacity/REALLOC_COEF ON_DEBUG( + 2), stk->elSize, stk->size)) == 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            
            return ALLOC_ERROR;
        }

        stk->capacity /= REALLOC_COEF;

        #ifndef NDEBUG
        *(int*)((char*)stk->data + (stk->capacity + 1)*stk->elSize) = DataCanaryGuardTopREF;     
        stk->HashSum = hash(stk->data, stk->capacity + 2);
        #endif

        return NO_ERRORS;
    }

    else
    {
        if((stk->data = wrecalloc(stk->data, stk->capacity*REALLOC_COEF ON_DEBUG( + 2), stk->elSize, stk->size))== 0)
        {
            stk->Error += ALLOC_ERROR;
            free(stk->data);
            
            return ALLOC_ERROR;
        }

        stk->capacity *= REALLOC_COEF;

        #ifndef NDEBUG
        stk->HashSum = hash(stk->data, stk->capacity + 2);
        *(int*)((char*)stk->data + (stk->capacity + 1)*stk->elSize) = DataCanaryGuardTopREF;
        #endif

        return NO_ERRORS;
    } 

    #ifndef NDEBUG
    stk->HashSum = hash(stk->data, stk->capacity + 2);
    #endif


    STACK_ASSERT(stk);
    return NO_ERRORS;
}

void* wrecalloc(void* ptr, size_t num, size_t size, size_t PrevSize) //POISON
{
    
    void* tptr = calloc(num, size);
    if(tptr == NULL)
    {
        free(ptr);
        free(tptr);
        return NULL;
    }

    memset(ON_DEBUG((char*))tptr ON_DEBUG( + 1*size) , POISON, (num - 1 ON_DEBUG( - 1))*size);
    memcpy((char*)tptr, ptr, (PrevSize ON_DEBUG(+ 1))*size);
    
    free(ptr);
    
    //tptr = memset((char*)tptr + PrevSize*size, 1, size*num - PrevSize*size);
    
    return tptr;
} 







/*void errParse(int e)
{
    switch(e)
    {
        case ALLOC_ERROR:
        {
            fprintf(stderr, "Cannot allocate memory.\n");
            break;
        }

        case DESTRUCTOR_ERROR:2122219134
            fprintf(stderr, "Could not destruct stack, closing program.");
            break;
        }

        case FILE_CREATION_ERROR:
        {
            fprintf(stderr, "Could not open existing or create new log.txt file.");
            break;
        }

        case MEMSET_FAILURE:
        {
            fprintf(stderr, "Could not initialise allocated memory.");
            break;
        }

        case STACK_UNDERFLOW:
        {
            fprintf(stderr, "You tried to grab an element from an empty stack.");
            break;
        }

        default:
        {
            fprintf(stderr, "An undefined error appeared. Please, make a report to make program better.");
        }
    }
}*/