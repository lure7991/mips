#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Fetch{
	 public:
	 	int instructionIndex, sp, fp, pc;
		int instruction[1200];
		void fill();

} IF; 

void Fetch::fill(){
		string instructionInput;
		ifstream inputFile("instruction.txt");

		instructionIndex = 1;
		if (inputFile.is_open()){
			instructionIndex = 1;
			while (getline(inputFile,instructionInput)){
				instruction[instructionIndex] = strtoul(instructionInput.substr(2, 10).c_str(), NULL, 16);
				instructionIndex++;
			}
			inputFile.close();
		} 
		instruction[instructionIndex] = -1;
	
		sp = instruction[1];
		fp = instruction[2];
		pc = instruction[6];
}