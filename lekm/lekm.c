#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"

/* Global Variables */
long long steps = 0, maxStack = 0, numVarIndir=0, numVarStraight=0, i=0, hotClos=0;
bool varIndir;

Environment *env, *freeEnv;

#define jump(ct, opcode) steps++; goto *codetable[opcode];

unsigned long long stackSize = ((unsigned long long) 2) << 20;
unsigned long long envSize = ((unsigned long long) 2) << 30;

Code* runlkm(Code* code){
  static const void *codetable[] = {&&APP, &&LAM, &&VAR};
  /// INITIALIZATION ///
  Stack stack = (Stack) { NULL, malloc(stackSize) };
  stack.head = stack.tail - 1;
  env = malloc(envSize); 
  freeEnv = env;
  Closure clos = (Closure) { code, NULL };
  //tmp variables
  int var; Environment* tmp;
  
  jump(codetable, clos.code->opcode);

APP:
  #ifdef TRACE
  printf("APP: "); trace(clos, stack);
  #endif
  maxStack = stack.head - stack.tail > maxStack ? stack.head - stack.tail : maxStack;
  if(stack.head == stack.tail + stackSize){
    printf("Error: stack overflow\n");
    exit(-1);
  }
  *(++stack.head) = (StackObject) { arg, .clos={ clos.code->m, clos.env } };
  jump(codetable,(++clos.code)->opcode);

LAM: 
  if(stack.head < stack.tail) return clos.code;
  if(stack.head->update) goto UPDATE;
  
  #ifdef TRACE
  printf("LAM: "); trace(clos,stack);
  #endif 
  
  tmp = clos.env;
  clos.env = freeEnv++;
  if(freeEnv == env + envSize){
    printf("Error: environment overflow\n");
    exit(-1);
  }
  *clos.env = (Environment) {(stack.head--)->clos, tmp};
  jump(codetable,(++clos.code)->opcode);

UPDATE:
  #ifdef TRACE
  printf("UPD: "); trace(clos,stack);
  #endif
  (stack.head--)->updLoc->clos = clos;
  jump(codetable,clos.code->opcode);

VAR:
  #ifdef TRACE
  printf("VAR: ");trace(clos,stack);
  #endif

  #ifdef DEBUG
  varIndir = 0;
  for(var=clos.code->var; var > 0; var--){
    if(clos.env - clos.env -> next > 1){
      varIndir=1;
    }
    clos.env = clos.env->next;
  }
  if(varIndir) numVarIndir ++;
  else numVarStraight ++;
  clos.env->clos.count ++;
  #else
  for(var=clos.code->var; var > 0; var--)
    clos.env = clos.env->next;
  #endif 

  //Shortcut, if closure is a value, dont bother pushing update, along with
  //collapsed marker update, both from Improving Lazy Kriving Machine
  if(clos.env->clos.code->opcode != OPLAM && stack.head[0].update == arg) 
    *(++stack.head) = (StackObject){upd, .updLoc=clos.env};
 
  hotClos = ++clos.env->clos.count > hotClos ? clos.env->clos.count : hotClos;
  clos = clos.env->clos;
  jump(codetable,clos.code->opcode);
  
  //Shouldn't reach here
  return NULL;
}
 
Code* createCode(Bytecode* bytecode){
  Code* code;
  code = malloc(sizeof(Code) * bytecode->len);
  int i;
  for(i=0; i < bytecode->len; i++){
    code[i].opcode = bytecode->code[i].opcode;
    switch(bytecode->code[i].opcode){
      case OPAPP:
        code[i].m = code + bytecode->code[i].arg;
        break;
      case OPLAM:
        code[i].var = bytecode->code[i].arg;
        break;
      case OPVAR:
        code[i].fVar = bytecode->code[i].arg;
        break;
    }
  }
  return code;
}

int main(int argc, char **argv){
  // File input 
  if(argc != 2){
    printf("usage: lkm <filename> \n");
  }
  else{
    Code* code = createCode(readCode(argv[1]));
    Code* result = runlkm(code);
    printf("Result: "); traceCode(result); printf("\n");
    printf("Took %llu steps \n", steps);
    #ifdef DEBUG
    printf("Hottest closure: %llu\n", hotClos);
    printf("Max stacksize = %llu \n", maxStack);
    printf("Max envsize = %lu \n", freeEnv - env);
    printf("%llu straight var lookups, %llu indirect var lookups\n",
           numVarStraight, numVarIndir);
    #endif

  }
  return 0;
}

