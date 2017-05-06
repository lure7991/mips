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

int reg[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int instruction[500];
int instructionIndex = 0;

int pc;
int sp = 29;
int fp = 30;
int ra = 31;

int bjPC;
bool bj = false;
int branchOn = 0;

int opcode;
int rd;
int rs;
int rt;
int shamt;
int funct;
int address;
int immediate;

void printReg(){
	printf("Reg values: \n");
	int i;
	for(i=0; i<32; i++){
		printf("	Reg #%d = %d\n",i,reg[i]);
	}
}

void printMem(){
	// printf("Mem values after pipeline: \n");
	int i;
	for(i=0; i<1200; i++){
// 		if(mem.memory[i]) 
			printf("Mem #%d = %d	",i,mem.memory[i]);
	}
}

int main(){

//Instruction Fetch Pipeline//////////////////////////////////////////////////////////////
//printf("\nInstruction Fetch:\n");
	Fetch IF;

	IF.fill();
	
	if(sp>31 || fp>31) printf("uh oh 1\n");
	reg[sp] = IF.sp;
	reg[fp] = IF.fp/4;
	pc = IF.pc;

	printf("	sp = %d\n",reg[sp]);
	//printf("	fp = %d\n",reg[fp]);
	printf("	pc = %d\n",pc);
	
	Memory mem;
	int fill;
	for(fill=0;fill<1200;fill++) mem.memory[fill] = IF.instruction[fill];

	//while(pc){
//  	int w;
//  	for(w=0;w<20000;w++){
// 		if(pc>=500){
// 			printf("pc beyond 500\n");
// 			printf("	pc = %d\n",pc);
// 			return 0;
// 		}
		
		int answer = 1;
		while(answer){
			
		pc++;
		if(pc == branchOn) pc = bjPC;
 		printf("\nInstruction: 0x%x\n",IF.instruction[pc]);
		printf("pc = %d\n",pc);
		printf("cycleCount = %d\n",cycleCount);

//Decode Pipeline//////////////////////////////////////////////////////////////////////////
//printf("\nDecode:\n\n");
		Decode id;
		reg[0] = 0;
		id.pc = pc;
		id.bj = false;
		id.input = IF.instruction[pc];

		id.setOP();
		opcode = id.opcode;

		switch(id.opcode){
			case 0: 
				id.decodeR();
				printf("R-Type\n");
				rd = id.rd;
				rs = id.rs;
				rt = id.rt;
				shamt = id.shamt;
				funct = id.funct;
				//printf("rd = %d\n",rd);
				//printf("rs = %d\n",rs);
				//printf("rt = %d\n",rt);
				//printf("shift amount = %d\n",shamt);
				//printf("function = %d\n",funct);
				break;
			case 0x2: 
				id.decodeJ();
				printf("J-Type\n");
				if(ra>31 || ra<0) printf("1: trying to access out of bounds reg\n");
				reg[ra] = id.ra;
				address = id.address;   
				printf("ra = %d\n",reg[ra]);
				printf("address = %d\n",address);
				break;
			case 0x3: 
				id.decodeJ();
				printf("J-Type\n");
				if(ra>31 || ra<0) printf("2: trying to access out of bounds reg\n");
				reg[ra] = id.ra;
				address = id.address;  
				printf("ra = %d\n",reg[ra]);
				printf("address = %d\n",address);
				break;
			case 0x1f:
				id.special();
				printf("Special-Type\n");
				rd = id.rd;
				printf("rd = %d\n",rd);
			default: 
				id.decodeI();
				printf("I-Type\n");
				rs = id.rs;
				rt = id.rt;
				immediate = id.immediate;
				//printf("rs = %d\n",rs);
				//printf("rt = %d\n",rt);
				//printf("immediate = %d\n",immediate);
				break;
		}
		
		bj = id.bj;
		if(bj){
			printf("Branch Taken\n");
			bjPC = id.pc + 1;
			branchOn = pc + 2;
		}
		else if(!bj){
//Execute Pipeline//////////////////////////////////////////////////////////////////////////
//printf("Execute:\n\n");
	Execute ex;
	reg[0] = 0;
	ex.opcode = opcode;
	if(rd>31 || rt>31 || rs>31 || rd<0 || rt<0 || rs<0) printf("3: trying to access out of bounds reg\n");
	ex.rd = reg[rd];
	ex.rt = reg[rt];
	ex.rs = reg[rs];
	ex.immediate = immediate;
	ex.address = address;
	ex.shamt = shamt;
	ex.function = funct;
	ex.pc = pc;
		
	switch(ex.opcode){
		case 0: 
			ex.executeR();
			if(rd>31 || rd<0) printf("4: trying to access out of bounds reg\n");
			reg[rd] = ex.rd;
			break;
		case 0x2: //j-type
			break;
		case 0x1f: 
			ex.special();
			if(rt>31 || rt<0) printf("5: trying to access out of bounds reg\n");
			reg[rt] = ex.rt;
			break;
		default: 
			ex.executeI();
			pc = ex.pc;
			if(rt>31 || rt<0) printf("6: trying to access out of bounds reg\n");
			reg[rt] = ex.rt;
			//printf("rt = %d --> reg #%d\n",reg[rt], rt);
			break;
	}
	
//Memory Pipeline//////////////////////////////////////////////////////////////////////////
//printf("Memory:\n\n");

	reg[0] = 0;
	mem.opcode = opcode;
	if(rd>31 || rt>31 || rs>31 || rd<0 || rt<0 || rs<0) printf("7: trying to access out of bounds reg\n");
	//mem.rd = reg[rd];
	
	mem.rt = reg[rt];
	mem.rs = reg[rs];
	mem.immediate = immediate;
	//mem.address = address;
	//mem.shamt = shamt;
	//mem.function = funct;
		
	//mem.pc = pc;
	//mem.cycleCount = cycleCount;

 	mem.doMem();
		
	if(mem.load){
		if(mem.rt>31 || mem.rt<0) printf("8: trying to access out of bounds reg\n");
		reg[rt] = mem.rt;
	}
		}
// Write-Back Pipeline//////////////////////////////////////////////////////////////////////////
////////////////////////////////////END OF PIPELINE/////////////////////////////////////////////////////////////
	cycleCount++;
	
	reg[0] = 0;
			
	printf("Would you like to run another instruction? (1/0) --> ");
	scanf("%d", &answer);
	//if(cycleCount>=20000) answer = false;
	if(!answer){
		printf("Final Cycle Count = %d\n",cycleCount);
	}
}

 	printMem();
 	printf("Final Cycle Count = %d\n",cycleCount);
 	printReg();

	return(0);

}