#ifndef IO_h
#define IO_h

#if defined(stdioVersion)
/*IO for command line on PC */
//#warning "Compiling for command line stdio"
#include <stdio.h>
#include<fstream>
//#include <sstream>
//#include<iostream>
#include <cstring>
//#include<cstdlib>

using namespace std;

#elif defined(SDVersion)
/*IO for serial port on Arduino */
#warning "Compiling for Arduino serial"
#include <Arduino.h>
#include <SD.h>
#endif

class IO {
	private:
        // Quick fix, REMOVE LATER!!! LEON
#if defined(stdioVersion)
        ifstream recipeFile_; // Global variable to remember the file handle
#elif defined(SDVersion)
        File recipeFile;
#endif
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
