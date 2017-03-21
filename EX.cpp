#include <stdio.h>

int reg[32];

int opcode;
int function;
int rd;
int rt;
int rs;
int shamt;
int immediate;
int address;

void executeR(){
	switch(function){
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
}

void executeI(){
	
}

void executeJ(){

}

int main(){

	//fill the registers with their number for easy testing
	//normally the reg values are filled in from ID
	int i;
	for(i=0;i<32;i++)reg[i]=i;
	
	//example for R-Type: ADD $t1 $t2 $t3
	opcode = 0;
	function = 0x20;
	rd = reg[9];
	rt = reg[11];
	rs = reg[10];
	shamt = 0;
	immediate = 0;
	address = 0;
	
	//Other examples that can be used to test:
	  //R-Type: ADD $t1 $t2 $t3
    //I-Type: addi t0 t1 0x0007
    //I-Type: andi $t0,$zero,0xFFFF
    //J-Type: j 0x0004
	
  switch(opcode){
    case 0: executeR();
            break;
    case 0x2: executeJ();
            break;
    default: executeI();
  }
  return(0);
}