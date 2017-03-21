#include <stdio.h>
#include "ID.hpp"

const int GLOBAL_CONST_MEMORY= 100000;    //PC inc. by 1 byte, not 4; Branch/Jump offset already aligned (no << 2)

int main(){
//**** Fetch Pipeline *****//
  int PC = 0; 
  

//**** Decode Pipeline *****//
  Decode id;
  printf("In Decode Stage\n");
  switch(id.opcode){
    case 0: id.decodeR();
            break;
    case 0x2: id.decodeJ();
            break;
    default: id.decodeI();
  }
  printf("Decoded instruction complete");
  return(0);
}