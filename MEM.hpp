#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class Memory{
	public:
			int opcode, function, shamt, immediate, address; 
			int jumpAddress, pc, ra, rd, rt, rs, temp, index;
			bool load, error;
			int memory[1200];
			void doMem();
} mem;

void Memory::doMem(){
	load = false;
	switch(opcode){
		case 0x28: //printf("	Executing: sb\n");
							rt = rt & 0b00000000000000000000000011111111;
							index = (rt + immediate) % 4;
							rs = floor((rs + immediate)/4);
							switch(index){
								case 0: 
									if(rs>=1200 || rs<0) printf("s1 accessing invalid memory\n");
									memory[rs] = (memory[rs] & 0x00FFFFFF) | ((rt << 24) & 0xFF000000);
// 									temp = memory[rs] & 0x00FFFFFF;
// 									rt = (rt << (6*4)) & 0xFF000000;
// 									temp = temp | rt;
// 									if(rs>=1200 || rs<0) printf("s1.2 accessing invalid memory\n");
// 									memory[rs] = temp;
									break;
								case 1:
									if(rs>=1200 || rs<0) printf("s2 accessing invalid memory\n");
										memory[rs] = (memory[rs] & 0xFF00FFFF) | ((rt << 16) & 0x00FF0000);
// 									temp = memory[rs] & 0xFF00FFFF;
// 									rt = (rt << (4*4)) & 0x00FF0000;
// 									temp = temp | rt;
// 									if(rs>=1200 || rs<0) printf("s2.2 accessing invalid memory\n");
// 									memory[rs] = temp;
									break;
								case 2:
									if(rs>=1200 || rs<0) printf("s3 accessing invalid memory\n");
										memory[rs] = (memory[rs] & 0xFFFF00FF) | ((rt << 8) & 0x0000FF00);
// 									temp = memory[rs] & 0xFFFF00FF;
// 									rt = (rt << (2*4)) & 0x0000FF00;
// 									temp = temp | rt;
// 									if(rs>=1200 || rs<0) printf("s3.2 accessing invalid memory\n");
// 									memory[rs] = temp;
// 									break;
								case 3:
									if(rs>=1200 || rs<0) printf("4 accessing invalid memory\n");
										memory[rs] = (memory[rs] & 0xFFFFFF00) | ((rt << 0) & 0x000000FF);
// 									temp = memory[rs] & 0xFFFFFF00;
// 									rt = rt & 0x000000FF;
// 									temp = temp | rt;
// 									if(rs>=1200 || rs<0) printf("4.2 accessing invalid memory\n");
// 									memory[rs] = temp;
									break;
								//default: printf("sb problem\n");
							}
							//printf("%d --> m #%d\n",memory[rs],rs);
							break;
		case 0x29: //printf("	Executing: sh\n");
							rt = rt & 0b00000000000000001111111111111111;
							index = (rt + immediate) % 4;
							rs = floor((rs + immediate)/4);
							switch(index){
								case 0: 
									if(rs>=1200 || rs<0) printf("5 accessing invalid memory\n");
									memory[rs] = (memory[rs] & 0x0000FFFF) | ((rt << 24) & 0xFFFF0000);
// 									temp = memory[rs] & 0x0000FFFF;
// 									rt = (rt << (4*4)) & 0xFFFF0000;
// 									temp = temp | rt;
// 									if(rs>=1200 || rs<0) printf("5.2 accessing invalid memory\n");
// 									memory[rs] = temp;
									break;
								case 1:
									if(rs>=1200 || rs<0) printf("6 accessing invalid memory\n");
										memory[rs] = (memory[rs] & 0xFFFF0000) | ((rt << 8) & 0x0000FFFF);
// 									temp = memory[rs] & 0xFFFF0000;
// 									rt = rt & 0x0000FFFF;
// 									temp = temp | rt;
// 									if(rs>=1200 || rs<0) printf("6.2 accessing invalid memory\n");
// 									memory[rs] = temp;
									break;
								//default: printf("sh problem\n");
							}
							//printf("%d --> m #%d\n",memory[rs],rs);
							break;
		case 0x2b: //printf("	Executing: sw\n");
							index = (rs + immediate) % 4;
							rs = floor((rs + immediate)/4);
							if(rs+index>=1200 || rs+index<0) printf("7 accessing invalid memory\n");
							memory[rs+index] = rt;
							//printf("%d --> m #%d\n",rt,rs+index);
							break;
		case 0x24: //printf("	Executing: lbu\n");
							load = true;
							switch(immediate){
								if((rs/4)>=1200 || rs/4<0) printf("accessing invalid memory\n");
								case 0: 
									rt = (rt & 0x00FFFFFF) | ((memory[rs] << 24) & 0xFF000000);
									//temp = ((memory[rs/4] & 0xFF000000) >> (6*4));
									break;
								case 1: 
									rt = (rt & 0xFF00FFFF) | ((memory[rs] << 24) & 0x00FF0000);
									//temp = ((memory[rs/4] & 0x00FF0000) >> (4*4));
									break;
								case 2: 
									rt = (rt & 0xFFFF00FF) | ((memory[rs] << 24) & 0x0000FF00);
									//temp = ((memory[rs/4] & 0x0000FF00) >> (4*2));
									break;
								case 3: 
									rt = (rt & 0xFFFFFF00) | ((memory[rs] << 24) & 0x000000FF);
									//temp = (memory[rs/4] & 0x000000FF);
									break;
								//default: printf("lbu problem\n");
							}
							//printf("rt = %d \n",rt);
							break;
			case 0x25: //printf("	Executing: lhu\n");
							load = true;
							if((rs/4)>=1200 || rs/4<0) printf("accessing invalid memory\n");
							switch(immediate){
								case 0: 
									rt = (rt & 0x0000FFFF) | ((memory[rs] << 24) & 0xFFFF0000);
									//temp = ((memory[rs/4] & 0xFFFF0000) >> (4*4));
									break;
								case 1:
									rt = (rt & 0xFFFF0000) | ((memory[rs] << 24) & 0x0000FFFF);
									//temp = (memory[rs/4] & 0x0000FFFF);
									break;
								//default: printf("lhu problem\n");
							}
							//printf("rt = %d \n",rt);
							break;
			case 0xf: //printf("	Executing: lui\n");
							load = true;
							rt = immediate & 0b11111111111111110000000000000000;
							printf("rt = %d \n",rt);
							break;
			case 0x23: //printf("	Executing: lw\n");
							load = true;
							if(((rs+immediate)/4)>=1200 || (rs+immediate)/4<0) printf("accessing invalid memory\n");
							rt = memory[(rs+immediate)/4];
							//printf("rt = %d \n",rt);
							break;
	}
}