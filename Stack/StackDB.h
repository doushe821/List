
#ifndef STACK_DB
#define STACK_DB


#define NDEBUG

#include "Stack.h"
#ifdef NDEBUG
#define ON_DEBUG(code) 
#else
#define ON_DEBUG(code) code
#endif //NDEBUG



const size_t UNLIKELY_STACK_SIZE = 666666666666666666; // Very unlikely to appear, used as a sign of stack underflow.


void StackAssert(Stack_t* stk);
int StackError(Stack_t* stk);

#endif