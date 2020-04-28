#include "TestRecipes.h"

//Don't inlude, IO.h. correct IO is loaded in Testrecipes.h //.

IO * myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

int main(int argc, char** argv){
	myIO->initSerial();
	
	char filename[20];

	if (argc==1) {
	  myIO->serialPrintln((char *)"Usage: TestRecipes [FILE]..."); 
	}
	else {
	  strcpy(filename,argv[1]);
	  //myIO->serialPrintln(filename);
	}

	
	// myIO->serialPrint((char*)"Please enter the file name: ");
	// scanf("%s", filename);
	// myIO->serialPrint("");
  
	myIO = new IO(filename);
  
	bool check = myIO->checkFile(filename);
  
	if(!check){
		myIO->serialPrintln((char*)"Failed to open file for writing recipes file");
		return 0;
	}
	
	int numRecipes= myrecipes->LoadRecipes();
	myIO->serialPrintln((char*)"");
	myIO->serialPrint((char*)"Number of recipes found : ");
	myIO->serialPrintln(numRecipes+1); // numRecipes starts from 0
	myIO->recipeFileclose();
}

#if defined(ESP_PLATFORM)
void setup(){
  //TO BE DONE
}

void loop(){
  //do nothing
}
#endif
