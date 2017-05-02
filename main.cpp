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

using namespace std;

int cycleCount = 0;

int reg[32];

int instruction[500];
int instructionIndex = 0;

int pc;
int sp = 29;
int fp = 30;
int ra = 31;

int opcode;
int rd;
int rs;
int rt;
int shamt;
int funct;
int address;
int immediate;

void printReg(){
	printf("Reg values after pipeline: \n");
	int i;
	for(i=0; i<32; i++){
	printf("	Reg #%d = %d\n",i,reg[i]);
}
}

void printMem(){
	printf("Mem values after pipeline: \n");
	int i;
	for(i=0; i<1200; i++){
	printf("Mem #%d = %d	",i,mem.memory[i]);
}
}

int main(){

//Instruction Fetch Pipeline//////////////////////////////////////////////////////////////
//printf("\nInstruction Fetch:\n");
	Fetch IF;

	IF.fill();

	//instructionIndex = IF.instructionIndex;

	reg[sp] = IF.sp;
	reg[fp] = IF.fp;
	pc = IF.pc;

	printf("	sp = 0x%x\n",reg[sp]);
	printf("	fp = 0x%x\n",reg[fp]);
	printf("	pc = %d\n",pc);

	//int answer = 1;
	pc--;
	//while(answer){
	while(pc){
		//printf("\nInstruction Fetch:\n\n");
		pc++;
		printf("\nInstruction: 0x%x\n",IF.instruction[pc]);
		printf("pc = %d\n",pc+1);


//Decode Pipeline//////////////////////////////////////////////////////////////////////////
//printf("\nDecode:\n\n");
		Decode id;

		id.pc = pc;
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
				pc = id.pc;

				printf("\nRecieved R-Type: \n");
				printf("	 Rd = %d\n",reg[rd]);
				printf("	 Rs = %d\n",reg[rs]);
				printf("	 Rt = %d\n",reg[rt]);
				printf("	 Shamt = %d\n",shamt);
				printf("	 Function = 0x%x\n",funct); 
				break;
			case 0x2: 
				id.decodeJ();
				pc = id.pc;
				reg[ra] = id.ra;
				address = id.address;

				printf("\nRecieved J-Type: \n");        
				printf("	 pc = %d\n",pc+1);
				printf("	 ra = 0x%x\n",reg[ra]);
				printf("	 Address = 0x%x\n",address);    
				break;
			case 0x3: 
				id.decodeJ();
				pc = id.pc;
				reg[ra] = id.ra;
				address = id.address;

				printf("\nRecieved J-Type: \n");        
				printf("	 pc = %d\n",pc+1);
				printf("	 ra = 0x%x\n",reg[ra]);
				printf("	 Address = 0x%x\n",address);    
				break;
			case 0x1f:
				id.special();
				rd = id.rd;
				printf("\nRecieved Special Type: seb\n");
				printf("	 Rd = %d\n",reg[rd]);	
			default: 
				id.decodeI();
				rs = id.rs;
				rt = id.rt;
				pc = id.pc;
				immediate = id.immediate;

				printf("\nRecieved I-Type: \n");
				printf("	 pc = %d\n",pc+1);
				printf("	 Rs = %d\n",reg[rs]);
				printf("	 Rt = %d\n",reg[rt]);
				printf("	 Immediate = %d\n",immediate);
				break;
	}

//Execute Pipeline//////////////////////////////////////////////////////////////////////////
//printf("Execute:\n\n");
	Execute ex;
	ex.opcode = opcode;
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
			reg[rd] = ex.rd;
			printf("Rd = %d to Reg #%d\n",reg[rd], rd);
			break;
		case 0x2: //j-type
			break;
		case 0x1f: 
			ex.special();
			reg[rt] = ex.rt;
			printf("Rt = %d to Reg #%d\n",reg[rt], rt);
		default: 
			ex.executeI();
			pc = ex.pc;
			reg[rt] = ex.rt;
			printf("Rt = %d to Reg #%d\n",reg[rt], rt);
	}

//Memory Pipeline//////////////////////////////////////////////////////////////////////////
//printf("Memory:\n\n");

	Memory mem;
	mem.opcode = opcode;
	mem.rd = reg[rd];
	mem.rt = reg[rt];
	mem.rs = reg[rs];
	mem.address = address;
	mem.shamt = shamt;
	mem.function = funct;
	mem.pc = pc;

	mem.doMem();

	if(mem.load){
		reg[mem.rt] = mem.temp;
	}
	else if (!mem.load){  
		reg[rt] = mem.rt;
	}
	
// Write-Back Pipeline//////////////////////////////////////////////////////////////////////////

	printReg();

////////////////////////////////////END OF PIPELINE/////////////////////////////////////////////////////////////
	cycleCount++;
	
	printf("Current Cycle Count = %d\n",cycleCount);
	
	printf("pc = %d\n",pc+1);
	
// 	printf("Would you like to run another instruction? (1/0) --> ");
// 	scanf("%d", &answer);
// 	if(!answer){
// 		printf("Final Cycle Count = %d\n",cycleCount);
// 		//return(0);
// 	}
	}

	printMem();
	printf("Final Cycle Count = %d\n",cycleCount);
	printReg();
	
	
// //**** Start cache bullshit ****////
// 	iCache newCache; 
// 	newCache.cacheSize= 128;
// 	// newCache.address= 0b11100011100011100011100011100011
// 	// newCache.address= 0b00000000000000000000000000000110;
// 	//index= 101010101010
// 	//tag= 11100000000000001010

// 	// int addressArray[2]= {0b11100000000000001010101010101010, 0b00000000000000000000000000000110};
// 	// int temp_PC= 0b11100011100011100011100011100010;
	 
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
// 		tempPC= temp_PC-offset;
// 		// for(int j= 0; j<newCache.blockSize; j++){
// 		// 	// newCache.idata[newCache.index][]= memory[PC];
// 		for(int j= 0; j<newCache.blockSize; j++){
// 			//instruction[PC]
// 			newCache.idata[newCache.index][newCache.offset]= memory[tempPC+j];
// 			newCache.cacheValid[newCache.index]= true;
// 		}
			

// 		// }
// 		// newCache.idata[newCache.index][newCache.blocksize]= memory[PC]
// 		cout<<"cache misses= "<<newCache.numMisses<<endl;
// 		cout<<"cache hits= "<< newCache.numHits<<endl;
// 	}
// 	i--;

	return(0);

}