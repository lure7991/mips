#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Fetch{
	 public:
	 	int instructionIndex, sp, fp, pc;
		int instruction[500];
		void fill();

} IF; 

void Fetch::fill(){
		string instructionInput;
		ifstream inputFile("instruction.txt");

		instructionIndex = 0;
		if (inputFile.is_open()){
			instructionIndex = 0;
			while (getline(inputFile,instructionInput)){
				instruction[instructionIndex] = strtoul(instructionInput.substr(2, 10).c_str(), NULL, 16);
				instructionIndex++;
			}
			inputFile.close();
		} 
		instruction[instructionIndex] = -1;
	
	int k=1;
	for(k=1; k<200;k++) printf("%d 0x%x || ",k,instruction[k]);
	
		sp = instruction[0];
		fp = instruction[1];
		pc = instruction[5];
}