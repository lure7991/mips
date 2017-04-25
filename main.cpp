#include <stdio.h>
#include <fstream>
#include <iostream>
#include "ID.hpp"
#include "EX.hpp"
//#include "IF.hpp"
using namespace std;

const int MEM_SIZE = 1000000000;    //PC inc. by 1 byte, not 4; Branch/Jump offset already aligned (no << 2)
// <<<<<<< HEAD
// =======
int REGISTERS[32]; //need code to put ID answers into their regs
int MEMORY[MEM_SIZE]= {};
// >>>>>>> 1281814237362624300a28fc2843fe5f470d4747

char opType[4];
int opcode[4];
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
  opcode[1] = id.opcode;
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
              printf("Rd = %d\n",rd[1]);
              printf("Rs = %d\n",rs[1]);
              printf("Rt = %d\n",rt[1]);
              printf("Shamt = %d\n",shamt[1]);
              printf("Function = 0x%x\n\n",funct[1]); 
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
              printf("Adress = 0x%x\n\n",address[1]);        
              break;
    default: 
              id.decodeI();
              opType[1] = 'i';
              rs[1] = id.rs;
              rt[1] = id.rt;
              immediate[1] = id.immediate;
              printf("\nRecieved I-Type: \n");
              printf("Rs = %d\n",rs[1]);
              printf("Rt = %d\n",rt[1]);
              printf("Immediate = %d\n\n",immediate[1]);
              break;
  }
  printf("Decoded instruction complete\n\n");
  
  //**** Execute Pipeline *****//
  
  ex.opcode = opcode[1];
  ex.rd = rd[1];
  ex.rt = rt[1];
  ex.rs = rs[1];
  ex.address = address[1];
  ex.shamt = shamt[1];
  ex.function = funct[1];
  ex.opcode = opcode[1];
  
  switch(ex.opcode){ //select format
    case 0: ex.executeR();
						//reg[rd_reg] = rd; //save the new rd back to the reg
						//if command is jr, note that pc is changed in the executeR() 
            break;
    case 0x2: ex.executeJ();
            break;
    default: ex.executeI();
  }
  
  return(0);
}