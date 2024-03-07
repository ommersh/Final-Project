#pragma once
#include "TcaCalculation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


struct sFileData {
	TcaCalculation::sPointData* data = nullptr;
	int size;
};

//constants
const int TIMES_COL = 0;
const int R1X_COL = 1;
const int R1Y_COL = 2;
const int R1Z_COL = 3;
const int V1X_COL = 4;
const int V1Y_COL = 5;
const int V1Z_COL = 6;
const int R2X_COL = 7;
const int R2Y_COL = 8;
const int R2Z_COL = 9;
const int V2X_COL = 10;
const int V2Y_COL = 11;
const int V2Z_COL = 12;

using namespace std;
class FileReader
{
public:
	sFileData readDataFromFile(string filePath);
};
