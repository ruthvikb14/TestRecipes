# TestRecipes
Ruthvik Bandaru, Leon Abelmann
April 2020

Environment to test loading of MagOD recipes files. TestRecipes.ino is the
main file. It compiles with Arduino IDE. This file is symbolically linked to
TestRecipes.cc, which compiles with g++ using a makefile. So do all
editing in TestRecipes.ino.

The Arduino version reads RECIPES.CSV from SD card and outputs debug
information on the serial port (serial monitor).

The command line version reads a recipe file as parameter, use for instance as
% ./TestRecipes RECIPE2.CSV
Debug output is sent to command line.

To discriminate whether we compile for Arduino or command line, we use
a trick with precompiler tags (-D). The makefile has the tag -D
sdioVersion, the Arduino uses the tag ESP_PLATFORM.
