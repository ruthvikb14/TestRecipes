#include "TestRecipes.h"
//Don't inlude, IO.h. correct IO is loaded in Testrecipes.h //.
//Don't include any c specific libraries

IO * myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

void setup(){
	myIO->initSerial();
}

int main(){
  char filename[] = "RECIPES.CSV";
  char m;

  //myIO->serialPrintln((char*)"TestRecipes, expects RECIPES.CSV in current directory");
  // myIO->serialPrintln(1000);
  
  /* check if Recipes file exists */
  
  myIO = new IO(filename);
  
  if(!myIO->recipeFileavailable()){
    myIO->serialPrintln((char*)"Failed to open file for writing recipes file");
    return 0;
  }
  
  /* Test if file can be correctly read: Read file contents character
     by character and display on command line or serial monitor until EOF*/ 
  // m = myIO->recipeFileread();
  // myIO->serialPrint(&m);
  // while(myIO->recipeFileavailable()){
    // myIO->serialPrint(&m);
    // m = myIO->recipeFileread();
  // }
  // myIO->recipeFileclose();

  /* Read Recipes from file */
  myIO = new IO(filename);
  int numRecipes= myrecipes->LoadRecipes();
  myIO->serialPrintln((char*)"");
  myIO->serialPrint((char*)"Number of recipes found : ");
  myIO->serialPrint(numRecipes+1); // numRecipes starts from 0
  // m=numRecipes+'0';//This trick will only work for num [0..9]
  // myIO->serialPrintln(&m);
  myIO->recipeFileclose();
}
