#include <stdio.h>
#include <stdlib.h>

class Memory{
	public:
			int opcode, function, shamt, immediate, address, jumpAddress, pc, ra, rd, rt, rs;
			void store();
			void load();
} mem;

void Memory::store(){
	printf("reached mem store\n");
}

void Memory::load(){
	printf("reached mem store\n");
}