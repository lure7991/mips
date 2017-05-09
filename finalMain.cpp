#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include "IF.hpp"
#include "ID.hpp"
#include "EX.hpp"
#include "MEM.hpp"

using namespace std;

int opcode;
int rd;
int rs;
int rt;
int shamt;
int funct;
int address;
int immediate;

int main(){

//Decode Pipeline//////////////////////////////////////////////////////////////////////////
//printf("\nDecode:\n\n");
		Decode id;
		id.input = 0x014B4820;

		id.setOP();
		opcode = id.opcode;

		switch(id.opcode){
			case 0: 
				id.decodeR();
				printf("R-Type\n");
				rd = id.rd;
				rs = id.rs;
				rt = id.rt;
				shamt = id.shamt;
				funct = id.funct;
        printf("opcode = %d\n",opcode);
				printf("rd = %d\n",rd);
				printf("rs = %d\n",rs);
				printf("rt = %d\n",rt);
				printf("shift amount = %d\n",shamt);
				printf("function = %d\n",funct);
				break;
			case 0x2: 
				id.decodeJ();
				printf("J-Type\n");
				address = id.address;   
        printf("opcode = %d\n",opcode);
				printf("address = %d\n",address);
				break;
			case 0x3: 
				id.decodeJ();
				printf("J-Type\n");
				address = id.address;  
        printf("opcode = %d\n",opcode);
				printf("address = %d\n",address);
				break;
			default: 
				id.decodeI();
				printf("I-Type\n");
				rs = id.rs;
				rt = id.rt;
				immediate = id.immediate;
        printf("opcode = %d\n",opcode);
				printf("rs = %d\n",rs);
				printf("rt = %d\n",rt);
				printf("immediate = %d\n",immediate);
				break;
		}

	return(0);
}