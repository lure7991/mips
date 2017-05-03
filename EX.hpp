#include <stdio.h>
#include <stdlib.h>

class Execute{
	public:
			int opcode, function, shamt, immediate, address, jumpAddress, pc, ra, rd, rt, rs;
			void executeR();
			void executeI();
			void special();
} ex;

void Execute::executeR(){ 
	switch(function){ 
		case 0x20: //printf("Executing: add\n");
						rd = rs + rt;
            break;
		case 0x21: //printf("Executing: addu\n"); 
						rd = rs + rt;          
						break;
		case 0x24: //printf("Executing: and\n");
            rd = rs & rt;
            break;
		case 0x08: //printf("Executing: jr\n");
            pc = rs;
						break;
		case 0x27: //printf("Executing: nor\n");
            rd = ~(rs | rt);
            break;
		case 0x25: //printf("Executing: or\n");
            rd = rs | rt;
            break;
		case 0x2a: //printf("Executing: slt\n");
            if(rs < rt) rd = 1;
						else if(rs > rt) rd = 0;
            break;
		case 0x02b: //printf("Executing: sltu\n");
						if(abs(rs) < abs(rt)) rd = 1;
						else if(abs(rs) > abs(rt)) rd = 0;
            break;
		case 0x00: //printf("Executing: sll\n");
            rd = rt << shamt;
            break;
		case 0x02: //printf("Executing: srl\n");
            rd = rt >> shamt;
            break;
		case 0x22: //printf("Executing: sub\n");
            rd = rs - rt;
            break;
		case 0x23: //printf("Executing: subu\n");
						rd = rs - rt;
            break;
		case 0xb: //printf("Executing: movn\n");
						if(rt) rd = rs;
            break;
		case 0xa: //printf("Executing: movz\n");
						if(!rt) rd = rs;
            break;
		case 0x26: //printf("Executing: xor\n");
						rd = rs ^ rt;
            break;
		//default: printf("Invalid R Command\n");
  }
}

int signExtend(int i){
	int signExtendBit = 0b1000000000000000 & i;
	if(signExtendBit){
		i = i | 0b11111111111111110000000000000000;
	}
	return i;
}

void Execute::executeI(){
	switch(opcode){
		case 0x8: //printf("Executing: addi\n");
						rt = rs + immediate;
            break;
		case 0x9: //printf("Executing: addiu\n"); 
						rt = rs + immediate;
            break;
		case 0xc: //printf("Executing: andi\n"); 
						rt = rs & immediate;
            break;
		case 0x4: //printf("Executing: beq\n");   
            break;
		case 0x5: //printf("Executing: bne\n"); 
            break;
		case 0x7: //printf("Executing: bgtz\n"); 
            break;
		case 0x6: //printf("Executing: blez\n"); 
            break;
		case 0x1: //printf("Executing: bltz\n"); 
            break;                            
		case 0x24: //printf("Executing: lbu\n"); 
            break;
		case 0x25: //printf("Executing: lhu\n"); 
            break;
		case 0x30: //printf("Executing: ll\n"); 
            break;
		case 0xf: //printf("Executing: lui\n"); 
            break;
		case 0x23: //printf("Executing: lw\n"); 
            break;
		case 0xd: //printf("Executing: ori\n"); 
						rt = rs | immediate;
            break;
		case 0xa: //printf("Executing: slti\n"); 
						if(rs < immediate) rt = 1;
						else rt = 0;
            break;
		case 0xb: //printf("Executing: sltiu\n"); 
						if(abs(rs) < abs(immediate)) rt = 1;
						else rt = 0;
            break;
		case 0x28: //printf("Executing: sb\n"); 
            break;
		case 0x38: //printf("Executing: sc\n"); 
            break;
		case 0x29: //printf("Executing: sh\n"); 
            break;
		case 0x2b: //printf("Executing: sw\n"); 
            break;
		case 0xe: //printf("Executing: xori");
						rt = rs ^ immediate;
						break;
		//default: printf("Invalid I Command\n");
  }
}

void Execute::special(){
	if(rt & 0b10000000) rd = 0b11111111111111111111111111111111;
	else if(!(rt & 0b10000000)) rd = 0b00000000000000000000000011111111;
}