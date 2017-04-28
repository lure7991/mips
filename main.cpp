#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "IF.hpp"
#include "ID.hpp"
#include "EX.hpp"
#include "MEM.hpp"

using namespace std;

int cycleCount = 0;

//int reg[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31}; //need code to put ID answers into their regs
int reg[32];
int memory[1200];

int instruction[500];
int instructionIndex = 0;

int pc;
int sp;
int fp;
int ra;

int opcode;
int rd;
int rs;
int rt;
int shamt;
int funct;
int address;
int immediate;

int main(){
	
//Instruction Fetch Pipeline//////////////////////////////////////////////////////////////
printf("\nInstruction Fetch:\n");
Fetch IF;
	
	IF.fill();
	
	instructionIndex = IF.instructionIndex;
	
	sp = IF.sp;
	fp = IF.fp;
	pc = IF.pc;
	
	printf("	sp = 0x%x\n",sp);
	printf("	fp = 0x%x\n",fp);
	printf("	pc = %d\n",pc);
	
	int answer = 1;
	//int nextInstruction = 10; //PC-1;
	
							//while(pc){
							//while(instruction[cycleCount] != -1){
							pc--;
							while(answer){
										printf("\nInstruction Fetch:\n\n");
										pc++;
										printf("\n	Instruction: 0x%x\n",IF.instruction[pc]);
										printf("	pc = %d\n",pc);

//Decode Pipeline//////////////////////////////////////////////////////////////////////////
	printf("\nDecode:\n\n");
	Decode id;
	
	id.pc = pc;
	id.input = IF.instruction[pc];
	
  printf("	Input: 0x%x\n", id.input);
  
  id.setOP();
  opcode = id.opcode;

	printf("	Opcode: 0x%x\n", id.opcode);
  switch(id.opcode){
    case 0: 
              id.decodeR();
              rd = id.rd;
              rs = id.rs;
              rt = id.rt;
              shamt = id.shamt;
              funct = id.funct;
							pc = id.pc;
	
              printf("\n	Recieved R-Type: \n");
              printf("		 Rd = %d\n",reg[rd]);
              printf("		 Rs = %d\n",reg[rs]);
              printf("		 Rt = %d\n",reg[rt]);
              printf("		 Shamt = %d\n",shamt);
              printf("		 Function = 0x%x\n\n",funct); 
              break;
    case 0x2: 
              id.decodeJ();
              pc = id.pc;
              ra = id.ra;
              address = id.address;
	
              printf("\n	Recieved J-Type: \n");        
              printf("		 pc = %d\n",pc);
              printf("		 ra = 0x%x\n",ra);
              printf("		 Adress = 0x%x\n\n",address);    
              break;
		case 0x1f:
							id.special();
							rd = id.rd;
							printf("\n	Recieved Special Type: seb\n");
              printf("		 Rd = %d\n",reg[rd]);
    default: 
              id.decodeI();
              rs = id.rs;
              rt = id.rt;
							pc = id.pc;
              immediate = id.immediate;
	
              printf("\n	Recieved I-Type: \n");
							printf("		 pc = %d\n",pc);
              printf("		 Rs = %d\n",reg[rs]);
              printf("		 Rt = %d\n",reg[rt]);
              printf("		 Immediate = %d\n\n",immediate);
              break;
  }
	
//Execute Pipeline//////////////////////////////////////////////////////////////////////////
	printf("Execute:\n\n");
	Execute ex;
	ex.opcode = opcode;
	ex.rd = reg[rd];
	ex.rt = reg[rt];
	ex.rs = reg[rs];
	ex.address = address;
	ex.shamt = shamt;
	ex.function = funct;
	
  switch(ex.opcode){ //select format
    case 0: 
						ex.executeR();
						reg[rd] = ex.rd;
						
						printf("\n	Result: ");
						printf("Writing: Rd = %d to Reg #%d\n",reg[rd], rd);
            break;
    case 0x2: 
// 						ex.executeJ();
// 						ra = ex.ra; 
// 						//pc = ex.pc;
						
// 						printf("\n	Result: ");
// 						printf("ra = %d \n",ra);
// 						printf("		pc = %d\n",pc);
            break;
		case 0x1f: 
						ex.special();
						reg[rt] = ex.rt;
						
						printf("\n	Result: ");
						printf("Writing: Rt = %d to Reg #%d\n",reg[rt], rt);
    default: 
						ex.executeI();
						//pc = ex.pc;
						reg[rt] = ex.rt;
						
						printf("\n	Result: ");
						printf("Writing: Rt = %d to Reg #%d\n",reg[rt], rt);
  }
  printf("\n");
	
//Memory Pipeline//////////////////////////////////////////////////////////////////////////
	printf("Memory:\n\n");
	Memory mem;
	mem.opcode = opcode;
	mem.rd = reg[rd];
	mem.rt = reg[rt];
	mem.rs = reg[rs];
	mem.address = address;
	mem.shamt = shamt;
	mem.function = funct;
	mem.pc = pc;
								
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

//Write-Back Pipeline//////////////////////////////////////////////////////////////////////////
	
	printf("	Reg values after pipeline: \n\n");
	int i;
	for(i=0; i<32; i++){
		printf("	Reg #%d = %d\n",i,reg[i]);
	}
	printf("\n");
	
////////////////////////////////////END OF PIPELINE/////////////////////////////////////////////////////////////
	cycleCount++;
	//nextInstruction++;
	printf("	\npc = %d\n",pc);
	printf("Current Cycle Count = %d\n\n",cycleCount);
	
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