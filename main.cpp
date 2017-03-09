#include <stdio.h>
#include "ID.hpp"

int main(){
  Decode id;
  printf("Op Code = %x -> ",id.opcode);
  switch(id.opcode){
    case 0: id.decodeR();
            break;
    case 0x2: id.decodeJ();
            break;
    default: id.decodeI();
  }
  return(0);
}