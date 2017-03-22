#include <stdio.h>
#include <fstream>
#include <iostream>
#include "ID.hpp"
#include "IF.hpp"
using namespace std;


const int MEM_SIZE = 1000000000;    //PC inc. by 1 byte, not 4; Branch/Jump offset already aligned (no << 2)
// <<<<<<< HEAD
// =======
int REGISTERS[32]; //need code to put ID answers into their regs
int MEMORY[MEM_SIZE]= {};
// >>>>>>> 1281814237362624300a28fc2843fe5f470d4747

int main(){
//**** Fetch Pipeline *****//
  Fetch IF;
  IF.getPC();
  IF.getInst();
  IF.fetchIF(MEMORY);
  // char PC=0;
  // PC= fopen("/Users/aliciaharper/Desktop/mips/PC", "r")
  // printf("%d\n", PC);
//**** Decode Pipeline *****//
  Decode id;
  id.input= IF.hexInstruction;
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