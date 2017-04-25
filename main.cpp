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

char opType[4];
int rd[4];
int rs[4];
int rt[4];
int shamt[4];
int funct[4];
int pc;
int ra;
int address[4];
int immediate[4];

int main(){
    
//**** Decode Pipeline *****//
  printf("\nIn Decode Stage\n");
  Decode id;
  
  //example inputs
  id.input = 0x014B4820; //R-Type: ADD $t1 $t2 $t3
  //id.input = 0x21280007; //I-Type: addi t0 t1 0x0007
  //id.input = 0x3008FFFF; //I-Type: andi $t0,$zero,0xFFFF
  //id.input = 0x08000004; //J-Type: j 0x0004
  //id.input = 0x3008FFFF;
  
  //id.input= IF.hexInstruction;
  printf("Input: 0x%x\n", id.input);
  
  id.setOP();
  printf("Opcode: %d\n", id.opcode);
  switch(id.opcode){
    case 0: 
              id.decodeR();
              opType[1] = 'r';
              rd[1] = id.rd;
              rs[1] = id.rs;
              rt[1] = id.rt;
              shamt[1] = id.shamt;
              funct[1] = id.funct;
              printf("\nRecieved R-Type: \n");
              printf("Rd = %d\n",rd);
              printf("Rs = %d\n",rs);
              printf("Rt = %d\n",rt);
              printf("Shamt = %d\n",shamt);
              printf("Function = 0x%x\n\n",funct); 
              break;
    case 0x2: 
              id.decodeJ();
              opType[1] = 'j';
              pc = id.pc;
              ra = id.ra;
              address[1] = id.address;
              printf("\nRecieved J-Type: \n");        
              printf("PC = 0x%x\n",pc);
              printf("ra = 0x%x\n",ra);
              printf("Adress = 0x%x\n\n",address);        
              break;
    default: 
              id.decodeI();
              opType[1] = 'i';
              rs[1] = id.rs;
              rt[1] = id.rt;
              immediate[1] = id.immediate;
              printf("\nRecieved I-Type: \n");
              printf("Rs = %d\n",rs);
              printf("Rt = %d\n",rt);
              printf("Immediate = %d\n\n",immediate);
              break;
  }
  printf("Decoded instruction complete\n\n");
  
  //**** Execute Pipeline *****//
  
  
  
  switch(opcode){ //select format
    case 0: executeR();
						//reg[rd_reg] = rd; //save the new rd back to the reg
						//if command is jr, note that pc is changed in the executeR() 
            break;
    case 0x2: executeJ();
            break;
    default: executeI();
  }
  
  return(0);
}