#include "TestRecipes.h"

IO *  myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

bool done = false;

char* file(){
  char* filename;
  #if defined(stdioVersion)
	filename = "RECIPES1.csv";
  #elif defined(ESP_PLATFORM)
    filename = "/RECIPES.csv";
  #endif
  myIO->serialPrintln(filename);
  return filename;
}

int main(char* argv){
  
  //myIO.serialPrintln((char*)"TestRecipes, expects RECIPES.CSV in current directory");

  //file();
  myIO->initSerial();
  
  myIO = new IO(file());
  /* check if Recipes file exists */
  
  bool check = myIO->checkFile();
  if(!check){
    myIO->serialPrintln((char*)"Failed to open file for writing recipes file");
    return 0;
    }

  /* Read file contents character by character and display on command
     line or serial monitor */
  //while(myIO->recipeFileavailable())
    //{
      //myIO->serialPrint((char*)myIO->recipeFileread());
    //}
    
  /* Read Recipes from file */
  int numRecipes= myrecipes->LoadRecipes();
  myIO->serialPrintln((char*)"");
  myIO->serialPrint((char*)"Number of recipes found : ");
  myIO->serialPrintln(numRecipes+1);
  myIO->recipeFileclose();
}

#if defined(ESP_PLATFORM)
void setup(){
  myIO->initSerial();
  //main();
}


void loop(){
  if (not done) {
    done = true;
    main();
  }
  delay(200);
  }
#endif