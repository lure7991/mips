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
#include "iCache.hpp"
#include "dCache.hpp"

#define EARLY_START true	 //Turn on/off early start
#define	ICACHE_ON	true 	//Turn on/off iCache
#define WRITE_POLICY	"WRITE_THROUGH"

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

bool memoryValid[1200];
int writeBuffer= 0;
int missPenalty=0;

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
	
	Memory mem;
	int fill;
	for(fill=0;fill<1200;fill++) mem.memory[fill] = IF.instruction[fill];

	//while(pc){
// 	int w;
// 	for(w=0;w<1771;w++){
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
	
// 	printf("Would you like to run another instruction? (1/0) --> ");
// 	scanf("%d", &answer);
// 	if(!answer){
// 		printf("Final Cycle Count = %d\n",cycleCount);
// 	}
	reg[0] = 0;
//}
	printf("Would you like to run another instruction? (1/0) --> ");
	scanf("%d", &answer);
	if(!answer){
		printf("Final Cycle Count = %d\n",cycleCount);
	}
}

 	//printMem();
 	printf("Final Cycle Count = %d\n",cycleCount);
 	printReg();


// //**** Start cache ****////
	iCache newCache;  
	int testAddress=pc;
	int tempPC=0;
	for(int ii=0; ii<32 ; ii++){
		newCache.address[ii]= (testAddress>>ii)&1;	//put PC values into address array 
	} 
	newCache.PC= pc; 
	newCache.parcePC_icache();
		//run twice for testing purposes
		// switch (newCache.access()){
		// 	case true:
		// 	cout<<"cacheHit!"<<endl;
		// 	cout<<"Data= "<< newCache.data<<endl; 

		// }
		if (newCache.access()){
			cout<<"cacheHit!"<<endl;
			cout<<"Data= "<< newCache.data<<endl; 
			//Give information to IF.foo= blah
		}
		else{
			missPenalty=0;
			//cache miss
			cout<<"Cache Miss! Accessing main memory"<<endl;
			tempPC= pc; //Change temp_PC to PC later
			tempPC= pc-newCache.offset;
			if (newCache.offset!=0){
				cycleCount= cycleCount+6;
			}
			//Filling cache
			for(int jj= 0; jj<newCache.blockSize; jj++){
				cout<<"PC= "<< tempPC+jj <<endl;
				printf("%x\n",IF.instruction[tempPC+jj]);
				newCache.idata[newCache.index][newCache.offset]= IF.instruction[tempPC+jj];
				missPenalty= missPenalty+2; 
			}
			cycleCount= cycleCount+missPenalty; 
			if(EARLY_START){
				cycleCount= cycleCount+missPenalty-(newCache.blockSize-1-newCache.offset);					
				}
			cout<<"cache misses= "<<newCache.numMisses<<endl;
			cout<<"cache hits= "<< newCache.numHits<<endl;
			cout<<"Cache hit rate= "<<newCache.hitRate<<endl;
		}
		//ii--;

	//*** Start dCache ***//
	dCache dcache;
	int dAddress= mem.rs;
	dcache.PC=mem.rs; 
	int tempAddress=0;
	for(int ii=0; ii<32 ; ii++){
		dcache.address[ii]= (tempAddress>>ii)&1;	//put PC values into address array 
	} 
	dcache.parcePC_dcache();
	tempAddress=0; 

	//Dcache hit
	if(dcache.access()){
			if(!dcache.write){
				reg[rt]= dcache.data;
			}
			else if(dcache.write){
				tempAddress= dAddress-dcache.offset;		
				for(int kk= 0; kk<dcache.blockSize; kk++){
					dcache.dData[dcache.index][dcache.offset]= mem.memory[tempAddress+kk];
				}
				if(WRITE_POLICY=="WRITE_THROUGH"){
					mem.memory[dAddress]= dcache.data;		//write from dCache to memory 
				}
				else if(WRITE_POLICY=="WRITE_BACK"){
					dcache.dirtyBit[dcache.index]= true;
				}
			}
	}

	else if(!dcache.access()){
		missPenalty=0; 
		if (dcache.offset!=0){
			cycleCount= cycleCount+6;
		}

		//Write through cache
		if (WRITE_POLICY=="WRITE_THROUGH"){
			//write to cache from memory
			missPenalty=0;
			tempAddress= dAddress-dcache.offset;		
			if(writeBuffer>0){
				cycleCount+=writeBuffer;
			}
			//fill cache
			for(int kk= 0; kk<dcache.blockSize; kk++){
				dcache.dData[dcache.index][dcache.offset]= mem.memory[tempAddress+kk];
				missPenalty= missPenalty+2;  	//6 cycle penalty for mem access
			}
				
			if(!dcache.write){
				//send information back into pipeline
				reg[rt]= dcache.data;
				cycleCount= cycleCount+missPenalty; 

			}
			if(dcache.write){
				writeBuffer=6; 
				mem.memory[dAddress]= dcache.dData[dcache.index][dcache.offset]; //Write through always writes to memory
				cycleCount= cycleCount+missPenalty+writeBuffer; 
			}
		}

		else if (WRITE_POLICY=="WRITE_BACK"){
			if(writeBuffer>0){
				cycleCount+=writeBuffer;
			}
			for(int kk= 0; kk<dcache.blockSize; kk++){
				dcache.dData[dcache.index][dcache.offset]= mem.memory[tempAddress+kk];
				missPenalty= missPenalty+2;  	//6 cycle penalty for mem access
			}
			if(dcache.dcacheValid[dcache.index]==1 && dcache.dirtyBit[dcache.index]==1){
				//discrepency b/t memory and cache, need to write to memory
				writeBuffer= 6+(dcache.blockSize-1)*2;		 
			}
			if(!dcache.write){
				// reg[value]= mem.memory[dcache.dData[dcache.index][dcache.offset]];
				cycleCount= cycleCount+missPenalty; 
			}
			if(dcache.write){
				dcache.dirtyBit[dcache.index]= 1;
				mem.memory[dAddress]= dcache.dData[dcache.index][dcache.offset]; 
			}
//write back to memory and then fill in cache
	return(0);
	}
	}
}