#include <stdio.h>
#include <fstream>
#include <iostream>


//Fetch an instruction from instruction memory 
// Read addr frm mem using PC val
// Place into IF/ID reg
// inc PC+1
// Save inc address into IF/ID
//Get PC val from main which may come from outside file?
class Fetch{
	 public:
	 	int instruction;
	 	int PC;
	 	Fetch (); 
	 	void fetchIF();

} IF; 

Fetch::Fetch() {
	// open a file in read mode.
   	int PC; 
   	
}