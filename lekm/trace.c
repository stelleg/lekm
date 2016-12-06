/* Tracing functions for debugging */
#include <stdio.h>
#include "code.h"

//Debugging trace functions
void traceCode(Code* code){
  switch(code->opcode){
    case OPAPP:
      printf("("); traceCode(code+1); printf(" "); traceCode(code->m); printf(")");
      break;
    case OPLAM:
      printf("\\"); traceCode(code+1);
      break;
    case OPVAR: 
      printf("%d", code->var); 
      break;
  }
}

void traceEnv(Environment* env){
  if(env){
      printf("\t[");
        traceCode(env->clos.code); 
        printf(", ");
        printf("%p", env->clos.env);
      printf("]\n");
      traceEnv(env->next);
  }
}

void traceStack(Stack stack){
  StackObject* stackptr;
  for(stackptr = stack.head; stackptr >= stack.tail; stackptr--){
    printf("\t{");
      if(stackptr->update) 
        printf("update %p", stackptr->updLoc);
      else{
        printf("arg "); 
        traceCode(stackptr->clos.code); 
        printf(", ");
        printf("%p", stackptr->clos.env);
      }
    printf("}\n");
  }
}

//Full state trace function
void trace(Closure clos, Stack stack){
  printf("Stacksize: %ld, ", stack.head + 1 - stack.tail); 
  traceCode(clos.code); printf("\n");
  traceEnv(clos.env);
  traceStack(stack);
}

