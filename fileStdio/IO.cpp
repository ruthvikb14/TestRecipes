<<<<<<< HEAD
/* IO functions for command line on PC */
#include "IO.h"


//Constructor
IO::IO(char * fname){
  //Nothing to do
  recipeFile_.open(fname, ios::in);
}
void IO::initSerial(){
  //Nothing to do
}

// void IO::serialPrint(float msg) {

  // printf("%f",msg);
// }

// void IO::serialPrintln(float msg) {
  // printf("%f\n",msg);
// }

// void IO::serialPrint(char * message) {

  // printf("%s",message);
// }

// void IO::serialPrintln(char * message) {
  // printf("%s\n",message);
// }

/* Test if recipes file is present */
bool IO::checkFile(char* filename) {
  // THIS NEED TO BE DONE:
  //recipeFile = SD.open("/RECIPES.CSV"); // recipeFile is global to IO.h
  if (recipeFile_.good()) {
	  cout << "File checked" << endl;
      return true;
    }
  else {
	  cout << "File not exists" << endl;
    return false;
  }
}

/* Check if recipefile still has on more character */
bool IO::recipeFileavailable() {
  // THIS NEED TO BE DONE:
  return recipeFile_.is_open() && !recipeFile_.eof();
}


/* Read next character from recipe file */
char IO::recipeFileread(){
	char c;
	// THIS NEED TO BE DONE:
	recipeFile_.get(c);
	//cout << c << endl;
	return c;
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
  recipeFile_.close();
}
=======
/* IO functions for command line on PC */
#include "IO.h"


//Constructor
IO::IO(char * fname){
  //Nothing to do
  recipeFile_.open(fname, ios::in);
}
void IO::initSerial(){
  //Nothing to do
}

// void IO::serialPrint(float msg) {

  // printf("%f",msg);
// }

// void IO::serialPrintln(float msg) {
  // printf("%f\n",msg);
// }

// void IO::serialPrint(char * message) {

  // printf("%s",message);
// }

// void IO::serialPrintln(char * message) {
  // printf("%s\n",message);
// }

/* Test if recipes file is present */
bool IO::checkFile(char* filename) {
  // THIS NEED TO BE DONE:
  //recipeFile = SD.open("/RECIPES.CSV"); // recipeFile is global to IO.h
  if (recipeFile_.good()) {
	  cout << "File checked" << endl;
      return true;
    }
  else {
	  cout << "File not exists" << endl;
    return false;
  }
}

/* Check if recipefile still has on more character */
bool IO::recipeFileavailable() {
  // THIS NEED TO BE DONE:
  return recipeFile_.is_open() && !recipeFile_.eof();
}


/* Read next character from recipe file */
char IO::recipeFileread(){
	char c;
	// THIS NEED TO BE DONE:
	recipeFile_.get(c);
	//cout << c << endl;
	return c;
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
  recipeFile_.close();
}
>>>>>>> 611d5ecd304a4ea8da0d69a1a3ae196778539140
