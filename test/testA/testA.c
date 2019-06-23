#include <stdio.h>

int main(int argc, char *argv[]){

  unsigned a = 10;
  unsigned b = 20;

  unsigned result;
  unsigned additional;

  for(int i = 0; i < argc; ++i){
    result = a + b;
    additional = result + 5;
    printf("argv[%d]: %s\n",i,argv[i]);
  }


  printf("Result: %d, Additional: %d\n", result,additional);
  return 0;
}
