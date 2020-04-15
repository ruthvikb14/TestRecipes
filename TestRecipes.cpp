<<<<<<< HEAD
#include "./fileStdio/IO.h"
#include "TestRecipes.h"
#include<cstdio>
#include "./src/recipes/recipes.h"

IO * myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

void setup(){
	myIO->initSerial();
}

int main(){
	// myIO->serialPrintln((char*)"TestRecipes, expects RECIPES.CSV in current directory");
	// myIO->serialPrintln(1000);

	/* check if Recipes file exists */
	char filename[] = "RECIPES.CSV";
	char m;
	
	myIO = new IO(filename);
  
	if(!myIO->recipeFileavailable()){
		cout << "Failed to open file for writing recipes file" << endl;
		return 0;
    }
	
	//m = myIO->recipeFileread();
	/* Read file contents character by character and display on command line or serial monitor */
	// while(m!='\n'){
		// cout << m;
		// m = myIO->recipeFileread();
	// }
	//myIO->serialPrint((char*)"Number of recipes found : ");
	/* Read Recipes from file */
	int numRecipes= myrecipes->LoadRecipes(myrecipes->recipes_array);
	cout << numRecipes << endl;
	myIO->recipeFileclose();
}
=======
#include "./fileStdio/IO.h"
#include "TestRecipes.h"
#include<cstdio>
#include "./src/recipes/recipes.h"

IO * myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

void setup(){
	myIO->initSerial();
}

int main(){
	// myIO->serialPrintln((char*)"TestRecipes, expects RECIPES.CSV in current directory");
	// myIO->serialPrintln(1000);

	/* check if Recipes file exists */
	char filename[] = "RECIPES.CSV";
	char m;
	
	myIO = new IO(filename);
  
	if(!myIO->recipeFileavailable()){
		cout << "Failed to open file for writing recipes file" << endl;
		return 0;
    }
	
	//m = myIO->recipeFileread();
	/* Read file contents character by character and display on command line or serial monitor */
	// while(m!='\n'){
		// cout << m;
		// m = myIO->recipeFileread();
	// }
	//myIO->serialPrint((char*)"Number of recipes found : ");
	/* Read Recipes from file */
	int numRecipes= myrecipes->LoadRecipes(myrecipes->recipes_array);
	cout << numRecipes << endl;
	myIO->recipeFileclose();
}
>>>>>>> 611d5ecd304a4ea8da0d69a1a3ae196778539140
