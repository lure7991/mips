#include <stdio.h>
#include <inttypes.h>

class Decode{
    // int opcode,rs,rt,rd, shamt, funct, immediate,address;
    public:
        int opcode,rs,rt,rd, shamt, immediate, funct,address, input;
        Decode ();
        void decodeR();
        void decodeI();
        void decodeJ();
} id; 

Decode::Decode(){
    // Add input as a parameter for later rev

    //int input = 0x014B4820; //R-Type: ADD $t1 $t2 $t3
    //int input = 0x21280007; //I-Type: addi t0 t1 0x0007
    //int input = 0x3008FFFF; //I-Type: andi $t0,$zero,0xFFFF
    input = 0x2149FFAF; //I-Type: andi t1 t2 0xFFFC with 0xfffc = -4 decimal
    //int input = 0x08000004; //J-Type: j 0x0004
    
    opcode = input >> 26;

}

void Decode::decodeR(){
  //input = 0x014B4820;
  printf("R-Type\n");
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  rd = ((input >> 11) & 0b00000000000000000000000000011111);
  shamt = ((input >> 6) & 0b00000000000000000000000000011111);
  funct = (input & 0b00000000000000000000000000111111);
  printf("Rd = %d\n",rd);
  printf("Rs = %d\n",rs);
  printf("Rt = %d\n",rt);
  printf("Shamt = %d\n",shamt);
  printf("Function = %X\n",funct);
            
}

void Decode::decodeI(){
  //input = 0x21497FFF;
  printf("I-Type\n");
  rs = ((input >> 21) & 0b00000000000000000000000000011111);
  rt = ((input >> 16) & 0b00000000000000000000000000011111);
  immediate = (input & 0b00000000000000001111111111111111);
  if(immediate & 0b00000000000000001000000000000000){
    immediate = immediate & 0b00000000000000000111111111111111;
    immediate = immediate ^ 0b00000000000000000111111111111111;
    immediate = -1 * (immediate + 0b00000000000000000000000000000001);
  } 
  printf("Rs = %d\n",rs);
  printf("Rt = %d\n",rt);
  printf("Immediate = %d\n",immediate);
}

void Decode::decodeJ(){
  printf("J-Type\n");
  address = (input & 0b00000011111111111111111111111111);
  printf("Address = %X\n",address);
}

int main(){
  Decode id;
  printf("Op Code = %X -> ",id.opcode);
  switch(id.opcode){
    case 0: id.decodeR();
            break;
    case 0x2: id.decodeJ();
            break;
    default: id.decodeI();
  }
  return(0);
}