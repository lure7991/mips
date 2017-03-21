#include <stdio.h>

//fill register with numbers to be used in testing, normally not needed
int reg[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

//initalize variables
int opcode;
int function;
int shamt;
int immediate;
int address;
int pc;	
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

void executeI(){
	
}

void executeJ(){

}

int main(){
	//put exmaple values in here:  ///////////////////////////////
	//normally these would be passed in from previous pipelines///
	
	//example for R-Type: ADD $t1 $t2 $t3
	opcode = 0;
	function = 0x20;
	shamt = 0;
	//immediate;
	//address;
	//pc;	
	int rd_reg = 9;
	int rt_reg = 10;
	int rs_reg = 11;
	
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