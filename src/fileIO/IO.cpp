#include "IO.h"

/* ***************************************************************** */

/* IO functions for command line on PC */

//Constructor
IO::IO(char * fname){
	recipeFile_.open(fname, ios::in);
}

void IO::initSerial(){
  //Nothing to do
	printf("%63s\n", "**********************************************");
	printf("%50s","MAGOD Recipes By");
	printf("\n%60s\n", "Prof. Dr. Leon Abelmann & B.S.R.Ruthvik");
	printf("%63s\n", "**********************************************");
}

void IO::serialPrint(float message) {
	printf("%.1f",message);
}

void IO::serialPrintln(float message) {
	printf("%.1f\n",message);
}

void IO::serialPrint(char * message) {
	printf("%s",message);
}

void IO::serialPrintln(char * message) {
	printf("%s\n",message);
}

/* Test if recipes file is present */
bool IO::checkFile() {
  // recipeFile is global to IO.h
	if (recipeFile_.good()) {
		serialPrintln((char*)"****File checked****"); 
		return true;
	}
	else {
		serialPrintln((char*)"****File not exists****");
		return false;
	}
}


/* Check if recipefile still has on more character */
bool IO::recipeFileavailable() {
	return recipeFile_.is_open() && !recipeFile_.eof();
}


/* Read next character from recipe file */
char IO::recipeFileread(){
	char c;
	recipeFile_.get(c);
	return c;
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
	recipeFile_.close();
}

