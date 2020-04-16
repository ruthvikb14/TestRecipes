#include "IO.h"

/* ***************************************************************** */
#if defined(stdioVersion)

/* IO functions for command line on PC */

//Constructor
IO::IO(char * fname){
  recipeFile_.open(fname, ios::in);
}

void IO::initSerial(){
  //Nothing to do
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
	//cout << c;
	return c;
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
  recipeFile_.close();
}

/* ***************************************************************** */
/* IO functions for Arduino */
#elif defined(SDVersion)

//Constructor
IO::IO(){
  //Nothing to do
}

void IO::initSerial(){
  Serial.begin(9600);
}

void IO::serialPrint(const char* message) {
  Serial.print(message);
}

void IO::serialPrintln(const char* message) {
  Serial.println(message);
}

/* Test if recipes file is present */
bool IO::checkFile(char* filename) {
  recipeFile = SD.open("/RECIPES.CSV"); // recipeFile is global to IO.h
  if (recipeFile) {
      return true;
    }
  else {
    return false;
  }
}
/* Check if recipefile still has on more character */
bool IO::recipeFileavailable() {
  return recipeFile.available();
}

/* Read next character from recipe file */
char IO::recipeFileread(){
  return recipeFile.read();
}

/* Close the recipefile */
void IO::recipeFileclose(){
  recipeFile.close();
}

#endif
