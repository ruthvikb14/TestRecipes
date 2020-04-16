/*IO for command line on PC */
#ifndef IO_h
#define IO_h

#include <stdio.h>
#include<fstream>
#include <sstream>
#include<iostream>
#include <cstring>
#include<cstdlib>

using namespace std;
 
class IO {
	private:
	// TO BE DONE:
	ifstream recipeFile_; // Global variable to remember the file handle
	// Quick fix, REMOVE LATER!!! LEON
  
	public:
	IO(); //constructor
	IO(char * ); //constructor
	/* Initialize serial communication */
	void initSerial();
	/* Message to serial monitor */
	void serialPrint(float message);
	void serialPrintln(float message);
	void serialPrint(char* message);
	void serialPrintln(char* message);
	/* Check for presence of file */
	bool checkFile(char* filename);
	/* Check if recipefile still has on more character */
	bool recipeFileavailable();
	/* Read next character from recipe file */
	char recipeFileread();
	/* Close the recipefile */
	void recipeFileclose();
};

#endif
