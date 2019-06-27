#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

  unsigned a = 10;
  unsigned b = 20;

  unsigned* result = (unsigned *) malloc(1*sizeof(unsigned)) ;
  *result = 0;
  unsigned additional;

  for(int i = 0; i < argc; ++i){
    *result = a + b;
    additional = *result + 5;
    if(additional != 20){
      break;
    }
    printf("argv[%d]: %s\n",i,argv[i]);
  }


  printf("Result: %d, Additional: %d\n", *result,additional);
  free(result);
  return 0;
}
