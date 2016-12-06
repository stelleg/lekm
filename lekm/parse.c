#include "code.h"
#include <stdlib.h>
#include <stdio.h>

static long bufSize = 1048576;
char* buffer;
Code* code = malloc(bufSize);

Code* parseTerm(){
  switch(*buffer){
    case '(':
      Code* loc = code++; 
      parseTerm(); 
      parseChar(' ');
      parseTerm();
      parseChar(')');
      loc = (Code) {APP, code};
    case '\\':
      *code++ = (Code) {LAM, { }} ;
      parseVar(); 
      parseChar('.'); 
      parseTerm(); 
    default:
      parseVar();
      return
  }
}

Code* parseFile(char* filename){  
  int fgetc = fopen(filename, "r");    
  Code* code = malloc(sizeof(code));
  fread(code, sizeof(char), bufSize, stream);

  code->code = malloc(len);
  printf("Allocated %ld bytes at %p\n", len, bytecode->code);
  printf("Size of term in bytes: %lu \n", sizeof(bytecode->code));
  printf("File is %ld bytes, meaning %ld terms\n", len, len/sizeof(bytecode->code));
  long readlen = fread(bytecode->code, sizeof(bytecode->code), len/sizeof(bytecode->code), fp);
  bytecode->len = readlen;
  return bytecode;  

}
