#include <stdio.h>

class Decode{
    public:
        int pc, opcode,rs,rt,rd,ra, shamt, funct, immediate,address, jumpAddress, input;
				bool bj;
				void decodeR();
        void decodeI();
        void decodeJ();
				void setOP();
				void special();
} id; 

int branchAddress(int i){
	int branchAddress = 0;
	int signExtendBit = 0b1000000000000000 & i;
	if(signExtendBit){
		branchAddress = i << 2;
		branchAddress = branchAddress | 0b11111111111111000000000000000000;
	}
	else if(!signExtendBit){
		branchAddress = i << 2;
	}
	//printf("branchAddress+pc+1 = %d\n", branchAddress);
	return branchAddress;
}

void Decode::setOP(){
	 opcode = (input >> 26) & (0b00000000000000000000000000111111);
}

void Decode::decodeR(){
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  rd = ((input >> 11) & 0b00000000000000000000000000011111);
  shamt = ((input >> 6) & 0b00000000000000000000000000011111);
  funct = (input & 0b00000000000000000000000000111111);   
}

void Decode::decodeI(){
	bj = false;
	
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  immediate = (input & 0b00000000000000001111111111111111);
  if(immediate & 0b00000000000000001000000000000000){
    immediate = immediate & 0b00000000000000000111111111111111;
    immediate = immediate ^ 0b00000000000000000111111111111111;
    immediate = -1 * (immediate + 0b00000000000000000000000000000001);
  }
	
	switch(opcode){
		case 0x4: //printf("	Executing: beq\n");   
							if(rs==rt){
								bj = true;
								pc = pc + immediate;
							} 
							break;
			case 0x5: //printf("	Executing: bne\n"); 
							if(rs!=rt){
								bj = true;
								pc = pc + immediate;
							} 
							break;
			case 0x7: //printf("	Executing: bgtz\n"); 
							if(rs>0){
								bj = true;
								pc = pc + immediate;
							} 
							break;
			case 0x6: //printf("	Executing: blez\n"); 
							if(rs<=0){
								bj = true;
								pc = pc + immediate;
							} 
							break;
			case 0x1: //printf("	Executing: bltz\n"); 
							if(rs<0){
								bj = true;
								pc = pc + immediate;
							} 
							break;                            
	}
}

void Decode::decodeJ(){
	bj = false;
  address = (input & 0b00000011111111111111111111111111);
	pc += 1;
	jumpAddress = (pc & 0b11110000000000000000000000000000)|(0b00000001111111111111111111111111 & address);
	//jumpAddress = jumpAddress << 2;
	//printf("Jump Address = %d\n",jumpAddress);
	
	switch(opcode){
		case 0x2: //printf("\nExecuting: Jump\n");
						bj = true;
						pc = jumpAddress;
            break;
		case 0x3: //printf("Executing: Jump and Link\n");
						bj = true; 
						ra = pc + 2;
						pc = jumpAddress;
						break;
		//default: printf("Invalid Command\n");
  }   
}

void Decode::special(){
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  rd = ((input >> 11) & 0b00000000000000000000000000011111);
}