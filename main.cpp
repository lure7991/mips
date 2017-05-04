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
//#include "iCache.hpp"

//#define EARLY_START true; //Turn on/off early start
//#define	ICACHE_ON	true; //Turn on/off iCache

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
		if(mem.memory[i]) printf("Mem #%d = %d	",i,mem.memory[i]);
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
	printf("	fp = %d\n",reg[fp]);
	printf("	pc = %d\n",pc);

	//int answer = 1;
	//while(answer){
	//while(pc){
	int w;
	for(w=0;w<1000;w++){
		if(pc>=500){
			printf("pc beyond 500\n");
			printf("	pc = %d\n",pc);
			return 0;
		}
		pc++;
		if(pc == branchOn) pc = bjPC;
 		printf("\nInstruction: 0x%x\n",IF.instruction[pc]);
		printf("pc = %d\n",pc);
		printf("sp = %d\n",reg[sp]);

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
				rd = id.rd;
				rs = id.rs;
				rt = id.rt;
				shamt = id.shamt;
				funct = id.funct;
				break;
			case 0x2: 
				id.decodeJ();
				if(ra>31 || ra<0) printf("1: trying to access out of bounds reg\n");
				reg[ra] = id.ra;
				address = id.address;   
				break;
			case 0x3: 
				id.decodeJ();
				if(ra>31 || ra<0) printf("2: trying to access out of bounds reg\n");
				reg[ra] = id.ra;
				address = id.address;  
				break;
			case 0x1f:
				id.special();
				rd = id.rd;
			default: 
				id.decodeI();
				rs = id.rs;
				rt = id.rt;
				immediate = id.immediate;
				break;
		}
		
		bj = id.bj;
		if(bj){
			bjPC = id.pc;
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

	Memory mem;
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
	
// 	printf("Would you like to run another instruction? (1/0) --> ");
// 	scanf("%d", &answer);
// 	if(!answer){
// 		printf("Final Cycle Count = %d\n",cycleCount);
// 	}
	reg[0] = 0;
}

 	printMem();
 	printf("Final Cycle Count = %d\n",cycleCount);
 	printReg();
	
	
// // //**** Start cache bullshit ****////
// // 	iCache newCache; 
// // 	newCache.cacheSize= 128;
// // 	// newCache.address= 0b11100011100011100011100011100011
// // 	// newCache.address= 0b00000000000000000000000000000110;
// // 	//index= 101010101010
// // 	//tag= 11100000000000001010

// // 	// int addressArray[2]= {0b11100000000000001010101010101010, 0b00000000000000000000000000000110};
// // 	// int temp_PC= 0b11100011100011100011100011100010;
	 
// <<<<<<< HEAD
// 	int testAddress= pc;
// 	for(int i=0; i<32 ; i++){
// 		newCache.address[i]= (testAddress>>i)&1;	//Eventually change testAddress to PC value
// 	} 

// 	//*** Begin the cache simulation ** ///
// 	//*** IF checks iCache for instruction using PC val ***//
// 	//*** If there's a miss, the cache goes to memory[PC] and writes instruction to cache ***//
// 	cout<<"Testing with PC val of 0x16"<<endl;
// 	newCache.PC= pc; 
// 	newCache.parcePC();

// 	// newCache.idata[newCache.index][newCache.] 
// 	// newCache.iCache();
// 	int i=2;
// 	while(i>0){
// 		//run twice for testing purposes
// 	if (newCache.access()){
// 		cout<<"cacheHit!"<<endl;
// 		cout<<"Data= "<< newCache.data<<endl; 
// 	}
// 	else{
// 		//cache miss
// 		cout<<"Cache Miss! Accessing main memory"<<endl;

// 		//if cache miss, fill blocks with blocksize amount of memory lines

// 		tempPC= pc; //Change temp_PC to PC later
// 		tempPC= pc-offset;
// 		if (offset!=0){
// 			cycleCount= cycleCount+6;
// 		}
// 		//Filling cache
// 		for(int j= 0; j<newCache.blockSize; j++){
// 			newCache.idata[newCache.index][newCache.offset]= instruction[tempPC+j];
// 			cycleCount= cycleCount+2; 
// 		}
			
// 		cout<<"cache misses= "<<newCache.numMisses<<endl;
// 		cout<<"cache hits= "<< newCache.numHits<<endl;
// 	}
// 	i--;

	return(0);

}