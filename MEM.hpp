#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Memory{
	public:
			int opcode, function, shamt, immediate, address; 
			int jumpAddress, pc, ra, rd, rt, rs, temp, index;
			bool check;
			int memory[1200];
			void doMem();
} mem;

void Memory::doMem(){
	check = false;
	switch(opcode){
		case 0x28: printf("	Executing: sb\n");
							rt = rt & 0b00000000000000000000000011111111;
							
							index = (rt + immediate) % 4;
							rs = floor((rs + immediate)/4);
							switch(index){
								case 0: 
									temp = memory[rs] & 0x00FFFFFF;
									rt = (rt << 6);
									temp = temp | rt;
									memory[rs] = temp;
									break;
								case 1:
									temp = memory[rs] & 0xFF00FFFF;
									rt = (rt << 4) & 0x00FF0000;
									temp = temp | rt;
									memory[rs] = temp;
									break;
								case 2:
									temp = memory[rs] & 0xFFFF00FF;
									rt = (rt << 2) & 0x0000FF00;
									temp = temp | rt;
									memory[rs] = temp;
									break;
								case 3:
									temp = memory[rs] & 0xFFFFFF00;
									rt = rt & 0x000000FF;
									temp = temp | rt;
									memory[rs] = temp;
									break;
							}
							printf("%d --> m[%d]\n",rt,memory[rs]);
							break;
		case 0x29: printf("	Executing: sh\n"); 
							rt = rt & 0b00000000000000001111111111111111;
							
							index = (rt + immediate) % 4;
							rs = floor((rs + immediate)/4);
							switch(index){
								case 0: 
									temp = memory[rs] & 0x0000FFFF;
									rt = (rt << 4);
									temp = temp | rt;
									memory[rs] = temp;
									break;
								case 1:
									temp = memory[rs] & 0xFFFF0000;
									rt = 0x00FF0000;
									temp = temp | rt;
									memory[rs] = temp;
									break;
							}
							printf("%d --> m[%d]\n",rt,memory[rs]);
							break;
		case 0x2b: printf("	Executing: sw\n"); 
							index = (rt + immediate) % 4;
							rs = floor((rs + immediate)/4);
							memory[rs+immediate] = rt;
							printf("%d --> m[%d]\n",rt,rs+immediate);
							break;
		case 0x24: printf("	Executing: lbu\n"); 
							check = true;
							switch(immediate){
								case 0: 
									temp = ((memory[rs/4] & 0xFF000000) >> 6);
									break;
								case 1:
									temp = ((memory[rs/4] & 0x00FF0000) >> 4);
									break;
								case 2:
									temp = ((memory[rs/4] & 0x0000FF00) >> 2);
									break;
								case 3:
									temp = (memory[rs/4] & 0x000000FF);
									break;
								default: printf("load offset problem\n");
							}
							printf("Reg #%d <-- m[%d]\n", rt, temp);
							break;
			case 0x25: printf("	Executing: lhu\n"); 
							check = true;
							switch(immediate){
								case 0: 
									temp = ((memory[rs/4] & 0xFFFF0000) >> 4);
									break;
								case 1:
									temp = (memory[rs/4] & 0x0000FFFF);
									break;
								default: printf("load offset problem\n");
							}
							printf("Reg #%d <-- m[%d]\n", rt, temp);
							break;
			case 0xf: printf("	Executing: lui\n"); 
							rt = immediate & 0b11111111111111110000000000000000;
							printf("m[%d] --> reg #%d \n",rs+immediate,rt);
							break;
			case 0x23: printf("	Executing: lw\n"); 
							rt = memory[(rs+immediate)/4];
							printf("m[%d] --> reg #%d \n",rs+immediate,rt);
							break;
	}
}