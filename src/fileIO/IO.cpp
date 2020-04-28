#include "IO.h"

/* ***************************************************************** */

/* IO functions for command line on PC */

//Constructor
IO::IO(char * fname){
#if defined(stdioVersion)
  recipeFile_.open(fname, ios::in);
#elif defined(ESP_PLATFORM)
  //Nothing to do
#endif
}

void IO::initSerial(){
#if defined(stdioVersion)
  //Nothing to do
#elif defined(ESP_PLATFORM)
  Serial.begin(9600);
#endif
}

void IO::serialPrint(float message) {
#if defined(stdioVersion)
  printf("%.1f",message);
#elif defined(ESP_PLATFORM)
  Serial.print(message);
#endif
}

void IO::serialPrintln(float message) {
#if defined(stdioVersion)
  printf("%.1f\n",message);
#elif defined(ESP_PLATFORM)
  Serial.println(message);
#endif
}

void IO::serialPrint(char * message) {
#if defined(stdioVersion)
  printf("%s",message);
#elif defined(ESP_PLATFORM)
  Serial.print(message);
#endif
}

void IO::serialPrintln(char * message) {
#if defined(stdioVersion)
  printf("%s\n",message);
#elif defined(ESP_PLATFORM)
  Serial.println(message);
#endif
}

/* Test if recipes file is present */
bool IO::checkFile(char* filename) {
  // recipeFile is global to IO.h
#if defined(stdioVersion)
  if (recipeFile_.good()) {
    serialPrintln((char*)"File checked"); 
    return true;
  }
  else {
    serialPrintln((char*)"File not exists");
    return false;
  }
#elif defined(ESP_PLATFORM)
  recipeFile = SD.open("/RECIPES.CSV"); 
  if (recipeFile) {
    return true;
  }
  else {
    serialPrintln((char*)"File not exists");
    return false;
  }
#endif
}


/* Check if recipefile still has on more character */
bool IO::recipeFileavailable() {
#if defined(stdioVersion)
  return recipeFile_.is_open() && !recipeFile_.eof();
#elif defined(ESP_PLATFORM)
  return recipeFile.available();
#endif
}


/* Read next character from recipe file */
char IO::recipeFileread(){
#if defined(stdioVersion)
	char c;
	recipeFile_.get(c);
	//cout << c;
	return c;
#elif defined(ESP_PLATFORM)
	return recipeFile.read();
#endif
}

/* Close the recipefile */
void IO::recipeFileclose(){
#if defined(stdioVersion)
  recipeFile_.close();
#elif defined(ESP_PLATFORM)
  recipeFile.close();
#endif
}

