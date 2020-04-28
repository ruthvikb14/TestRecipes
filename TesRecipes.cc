#include "TestRecipes.h"

IO *  myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

bool done = false;

int main(int argc, char** argv){
  
  char filename[20];
  //myIO.serialPrintln((char*)"TestRecipes, expects RECIPES.CSV in current directory");
  
  #if defined(stdioVersion)
    myIO->initSerial();
  #endif
  
  myIO->serialPrint((char*)"Number of arguments passed: ");
  myIO->serialPrintln(argc);
  myIO->serialPrintln((char*)"");
  
  strcpy(filename, argv[1]);
  
  myIO = new IO(filename);
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
char* file[]= {"\0","/RECIPES.CSV"};

void setup(){
  myIO->initSerial();
  //main();
}


void loop(){
  if (not done) {
    done = true;
    main(1, file);
  }
  delay(200);
}
#endif
