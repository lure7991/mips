#include <stdio.h>

class Decode{
    // int opcode,rs,rt,rd, shamt, funct, immediate,address;
    public:
        int opcode,rs,rt,rd, shamt, funct, immediate,address, input;
        Decode ();
        void decodeR();
        void decodeI();
        void decodeJ();
} id; 

Decode::Decode(){
    // Add input as a parameter for later rev

    //int input = 0x014B4820; //R-Type: ADD $t1 $t2 $t3
    //int input = 0x21280007; //I-Type: addi t0 t1 0x0007
    int input = 0x3008FFFF; //I-Type: andi $t0,$zero,0xFFFF
    // input = 0x08000004; //J-Type: j 0x0004

    opcode = input >> 26;

}

void Decode::decodeR(){
  // printf("R-Type\n");
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  rd = ((input >> 11) & 0b00000000000000000000000000011111);
  shamt = ((input >> 6) & 0b00000000000000000000000000011111);
  funct = (input & 0b00000000000000000000000000111111);
  // printf("Rd = %d\n",rd);
  // printf("Rs = %d\n",rs);
  // printf("Rt = %d\n",rt);
  // printf("Shamt = %d\n",shamt);
  // printf("Function = %x\n",funct);
            
}

void Decode::decodeI(){
  // printf("I-Type\n");
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  immediate = (input & 0b00000000000000001111111111111111);
  // printf("Rs = %d\n",rs);
  // printf("Rt = %d\n",rt);
  // printf("Immediate = %d\n",immediate);
}

void Decode::decodeJ(){
  // printf("J-Type\n");
  address = (input & 0b00000011111111111111111111111111);
  // printf("Address = %x\n",address);
}