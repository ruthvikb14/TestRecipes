
.DEFAULT_GOAL := TestRecipes

CC=g++ #Compiler
CFLAGS=-D stdioVersion #Compiler directive for command line version

# Macro for MagOD object files
OBJ_RECIPES = src/recipes/recipes.o src/led/led.o

# Macro for object files
OBJ = TestRecipes.o src/fileIO/IO.o $(OBJ_RECIPES)

# Macro for MagOD includes files:
DEPS_RECIPES = src/recipes/recipes.h src/led/led.h 

# Macro for all dependencies
DEPS = TestRecipes.h src/fileIO/IO.h $(DEPS_RECIPES)

# All cpp files depend on the includes, so recompile their objects:
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# TestRecipes is special, Arduino IDE does not like a cpp in the root,
# instead we use a c++ (link to TestRecipes.ino using
# Unix: ln -s TestRecipes.ino TestRecipes.c++
# Windows: ??
TestRecipes.o: TestRecipes.c++ $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Dependencies of TestRecipes and what to include
TestRecipes: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Remove all compiled files
clean :
	@rm -f TestRecipes TestRecipes.o src/fileIO/IO.o \
	src/recipes/recipes.o src/led/led.o
