#include <stdio.h>
#include <stdlib.h>

class Execute{
	public:
			//initalize variables
			int opcode, function, shamt, immediate, address, jumpAddress, pc, ra, rd, rt, rs;
			void executeR();
			void executeI();
			void executeJ();
			void special();
} ex;

void Execute::executeR(){ //function to check for specific r-type command and execute
/*
	printf("EX Starting Values:\n");
	printf("PC = %X\n",pc);
	printf("Function = %X\n",function);
  printf("Shamt = %d\n",shamt);
	printf("Rd = %d\n",rd);
  printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);
*/
	switch(function){ //choose command based on r-type function
		case 0x20: printf("	Executing: add\n");
						rd = rs + rt;
						//printf("rd = %d\n",rd);
            break;
		case 0x21: printf("	Executing: addu\n"); 
	  				rd = abs(rs) + abs(rt);          
						break;
		case 0x24: printf("	Executing: and\n");
            rd = rs & rt;
						//printf("rd = %d\n",rd);
            break;
		case 0x08: printf("	Executing: jr\n");
            pc = rs;
						break;
		case 0x27: printf("	Executing: nor\n");
            rd = ~(rs | rt);
						//printf("rd = %d\n",rd);
            break;
		case 0x25: printf("	Executing: or\n");
            rd = rs | rt;
						//printf("rd = %d\n",rd);
            break;
		case 0x2a: printf("	Executing: slt\n");
            if(rs < rt) rd = 1;
						else if(rs > rt) rd = 0;
						//printf("rd = %d\n",rd);
            break;
		case 0x02b: printf("	Executing: sltu\n");
						if(abs(rs) < abs(rt)) rd = 1;
						else if(abs(rs) > abs(rt)) rd = 0;
            break;
		case 0x00: printf("	Executing: sll\n");
            rd = rt << shamt;
						//printf("rd = %d\n",rd);
            break;
		case 0x02: printf("	Executing: srl\n");
            rd = rt >> shamt;
						//printf("rd = %d\n",rd);
            break;
		case 0x22: printf("	Executing: sub\n");
            rd = rs - rt;
						//printf("rd = %d\n",rd);
            break;
		case 0x23: printf("	Executing: subu\n");
						rd = abs(rs) - (rt);
            break;
		case 0xb: printf("	Executing: movn\n");
						if(rt) rd = rs;
            break;
		case 0xa: printf("	Executing: movz\n");
						if(!rt) rd = rs;
            break;
		case 0x26: printf("	Executing: xor\n");
						rd = rs ^ rt;
            break;
		default: printf("	Invalid Command\n");
  }
/*
	printf("\nEnding Values:\n");
	printf("PC = %X\n",pc);
	printf("Function = %X\n",function);
  printf("Shamt = %d\n",shamt);
	printf("Rd = %d\n",rd);
  printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);
*/
}

int signExtend(int i){
	int signExtendBit = 0b1000000000000000 & i;
	if(signExtendBit){
		i = i | 0b11111111111111110000000000000000;
	}
	return i;
}

void Execute::executeI(){
/*
	printf("\nStarting Values:\n");
	printf("PC = %X\n",pc);
  printf("Rt = %d\n",immediate);
	printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);
*/
	switch(opcode){ //choose command based on r-type function
		case 0x8: printf("	Executing: addi\n");
						rt = rs + immediate;
						//printf("rt = %d\n",rd);
            break;
		case 0x9: printf("	Executing: addiu\n"); 
						rt = rs + abs(immediate);
						//printf("rt = %d\n",rd);
            break;
		case 0xc: printf("	Executing: andi\n"); 
						rt = rs & immediate;
						//printf("rt = %d\n",rd);
            break;
		case 0x4: printf("	Executing: beq\n");   //-----------------------------------------------
// 						if(rs==rt){
// 							printf("	Branch Taken\n");
// 							pc = pc + 1 + branchAddress(immediate);
// 						} 
// 						else printf("	Branch NOT Taken\n");
            break;
		case 0x5: printf("	Executing: bne\n"); 
// 						if(rs!=rt){
// 							printf("	Branch Taken\n");
// 							pc = pc + 1 + branchAddress(immediate);
// 						} 
// 						else printf("	Branch NOT Taken\n");
            break;
		case 0x7: printf("	Executing: bgtz\n"); 
// 						if(rs>0){
// 							printf("	Branch Taken\n");
// 							pc = pc + 1 + immediate;
// 						} 
// 						else printf("	Branch NOT Taken\n");
            break;
		case 0x6: printf("	Executing: blez\n"); 
// 						if(rs<=0){
// 							printf("	Branch Taken\n");
// 							pc = pc + 1 + immediate;
// 						} 
// 						else printf("	Branch NOT Taken\n");
            break;
		case 0x1: printf("	Executing: bltz\n"); 
// 						if(rs<0){
// 							printf("	Branch Taken\n");
// 							pc = pc + 1 + immediate;
// 						} 
// 						else printf("	Branch NOT Taken\n");
            break;                            //------------------------------------------------
		case 0x24: printf("	Executing: lbu\n"); 
						//uh oh
            break;
		case 0x25: printf("	Executing: lhu\n"); 
						//uh oh
            break;
		case 0x30: printf("	Executing: ll\n"); 
						//uh oh
            break;
		case 0xf: printf("	Executing: lui\n"); 
						//uh oh
            break;
		case 0x23: printf("	Executing: lw\n"); 
						//uh oh
            break;
		case 0xd: printf("	Executing: ori\n"); 
						rt = rs | immediate;
						//printf("rt = %d\n",rd);
            break;
		case 0xa: printf("	Executing: slti\n"); 
						if(rs < immediate) rt = 1;
						else rt = 0;
						//printf("rt = %d\n",rd);
            break;
		case 0xb: printf("	Executing: sltiu\n"); 
						if(abs(rs) < abs(immediate)) rt = 1;
						else rt = 0;
						//printf("rt = %d\n",rd);
            break;
		case 0x28: printf("	Executing: sb\n"); 
						//uh oh
            break;
		case 0x38: printf("	Executing: sc\n"); 
						//uh oh
            break;
		case 0x29: printf("	Executing: sh\n"); 
						//uh oh
            break;
		case 0x2b: printf("	Executing: sw\n"); 
						//uh oh
            break;
		case 0xe: printf(" Executing: xori");
						rt = rs ^ immediate;
						break;
		default: printf("	Invalid Command\n");
  }
/*
	printf("\nEnding Values:\n");
	printf("PC = %X\n",pc);
  printf("Rt = %d\n",immediate);
	printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);
*/
}

void Execute::executeJ(){
	/* The following commented code was moved to ID pipeline
	printf("\nStarting Values:\n");
	printf("PC = %X\n",pc);
	printf("ra = %X\n",ra);
	printf("Adress = %X\n",address);
	
	pc += 1;
	jumpAddress = (pc & 0b11110000000000000000000000000000)|(address << 2);
	
	switch(opcode){ //choose command based on r-type function
		case 0x2: printf("\nExecuting: Jump\n");
						pc = jumpAddress;
            break;
		case 0x3: printf("Executing: Jump and Link\n"); 
            ra = pc + 2;
						pc = jumpAddress;
						break;
  }   
	
	printf("\nEnding Values:\n");
	printf("PC = %X\n",pc);
	printf("ra = %X\n",ra);
	printf("Jump Adress = %X\n",jumpAddress);
	*/
}

void Execute::special(){
	if(rt & 0b10000000) rd = 0b11111111111111111111111111111111;
	else if(!(rt & 0b10000000)) rd = 0b00000000000000000000000011111111;
}