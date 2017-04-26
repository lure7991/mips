#include <stdio.h>

class Decode{
    //int pc, opcode,rs,rt,rd,ra, shamt, funct, immediate,address, jumpAddress, input;
    public:
        int pc, opcode,rs,rt,rd,ra, shamt, funct, immediate,address, jumpAddress, input;
        void decodeR();
        void decodeI();
        void decodeJ();
				void setOP();
} id; 

void Decode::setOP(){
	 opcode = (input >> 26) & (0b00000000000000000000000000111111);
	 //printf("	Opcode: 0x%x\n",opcode);
}

void Decode::decodeR(){
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  rd = ((input >> 11) & 0b00000000000000000000000000011111);
  shamt = ((input >> 6) & 0b00000000000000000000000000011111);
  funct = (input & 0b00000000000000000000000000111111);   
}

void Decode::decodeI(){
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  immediate = (input & 0b00000000000000001111111111111111);
  if(immediate & 0b00000000000000001000000000000000){
    immediate = immediate & 0b00000000000000000111111111111111;
    immediate = immediate ^ 0b00000000000000000111111111111111;
    immediate = -1 * (immediate + 0b00000000000000000000000000000001);
  }
}

void Decode::decodeJ(){
  address = (input & 0b00000011111111111111111111111111);
	
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
}