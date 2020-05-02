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

int main(int argc, char** argv){
  char filename[20];
  
#if defined(stdioVersion)
  myIO->initSerial();
  // We expect one parameter only
  if(not(argc==2)){
    myIO->serialPrintln((char*)"Usage: TestRecipes [FILE]");
    return 0;
  }
  else{
    strcpy(filename, argv[1]);
  }
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
  
  /* For debug: Read file contents character by character and display
     on command line or serial monitor */
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

