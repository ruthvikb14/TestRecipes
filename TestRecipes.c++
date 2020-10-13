#include "TestRecipes.h"

IO *  myIO; /*The exact IO routines depend on platform:
	      ESP32 serial monitor (ESP_PLATFORM)
	      Command line (stdioVersion)) */
recipes * myrecipes; //Defintion of recipe file

/* Arrays that stores a single recipe, for use in MagOD.ino */
unsigned int  B_nr_set; 
unsigned long Switching_time[B_NR_MAX]; 
double        B_arrayfield_x[B_NR_MAX];
double        B_arrayfield_y[B_NR_MAX];
double        B_arrayfield_z[B_NR_MAX];
int           LEDColor_array[B_NR_MAX];
int           LEDInt_array[B_NR_MAX];
bool          Gradient_x[B_NR_MAX];
bool          Gradient_y[B_NR_MAX];
bool          Gradient_z[B_NR_MAX];

// Help variables
bool done = false;

#if defined(stdioVersion)
/* For printing the recipes in a csv file*/
char* led_color(int i){
  if(i==0) return ((char*)"RED");
  else if(i ==1) return ((char*)"GREEN");
  else return ((char*)"BLUE");
  }
void serialRecipesFile(recipe* recipe, int numRecipes){
  int count = 0;
  if(myIO->recipeOut && !myIO->error_flag){
    myIO->recipeOut << "Step" << ","  << "Recipe Number" << "," << "B x" << "," << "B y" << ","
     << "B z" << "," << "Led Time" << "," << "Led Color" << ","
      << "Led Intensity" << "," << "Grad x" << "," << "Grad y" << "," << "Grad z" << endl;
    for (int i=0;i<=numRecipes;i++){
      for (int j=1;j<=recipe[i].recipe_sequence.length;j++){
        count+=1;
        myIO->recipeOut << count << "," << i+1 << "," << recipe[i].recipe_sequence.Bx[j] << ","
        << recipe[i].recipe_sequence.By[j] << ", "
        << recipe[i].recipe_sequence.Bz[j] << ", "
        << recipe[i].recipe_sequence.time[j] << ", "
        << led_color(recipe[i].recipe_sequence.led[j].color) << ", "
        << recipe[i].recipe_sequence.led[j].intensity << ", "
        << recipe[i].recipe_sequence.grad[j].grad_x << ", "
        << recipe[i].recipe_sequence.grad[j].grad_y << ", "
        << recipe[i].recipe_sequence.grad[j].grad_z << endl;
      }  
    }
  }
  else myIO->recipeOut << "**Error(s) in the Recipe file**" << endl;
}
#endif

int main(int argc, char** argv){
  char filename[20];
  
  #if defined(stdioVersion)
    myIO->initSerial();
    if(argc == 1 || argc > 3){
      myIO->serialPrintln((char*)"Usage: TestRecipes [FILE]");
      myIO->serialPrintln((char*)"Usage: TestRecipes -v [FILE]");
      myIO->serialPrintln((char*)"Usage: TestRecipes -o [FILE]");
    }
    else{
      if(strcmp(argv[1],"-v")==0) strcpy(filename, argv[2]);
      else if(strcmp(argv[1],"-o")==0) strcpy(filename, argv[2]);
      else strcpy(filename, argv[1]);
    }
  #elif defined(ESP_PLATFORM)
    strcpy(filename, argv[0]);
  #endif
  
  myIO = new IO(filename);
  
  /* check if Recipes file exists */ 
  bool check = myIO->checkFile();
  if(!check){
    //myIO->serialPrintln((char*)"Failed to open file");
    return 0;

    }
    
  /* For debug: Read file contents character by character and display
     on command line or serial monitor */
  //while(myIO->recipeFileavailable())
  //{
  //myIO->serialPrintch(myIO->recipeFileread());
  //}
  //myIO->recipeFileclose();
    
  // myIO = new IO(filename); 
  
  /* Read Recipes from file */
  recipe* numRecipes = myrecipes->LoadRecipes();
  if(strcmp(argv[1],"-v")==0) myrecipes->serialRecipesPrint(numRecipes, numRecipes->count);
  else if(strcmp(argv[1],"-o")==0) serialRecipesFile(numRecipes, numRecipes->count);  
  #if defined(ESP_PLATFORM)
    myrecipes->serialRecipesPrint(numRecipes, numRecipes->count);
  #endif
  //myIO->serialPrintln((char*)"");
  //myIO->serialPrint((char*)"Number of recipes found : ");
  //myIO->serialPrintln(numRecipes->count+1);
  
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
