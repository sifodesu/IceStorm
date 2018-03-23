#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "C_Rect.h"
#include "Global_Flags.h"


class Map {
public : 
	static void loadLevel(std::string name);			//Load the stream from levelpath

	//static void checkMate();		//prints in the console the current loaded matrix

	static void loadMatrix();	//loads a matrix from a stream

	static bool isItSolid(C_Rect reqt); //checks if the element at the given coord is solid

	static void trigger(C_Rect reqt, int direction, bool contact);

	static int getIdObject(double ay, int iy, double ax, int ix, int plan = 0);
	
	static int getID(int ix, int iy, int plan = 0);

	static void findOccurrence(int charry, double* ix, double* iy);

	static void saveMatrix();
	static void checkMate();
	static bool changed;
	static std::ifstream* currentLevel;	//the stream containing the level
	static std::vector<std::vector<std::vector<int>>> matrix;	//the matrix of the level	|  the pair indexes represent front plans, the odds back plans
	static std::string levelname;

	static int cx;		//the x of the matrix in the stream currentlevel
	static int cy;
};