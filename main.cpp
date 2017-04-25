#include <stdio.h>
#include <fstream>
#include <iostream>
#include "ID.hpp"
//#include "IF.hpp"
using namespace std;

const int MEM_SIZE = 1000000000;    //PC inc. by 1 byte, not 4; Branch/Jump offset already aligned (no << 2)
// <<<<<<< HEAD
// =======
int REGISTERS[32]; //need code to put ID answers into their regs
int MEMORY[MEM_SIZE]= {};
// >>>>>>> 1281814237362624300a28fc2843fe5f470d4747

char opType;
long int rd;
long int rs;
long int rt;
long int shamt;
long int funct;
long int pc;
long int ra;
long int address;
long int immediate;

int main(){
  
// //**** Fetch Pipeline *****//
//   Fetch IF;
//   IF.getPC();
//   IF.getInst();
//   IF.fetchIF(MEMORY);
//   // char PC=0;
//   //PC= fopen("/Users/aliciaharper/Desktop/mips/PC", "r");
//   // printf("%d\n", PC);
//   printf("%u\n", IF.hexPC);
  
//**** Decode Pipeline *****//
  Decode id;
  //id.input= IF.hexInstruction;
  printf("\nIn Decode Stage\n");
  
  //example inputs
  //id.input = 0x014B4820; //R-Type: ADD $t1 $t2 $t3
  //id.input = 0x21280007; //I-Type: addi t0 t1 0x0007
  //id.input = 0x3008FFFF; //I-Type: andi $t0,$zero,0xFFFF
  id.input = 0x08000004; //J-Type: j 0x0004
  //id.input = 0x3008FFFF;
  printf("Input: 0x%x\n", id.input);
  
  id.setOP();
  printf("Opcode: %d\n", id.opcode);
  switch(id.opcode){
    case 0: id.decodeR();
//               opType = 'r';
//               rd = id.rd;
//               rs = id.rs;
//               rt = id.rt;
//               shamt = id.shamt;
//               funct = id.funct;
              printf("\nRecieved R-Type: \n");
              printf("Rd = %d\n",id.rd);
              printf("Rs = %d\n",id.rs);
              printf("Rt = %d\n",id.rt);
              printf("Shamt = %d\n",id.shamt);
              printf("Function = 0x%x\n\n",id.funct); 
              break;
    case 0x2: id.decodeJ();
//               opType = 'j';
//               pc = id.pc;
//               ra = id.ra;
//               address = id.address;
              printf("\nRecieved J-Type: \n");        
              printf("PC = 0x%x\n",id.pc);
              printf("ra = 0x%x\n",id.ra);
              printf("Adress = 0x%x\n\n",id.address);        
              break;
    default: id.decodeI();
//               opType = 'i';
//               rs = id.rs;
//               rt = id.rt;
//               immediate = id.immediate;
              printf("\nRecieved I-Type: \n");
              printf("Rs = %d\n",id.rs);
              printf("Rt = %d\n",id.rt);
              printf("Immediate = %d\n\n",id.immediate);
              break;
  }
  printf("Decoded instruction complete\n\n");
  
  return(0);
}