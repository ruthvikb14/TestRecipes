#ifndef IO_h
#define IO_h

#include <cstring>
#include <stdio.h>
/*IO for command line on PC */
#include<fstream>

using namespace std;

class IO {
	private:
    // Quick fix, REMOVE LATER!!! LEON
    ifstream recipeFile_; // Global variable to remember the file handle

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
	bool checkFile();
	/* Check if recipefile still has on more character */
	bool recipeFileavailable();
	/* Read next character from recipe file */
	char recipeFileread();
	/* Close the recipefile */
	void recipeFileclose();
};

#endif
