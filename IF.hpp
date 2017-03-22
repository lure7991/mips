#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;


//Fetch an instruction from instruction memory 
// Read addr frm mem using PC val
// Place into IF/ID reg
// inc PC+1
// Save inc address into IF/ID
//Get PC val from main which may come from outside file?
class Fetch{
	 public:
	 	unsigned long hexInstruction;
	 	unsigned long hexPC;
	 	string instruction;
	 	string PC; //Change to array to hold multiple instructions? 
	 	Fetch (); 
	 	void getPC();
	 	void getInst();
	 	void fetchIF(int global_mem[]);

} IF; 

Fetch::Fetch() {
	// open a file in read mode.
	instruction = '0';
	PC= '0';
}

void Fetch::getPC(){ 
	ifstream pcFile; 
	pcFile.open("PC.txt");
	if (pcFile.is_open()){
		while(getline (pcFile, PC)){
			hexPC = stol(PC,nullptr, 16);
			// cout<<hexPC<< endl;
			// cout<<PC<<endl;
		}
	}
	pcFile.close();
}

void Fetch::getInst(){
	ifstream instructionFile;
	instructionFile.open("instruction.txt");
	if(instructionFile.is_open()){
		while (getline(instructionFile, instruction)){
			hexInstruction= stol(instruction, nullptr, 16);

			cout<<hexInstruction<<endl;
			cout<<instruction<<endl;
			//Place values into array ALT
		}
	}
}

void Fetch::fetchIF(int global_mem[]){
	global_mem[hexPC]= hexInstruction;
	cout<<global_mem[hexPC]<<endl; 
}