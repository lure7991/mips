#include <stdio.h>
#include <stdlib.h>

//fill register with numbers to be used in testing, normally not needed
int reg[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

//initalize variables
int opcode;
int function;
int shamt;
int immediate;
int address;
int jumpAddress;
int pc;	
int ra;
int rd;
int rt;
int rs;

void executeR(){ //function to check for specific r-type command and execute
	printf("\nStarting Values:\n");
	printf("PC = %X\n",pc);
	printf("Function = %X\n",function);
  printf("Shamt = %d\n",shamt);
	printf("Rd = %d\n",rd);
  printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);

	switch(function){ //choose command based on r-type function
		case 0x20: printf("Executing: add\n");
						rd = rs + rt;
						printf("rd = %d\n",rd);
            break;
		case 0x21: printf("Executing: addu\n"); 
            break;
		case 0x24: printf("Executing: and\n");
            rd = rs & rt;
						printf("rd = %d\n",rd);
            break;
		case 0x08: printf("Executing: jr\n");
            pc = rs;
						break;
		case 0x27: printf("Executing: nor\n");
            rd = ~(rs | rt);
						printf("rd = %d\n",rd);
            break;
		case 0x25: printf("Executing: or\n");
            rd = rs | rt;
						printf("rd = %d\n",rd);
            break;
		case 0x2a: printf("Executing: slt\n");
            if(rs < rt) rd = 1;
						else if(rs > rt) rd = 0;
						printf("rd = %d\n",rd);
            break;
		case 0x02b: printf("Executing: sltu\n");
            break;
		case 0x00: printf("Executing: sll\n");
            rd = rt << shamt;
						printf("rd = %d\n",rd);
            break;
		case 0x02: printf("Executing: srl\n");
            rd = rt >> shamt;
						printf("rd = %d\n",rd);
            break;
		case 0x22: printf("Executing: sub\n");
            rd = rs - rt;
						printf("rd = %d\n",rd);
            break;
		case 0x23: printf("Executing: subu\n");
            break;
  }   
	printf("\nEnding Values:\n");
	printf("PC = %X\n",pc);
	printf("Function = %X\n",function);
  printf("Shamt = %d\n",shamt);
	printf("Rd = %d\n",rd);
  printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);
}

int branchAddress(int i){
	int branchAddress = 0;
	int signExtendBit = 0b1000000000000000 & i;
	if(signExtendBit){
		branchAddress = i << 2;
		branchAddress = branchAddress | 0b11111111111111000000000000000000;
	}
	else if(!signExtendBit){
		branchAddress = i << 2;
		//i think the computer keeps numbers 0 filled
		//so if the sign extend bit is 0, just leave the stuff how it is
	}
	return branchAddress;
}

int signExtend(int i){
	int signExtendBit = 0b1000000000000000 & i;
	if(signExtendBit){
		i = i | 0b11111111111111110000000000000000;
	}
	return i;
}

void executeI(){
	printf("\nStarting Values:\n");
	printf("PC = %X\n",pc);
  printf("Rt = %d\n",immediate);
	printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);

	switch(opcode){ //choose command based on r-type function
		case 0x8: printf("Executing: addi\n");
						rt = rs + immediate;
						printf("rt = %d\n",rd);
            break;
		case 0x9: printf("Executing: addiu\n"); 
						rt = rs + abs(immediate);
						printf("rt = %d\n",rd);
            break;
		case 0xc: printf("Executing: andi\n"); 
						rt = rs & immediate;
						printf("rt = %d\n",rd);
            break;
		case 0x4: printf("Executing: beq\n"); 
						if(rs==rt){
							printf("Branch Taken\n");
							pc = pc + 1 + branchAddress(immediate);
						} 
            break;
		case 0x5: printf("Executing: bne\n"); 
						if(rs!=rt){
							printf("Branch Taken\n");
							pc = pc + 1 + branchAddress(immediate);
						} 
            break;
		case 0x24: printf("Executing: lbu\n"); 
						//uh oh
            break;
		case 0x25: printf("Executing: lhu\n"); 
						//uh oh
            break;
		case 0x30: printf("Executing: ll\n"); 
						//uh oh
            break;
		case 0xf: printf("Executing: lui\n"); 
						//uh oh
            break;
		case 0x23: printf("Executing: lw\n"); 
						//uh oh
            break;
		case 0xd: printf("Executing: ori\n"); 
						rt = rs | immediate;
						printf("rt = %d\n",rd);
            break;
		case 0xa: printf("Executing: slti\n"); 
						if(rs < immediate) rt = 1;
						else rt = 0;
						printf("rt = %d\n",rd);
            break;
		case 0xb: printf("Executing: sltiu\n"); 
						if(abs(rs) < abs(immediate)) rt = 1;
						else rt = 0;
						printf("rt = %d\n",rd);
            break;
		case 0x28: printf("Executing: sb\n"); 
						//uh oh
            break;
		case 0x38: printf("Executing: sc\n"); 
						//uh oh
            break;
		case 0x29: printf("Executing: sh\n"); 
						//uh oh
            break;
		case 0x2b: printf("Executing: sw\n"); 
						//uh oh
            break;
  }   
	printf("\nEnding Values:\n");
	printf("PC = %X\n",pc);
  printf("Rt = %d\n",immediate);
	printf("Rt = %d\n",rt);
  printf("Rs = %d\n\n",rs);
}

void executeJ(){
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

int main(){
	//put exmaple values in here:  ///////////////////////////////
	//normally these would be passed in from previous pipelines///
	
	int rd_reg = 0;
	int rs_reg = 0;
	int rt_reg = 0;
	
// 	example for R-Type: ADD $t1 $t2 $t3
// 	opcode = 0;
// 	function = 0x20;
// 	shamt = 0;
//  rd_reg = 9;
//  rt_reg = 10;
// 	rs_reg = 11;
// 	//immediate;
// 	//address;
// 	//pc;
	
//	example for J-Type: J 0x0200811 
// 	pc = 0x80aa00ef;
// 	opcode = 0x2;
// 	address = 0x200811;
	
//	example for I-Type: addi s0 t1 0xffff
	opcode = 0b001000;
	immediate = 0xffff;
	rt = 0b01001;
	rs = 0b10000;
	
	//////////////////////////////////////////////////////////////
	
	//start by taking in reg values
	rd = reg[rd_reg]; 
	rt = reg[rt_reg];
	rs = reg[rs_reg];
	
  switch(opcode){ //select format
    case 0: executeR();
						reg[rd_reg] = rd; //save the new rd back to the reg
						//if command is jr, note that pc is changed in the executeR() 
            break;
    case 0x2: executeJ();
            break;
    default: executeI();
  }
	
  return(0);
}