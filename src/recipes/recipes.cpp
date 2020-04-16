#include "recipes.h"

#define MAXPARSTRINGS 10 // Size of array that holds parameter names
#define MAX_LENGTH 80
//Constructor
recipes::recipes(){ 
  //Nothing to do
};

int recipe::count = -1;

void delay(int val){
	for(int i; i<val; i++);
}

/* Read one line from file, return store into line. Return true if end of line
was found */
bool readOneLine(char* line){
	char ch; //single characters read from file
	bool endofline = false; //did we run into a \n character?
	bool linefound = false;
	int idx = 0;
	// Read file until you find a non-comment line or run into the end of file
	while (myIO->recipeFileavailable() && not linefound){
		memset(line,'\0', MAX_LENGTH); // clear line contents
		while (myIO->recipeFileavailable() && !endofline) { 
			ch = myIO->recipeFileread();
			if(ch !='\n'){
				line[idx] = ch;
				idx++;
			}
			
			else{
				endofline = true;
			}
		}
		// Comment line starts with #, ignore it:
		if (endofline && line[0] == '#') {
			//myIO->serialPrint((char*)"Ignoring comment line:");
		}
		else{
			linefound=true;
		}
	}
	// myIO->serialPrint((char*)"Line found: ");
	//myIO->serialPrintln(line);				
	return linefound;
}

/* Search for delimiters in the line, and store the strings between
   the delimeters ch (e.g. ",") into col array of Strings. Return the
   number of col found */
int readColumns(char* line, const char* ch, char * col[], int numCol){
	int i=0; //counts columns
	int pos = 0;
	//myIO->serialPrintln((char*)"readColumns: ");myIO->serialPrint(line);
	// As long as there is at least two characters left and we want more columns
	while (strlen(line) > 1 && i<numCol){
	    pos =strstr(line, ch) - line;// find location of first delimeter
		// get part before delimeter
		for(int j=0; j< pos; j++){
			col[i][j] = line[j];
		}
		col[i][pos]='\0';
		//myIO->serialPrintln(col[i]);
		i++;
		line+= pos+1; // cut that part of the line, including delimeter
		//myIO->serialPrintln((char*)"After remove: ");myIO->serialPrint(line);
	}
	return i;//Length of col, [0..i-1].
}


/* Strip first and last quotes of text string. NOT TESTED! */
bool stripQuotes(char * str){
	//Should check if string really has quotes! Leon
	char * temp = new char[strlen(str)];
	//myIO->serialPrintln((char*)"stripQuotes: ");myIO->serialPrintln(str);
	strncpy(temp, str + 1, strlen(str)-2);
	str = temp;
	// myIO->serialPrint((char *)" -> ");
	//myIO->serialPrint(str);
  
	return true;
}

/* Are two strings equal? Ignore case */
bool comparestring(const char* s1, const char* s2){
	//myIO->serialPrintln((char*)"comparestring: ");myIO->serialPrintln(s1);myIO->serialPrintln("\t"s2);
	//if (s1.equalsIgnoreCase(s2)) { Serial.println("equal"); };
	return strcasecmp(s1, s2) == 0;
}

/* Which MagOD version do we have. Return 0 if not found */
int getversion(char* str){
	int version = 0;
	//cout << "getVersion: str= " << str << endl;
	if (comparestring(str,"MAGOD1")) version = 1;
	else if (comparestring(str,"MAGOD2")) version = 2;
    else myIO->serialPrintln((char*)"Version not recognized");
	return version;
}

/* Get list of parameter names from col[], which has length N.
Non-empty names are stored in names[]. Returns length of list [0:length] */
int getparameters(char * names[], char * col[], int N){
	int j=0;
	for (int i = 0; i < N; i++){
		names[i] = new char[MAX_LENGTH];
	}
	for (int i=0;( i<= 10 && i <=N) ; i++){
    // Forget the empty fields
		if (strlen(col[i]) != 0) {
			strcpy(names[j], col[i]);
			j=j+1;}
	};
	return j-1;
}

/* convert LED color in string to int, return -1 if not recognized */
int getLEDColor(char* str){
	if (comparestring(str,"GREEN")) return GREEN;
	if (comparestring(str,"RED")) return RED;
	if (comparestring(str,"BLUE")) return BLUE;
	return -1;
}

/* Read one line of the sequence and store the field and led parameter at location 'num'*/
bool readSequenceStep(char* line, sequence& seq, int num){
	bool stepFound=false;
	int N=7;
	char * col[N]; //We need to read 7 columns (0:6)
	int numCol; //Number of columns read
	
	for (int i = 0; i < N; i++){
		col[i] = new char[MAX_LENGTH];
	}
	numCol = readColumns(line, ",", col, N);
	//	 We expect six columns  (col[0] is empty):
	if (numCol>5){
		seq.Bx[num]            = atof(col[1]);//B_x[mT]
		seq.By[num]            = atof(col[2]);//B_y[mT]
		seq.Bz[num]            = atof(col[3]);//B_z[mT]
		seq.time[num]          = atof(col[4])*1000;//T_switch[ms]
		seq.led[num].color     = getLEDColor(col[5]);//LED Color (RED, GREEN, BLUE)
		seq.led[num].intensity = atoi(col[6]);//LED Intensity
		//Check if all values make sense:
		stepFound=(seq.Bx[num]>=0 && seq.By[num]>=0 && seq.Bz[num]>=0 &&
	       seq.time[num]>=0 && seq.led[num].color>=0 && seq.led[num].intensity >=0);
	}
	return stepFound;
}

/* Read sequence of steps from file, until @ endSequence is found */
bool readSequence(char* line, sequence& seq){
	bool endSequence=false;
	//char * line;
	int N=2;
	char * col[N]; //We need to read only 2 columns to find endSequence
	int numCol; // number of columns read
	int sequenceNumber=0;
	for (int i = 0; i < N; i++){
		col[i] = new char[MAX_LENGTH];
	}
	/* Read until you find EndSequence. The order of checking is
     important, first check endSequence, so that you don't read one
     line too many */
	while(not endSequence && readOneLine(line)){
		if (line[0] == '@'){
			numCol = readColumns(line, ",", col, N);
			endSequence=comparestring(col[1],"EndSequence");
			//if (endSequence) myIO->serialPrintln((char*)"End sequence found");
		}
		else { // We assume every line has a sequence step. Can be broken! LEON
			readSequenceStep(line, seq, sequenceNumber);
			sequenceNumber=sequenceNumber+1;
			//myIO->serialPrintln((char*)"sequenceNumber: ");myIO->serialPrint(sequenceNumber);
		}
	}
	seq.length=sequenceNumber-1; //Number of steps in this sequence
	//myIO->serialPrintln((char*)"Readsequence: Found number of steps: ");myIO->serialPrint(seq.length);
	return endSequence; //If all went well, endSequence is true
}

/* Load recipes from recipes file and store into array. Returns the
   number of recipes found */
int recipes::LoadRecipes()
{
	recipe * recipes_array = new recipe[MaxRecipes];
	int linecount = 0; //For debug only
	bool error = false;
	int version = 0 ;
	char* line = new char[MAX_LENGTH];
	int N = 10; // Maximum number of columns we expect to read
	char* col[N];
	int numCol; // Number of columns found on line
	//int recipeNumber; // We can load several recipes, numbered [0..MaxRecipes]
  
	for (int i = 0; i < N; i++){
		col[i] = new char[MAX_LENGTH];
    }
	//recipeNumber=-1;
	while(myIO->recipeFileavailable() && not error){
		// Read one line from the recipe file, ignore comment lines
		if (readOneLine(line)) {
			linecount++;
			if(line[0] == '@'){
			// Split line into column fields at comma's
				numCol = readColumns(line, ",", col, N);
				// Version number
				if (comparestring(col[1],"Version")) {
					version=getversion(col[2]);
					if (version==0) {error=true; break;}
				};
				// Parameter list
				if (comparestring(col[1],"Parameters")) {
					// To be done. It is good to check if the order is what we expect. LEON
				}
				// Recipe
				if (comparestring(col[1],"Recipe") && recipe::count < MaxRecipes && not error){
					// The recipe description is on the same line in col[2]:
					recipe::count++;//recipeNumber initialized at -1.
					strncpy(recipes_array[recipe::count].name,col[2],strlen(col[2])+1);
					myIO->serialPrintln(recipes_array[recipe::count].name);
					bool recipeEnd=false;
					//Keep reading until you find EndRecipe
					while (readOneLine(line) && not recipeEnd && not error){
						if (line[0] == '@') {
							// Split lines into columns
							numCol = readColumns(line, ",", col, N);
							// Number of cycles
							if (comparestring(col[1],"N_cycles")){
								recipes_array[recipe::count].N_cycles = atoi(col[2]);
							}	
							// Sequence
							if (comparestring(col[1],"Sequence")){
								if (not readSequence(line, recipes_array[recipe::count].recipe_sequence)){
									myIO->serialPrintln((char*)"Error, sequence not found");
									error=true;
								}
							}
						// End Recipe
						recipeEnd=comparestring(col[1],"EndRecipe");
						};//End if line starts with @
					};//End while readOneLine
				};//end if col[1]=Recipe
			};// end if "@"
		};//End if readOneLine for entire recipe
	} 
	/* For debug, send recipe to serial port for monitoring */
	/*for (int i=0;i<=recipe::count;i++){
		myIO->serialPrintln((char*)"");
		myIO->serialPrint(recipes_array[i].name); myIO->serialPrint((char*)"  ");
		myIO->serialPrint((char*)" Number of steps : " );myIO->serialPrint(recipes_array[i].recipe_sequence.length);
		myIO->serialPrintln((char*)"");
		for (int j=1;j<=recipes_array[i].recipe_sequence.length;j++){
			myIO->serialPrintln((char*)"");
			myIO->serialPrint((char*)"[ ");
			myIO->serialPrint(recipes_array[i].recipe_sequence.Bx[j]);myIO->serialPrint((char*)", ");
			myIO->serialPrint(recipes_array[i].recipe_sequence.By[j]);myIO->serialPrint((char*)", ");
			myIO->serialPrint(recipes_array[i].recipe_sequence.Bz[j]);myIO->serialPrint((char*)", ");
			myIO->serialPrint(recipes_array[i].recipe_sequence.time[j]);myIO->serialPrint((char*)", ");
			myIO->serialPrint(recipes_array[i].recipe_sequence.led[j].color);myIO->serialPrint((char*)", ");
			myIO->serialPrint(recipes_array[i].recipe_sequence.led[j].intensity);
			myIO->serialPrint((char*)"] ");
			myIO->serialPrintln((char*)"");
		}
	}*/
	return recipe::count; /* If succesfull recipeNumber >=0 */
}//End of LoadRecipes


/* Program_init()
This is the main function to change when altering the programs. This defines what each individual program can do. In an older version, the programs were hard coded. Later we defined a RECIPES.CSV that is read from flash, which is easier for the user. The code was tweaked, so it has a lot of remnants of the hard coded period. Needs major rehaul. LEON

The most important setting is the field B_arrayfield_i[j] (i = x or y or z) (j = 1,2,3,4... B_nr_set) which stores the magnetic fields that should be applied in consecutive order. Furthermore the timing should be stored in the Switching_time[j] array, which stores how long the magnetic field is kept at the value of B_arrayfield_i[j]. The length of these arrays should always be equal to B_nr_set. Some extra parameters are 
   //LED_switch_cycles;  whether the led color should switch after a number of cycles (0 = no switch, always the same color)
   //Nr_cycles; whether the program should stop after a number of cycles (0 = never stops)
   //Period_current_sense; //frequency of the current update
   //extra_par; //the value of the extra parameter that is saved in the header of the .CSV file
//all these parameters are initialized at standard values:
    //B_arrayfield_x[j] = 0.0; 
    //B_arrayfield_y[j] = 0.0; 
    //B_arrayfield_z[j] = 0.0;
    //Switching_time[j] = 500; (ms)
    //Gradient_x[j] = 1;
    //Gradient_y[j] = 1;
    //Gradient_z[j] = 1;
    //LED_switch_cycles = 0;
    //Nr_cycles = 0;
    //Period_current_sense = 1;
    //extra_par = 0.0;
    //ONLY changes to these standard settings should be programmed (when you want B_arrayfield_x[3] to be 0 in program x the program does not need the line B_arrayfield_x[3] = 0;) 

B_nr_max is set in MagOD2.ino. Increase if you need bigger sequences:
#define B_nr_max 12 //Max number of elements

Input: 
recipe_array: array of recipes describing measurement procedure
recipe_num: which recipe the user selected

*/

void recipes::program_init(int recipe_num)
{   
	unsigned int B_nr_set; 
	unsigned long Switching_time[B_NR_MAX]; 
	double B_arrayfield_x[B_NR_MAX];
	double B_arrayfield_y[B_NR_MAX];
	double B_arrayfield_z[B_NR_MAX];
	int LEDColor_array[B_NR_MAX];
	int LEDInt_array[B_NR_MAX];
	bool Gradient_x[B_NR_MAX];
	bool Gradient_y[B_NR_MAX];
	bool Gradient_z[B_NR_MAX];
	
	recipe * recipes_array = new recipe[MaxRecipes];
	myIO->serialPrintln((char*)"Starting program_init");delay(1000);

	// Reset all arrays,default to do nothing for 1 second 
	B_nr_set = 1; // [0..1]
	memset(B_arrayfield_x, 0, B_NR_MAX); // Fields off
	memset(B_arrayfield_y, 0, B_NR_MAX);
	memset(B_arrayfield_z, 0, B_NR_MAX);
	memset(Switching_time,1000,B_NR_MAX); // Switching times to 100 ms
	memset(Gradient_x, 1, B_NR_MAX); // Single coil operation (1)
	memset(Gradient_y, 1, B_NR_MAX);
	memset(Gradient_z, 1, B_NR_MAX);
	
	// Copy measurement settings from recipes_array. Inefficient? See above. LEON 
	B_nr_set =  recipes_array[recipe_num].recipe_sequence.length; 
	
	for (int i=0; i <= B_nr_set; i++){
		B_arrayfield_x[i] = recipes_array[recipe_num].recipe_sequence.Bx[i];
		B_arrayfield_y[i] = recipes_array[recipe_num].recipe_sequence.By[i];
		B_arrayfield_z[i] = recipes_array[recipe_num].recipe_sequence.Bz[i];
		Switching_time[i] = recipes_array[recipe_num].recipe_sequence.time[i];
		LEDColor_array[i] = recipes_array[recipe_num].recipe_sequence.led[i].color;
		LEDInt_array[i]   = recipes_array[recipe_num].recipe_sequence.led[i].intensity;
	}
}