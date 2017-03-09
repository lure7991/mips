#include <stdio.h>

void printBinary(int input){
  int tooLong = 0;
  while(input || (tooLong<=52)){
    if(input) printf("%d",1);
    else printf("%d",0);
    input >>= 1;
    tooLong++;
  }
  printf("\n");
}

int main(){
  int input = 0x014B4820;
  int test = 0b10101010101010101010101010101010;
  printf("%x\n",input);
  printBinary(test);
  return(0);
}