#include "TestRecipes.h"

IO *  myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

bool done = false;

int main(int argc, char** argv){
  
  char filename[20];
  //myIO.serialPrintln((char*)"TestRecipes, expects RECIPES.CSV in current directory");
  
  #if defined(stdioVersion)
    myIO->initSerial();
    if(argc==1){
      myIO->serialPrintln((char*)"Please provide the filename as in the format below");
      myIO->serialPrintln((char*)"TestRecipes filename (for DOS)");
      myIO->serialPrintln((char*)"./TestRecipes filename (for Mac)");
    }
    strcpy(filename, argv[1]);
  #elif defined(ESP_PLATFORM)
    strcpy(filename, argv[0]);
  #endif
  
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
      //myIO->serialPrintch(myIO->recipeFileread());
    //}
  //myIO->recipeFileclose();
  
  //myIO = new IO(filename); 
  
  /* Read Recipes from file */
  int numRecipes= myrecipes->LoadRecipes();
  myIO->serialPrintln((char*)"");
  myIO->serialPrint((char*)"Number of recipes found : ");
  myIO->serialPrintln(numRecipes+1);
  
  myIO->recipeFileclose();
  
}

#if defined(ESP_PLATFORM)
char* file[]= {"/RECIPES.CSV"};
int n_arg = sizeof(file)/sizeof(file[0]);
void setup(){
  myIO->initSerial();
  //main();
}


void loop(){
  if (not done) {
    done = true;
    main(n_arg, file);
  }
  delay(200);
}
#endif
