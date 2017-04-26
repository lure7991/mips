#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "ID.hpp"
#include "EX.hpp"
#include "MEM.hpp"

using namespace std;

int cycleCount = 0;

//int reg[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31}; //need code to put ID answers into their regs
int reg[32];
int memory[1200];

int instruction[1024];
int instructionIndex = 0;

int opcode[4];
int rd[4] = {0,0,0,0};
int rs[4] = {0,0,0,0};
int rt[4] = {0,0,0,0};
int shamt[4];
int funct[4];
int pc;
int sp;
int fp;
int ra;
int address[4];
int immediate[4];

//other///////////////////////////////////////////////
// >>>>>>> 1281814237362624300a28fc2843fe5f470d4747
//to compile on lucas machine: g++ -mcmodel=medium -Wall -Wextra -o output main.cpp
//////////////////////////////////////////////////////

int main(){
	
//Instruction Fetch Pipeline//////////////////////////////////////////////////////////////
printf("\nInstruction Fetch:\n\n");
	
	string instructionInput;
  ifstream inputFile("instruction.txt");
	
	instructionIndex = 0;
  if (inputFile.is_open()){
		instructionIndex = 0;
    while (getline(inputFile,instructionInput)){
    	instruction[instructionIndex] = strtoul(instructionInput.substr(2, 10).c_str(), NULL, 16);
			instructionIndex++;
    }
    inputFile.close();
  } 
	instruction[instructionIndex] = -1;
	
	sp = instruction[0];
	fp = instruction[1];
	pc = instruction[5];
	
	printf("	sp = 0x%x\n",sp);
	printf("	fp = 0x%x\n",fp);
	printf("	pc = 0x%x\n",pc);
	
	int answer = 1;
	int nextInstruction = 10;
	
									//while(instruction[cycleCount] != -1){
									while(answer){
										printf("\nInstruction Fetch:\n\n");
										printf("\n	Instruction: 0x%x\n",instruction[nextInstruction]);
	
//Decode Pipeline//////////////////////////////////////////////////////////////////////////
printf("\nDecode:\n\n");
Decode id;
	
	id.input = instruction[nextInstruction];
  //example inputs/////////////////////////////////////////
  //id.input = 0x014B4820; //R-Type: ADD $t1 $t2 $t3
  //id.input = 0x21280007; //I-Type: addi t0 t1 0x0007
  //id.input = 0x3008FFFF; //I-Type: andi $t0,$zero,0xFFFF
  //id.input = 0x08000004; //J-Type: j 0x0004
  //id.input = 0x3008FFFF;
	//id.input = 0x8d090000; //I-Type: lw $t1, 0($t0)
	//id.input = 0xad090000; //I-Type: sw $t1, 0($t0)
	/////////////////////////////////////////////////////////
	
  printf("	Input: 0x%x\n", id.input);
  
  id.setOP();
  opcode[1] = id.opcode;
	printf("	Opcode: 0x%x\n", id.opcode);
  switch(id.opcode){
    case 0: 
              id.decodeR();
              rd[1] = id.rd;
              rs[1] = id.rs;
              rt[1] = id.rt;
              shamt[1] = id.shamt;
              funct[1] = id.funct;
							pc = id.pc;
	
              printf("\n	Recieved R-Type: \n");
              printf("		 Rd = %d\n",reg[rd[1]]);
              printf("		 Rs = %d\n",reg[rs[1]]);
              printf("		 Rt = %d\n",reg[rt[1]]);
              printf("		 Shamt = %d\n",shamt[1]);
              printf("		 Function = 0x%x\n\n",funct[1]); 
              break;
    case 0x2: 
              id.decodeJ();
              pc = id.pc;
              ra = id.ra;
              address[1] = id.address;
	
              printf("\n	Recieved J-Type: \n");        
              printf("		 PC = 0x%x\n",pc);
              printf("		 ra = 0x%x\n",ra);
              printf("		 Adress = 0x%x\n\n",address[1]);    
              break;
		case 0x1f:
							id.special();
							rd[1] = id.rd;
							printf("\n	Recieved Special Type: seb\n");
              printf("		 Rd = %d\n",reg[rd[1]]);
    default: 
              id.decodeI();
              rs[1] = id.rs;
              rt[1] = id.rt;
              immediate[1] = id.immediate;
	
              printf("\n	Recieved I-Type: \n");
              printf("		 Rs = %d\n",reg[rs[1]]);
              printf("		 Rt = %d\n",reg[rt[1]]);
              printf("		 Immediate = %d\n\n",immediate[1]);
              break;
  }
  
//****ID-EX Shadow Variables***************************************************************
	
	opcode[2] = opcode[1];
	rd[2] = rd[1];
	rt[2] = rt[1];
	rs[2] = rs[1];
	address[2] = address[1];
	shamt[2] = shamt[1];
	funct[2] = funct[1];
	
//Execute Pipeline//////////////////////////////////////////////////////////////////////////
printf("Execute:\n\n");
Execute ex;
ex.opcode = opcode[2];
ex.rd = reg[rd[2]];
ex.rt = reg[rt[2]];
ex.rs = reg[rs[2]];
ex.address = address[2];
ex.shamt = shamt[2];
ex.function = funct[2];

	
  switch(ex.opcode){ //select format
    case 0: 
						ex.executeR();
						reg[rd[2]] = ex.rd; //save the new rd back to the reg
						//note that pc is changed during the id pipeline
						
						printf("\n	Result: ");
						printf("Writing: Rd = %d to Reg #%d\n",reg[rd[2]], rd[2]);
            break;
    case 0x2: 
						ex.executeJ();
						ra = ex.ra; 
						pc = ex.pc;
						
						printf("\n	Result: ");
						printf("ra = %d \n",ra);
						printf("		pc = %d\n",pc);
            break;
		case 0x1f: 
						ex.special();
						reg[rt[2]] = ex.rt;
						
						printf("\n	Result: ");
						printf("Writing: Rt = %d to Reg #%d\n",reg[rt[2]], rt[2]);
    default: 
						ex.executeI();
						pc = ex.pc;
						reg[rt[2]] = ex.rt;
						
						printf("\n	Result: ");
						printf("Writing: Rt = %d to Reg #%d\n",reg[rt[2]], rt[2]);
  }
  printf("\n");
	
//****EX-MEM Shadow Variables***************************************************************
	
  opcode[3] = opcode[2];
	rd[3] = rd[2];
	rt[3] = rt[2];
	rs[3] = rs[2];
	address[3] = address[2];
	shamt[3] = shamt[2];
	funct[3] = funct[2];
	
//Memory Pipeline//////////////////////////////////////////////////////////////////////////
printf("Memory:\n\n");
Memory mem;
mem.opcode = opcode[3];
mem.rd = reg[rd[3]];
mem.rt = reg[rt[3]];
mem.rs = reg[rs[3]];
mem.address = address[3];
mem.shamt = shamt[3];
mem.function = funct[3];


	printf("	Mem Opcode: 0x%x\n", mem.opcode);
	switch(mem.opcode){
		case 0x24: 
						mem.load();
            break;
		case 0x25:  
						mem.load();
            break;
		case 0x30: 
						mem.load();
            break;
		case 0xf: 
						mem.load();
            break;
		case 0x23:  
						mem.load();
            break;
		case 0x28:
						mem.store();
            break;
		case 0x38: 
						mem.store();
            break;
		case 0x29:
						mem.store();
            break;
		case 0x2b:  
						mem.store();
            break;
	}
	printf("\n");
	
//****MEM-WB Shadow Variables***************************************************************
	
  opcode[4] = opcode[3];
	rd[4] = rd[3];
	rt[4] = rt[3];
	rs[4] = rs[3];
	address[4] = address[3];
	shamt[4] = shamt[3];
	funct[4] = funct[3];

//Write-Back Pipeline//////////////////////////////////////////////////////////////////////////
	
	printf("	Reg values after pipeline: \n\n");
	int i;
	for(i=0; i<32; i++){
		printf("	Reg #%d = %d\n",i,reg[i]);
	}
	printf("\n");
	
////////////////////////////////////END OF PIPELINE/////////////////////////////////////////////////////////////
	cycleCount++;
	nextInstruction++;
	printf("\nCurrent Cycle Count = %d\n\n",cycleCount);
	
				printf("\nWould you like to run another instruction? (1/0) --> ");
				scanf("%d", &answer);
				if(!answer){
					printf("\nFinal Cycle Count = %d\n\n",cycleCount);
					return(0);
				}
	}
	printf("\nFinal Cycle Count = %d\n\n",cycleCount);
  return(0);
}