// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "sgp4.h"
#include <vector>


extern char help;
extern FILE* dbgfile;



//using namespace System;
//using namespace SGP4Funcs;

#define pi 3.14159265358979323846

char typerun, typeinput, opsmode;
gravconsttype  whichconst;
int whichcon;


int main()
//int main(array<System::String ^> ^args)
//int main()
{
    // Initialize TLE elements
    //Lemur2
    char lemurTle1[] = "1 44405U 19038V   24042.52387472  .00272713  00000+0  18454-2 0  9993";
    char LemurTle2[] = "2 44405  97.6524  42.6531 0012607 102.3131 257.9545 15.74144347255650";

    char cosmosTle1[] = "1 49583U 82092BY  22028.28241142  .09357949 -12093-5  26304-2 0  9992";
    char cosmosTle2[] = "2 49583  82.4403  43.5868 0012566 261.5890  98.3978 16.27573958 11208";

    // Parse TLE
    /*
    Typerun specifies whether you want to propagate the orbit for a specified amount of time ('c'),
    propagate until a specified time ('m'), or propagate until a specified altitude is reached ('a').
    */
    char typerun = 'c';

    gravconsttype whichconst = wgs72;
    double startmfe = 0.0; // Start propagation from the TLE epoch
    double stopmfe = 1440.0; // Stop propagation after 1440 minutes (1 day)
    double deltamin = 100.0; // Propagation step size of 1 minute
    elsetrec satrec;
    SGP4Funcs::twoline2rv(line1, line2, typerun, 'c', 'U', whichconst, startmfe, stopmfe, deltamin, satrec);
    // Set time
    //double tsince = 0.0; // Time since epoch in minutes


    int numOfPoints = static_cast<int>((stopmfe - startmfe) / deltamin) + 1;

    // Create time_points array
    std::vector<double> time_points;
    for (int i = 0; i < numOfPoints; ++i) {
        time_points.push_back(startmfe + i * deltamin);
    }

    // Call GetJdAndFrArrayForSattelite to get jd and fr arrays
    double jd1 = satrec.jdsatepoch;
    double fr1 = satrec.jdsatepochF;

    int num_points = static_cast<int>((stopmfe - startmfe) / deltamin) + 1;
    double* timePoints = new double[num_points];

    double r[3], v[3];
    for (int i = 0; i < num_points; ++i) {
        timePoints[i] = startmfe + i * deltamin;
        // Compute position and velocity
        SGP4Funcs::sgp4(satrec, startmfe + i * deltamin, r, v);
        // Output position and velocity
        std::cout << "Position (km): " << r[0] << ", " << r[1] << ", " << r[2] << std::endl;
        std::cout << "Velocity (km/s): " << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
    }

    return 0;
}

std::vector<double> arange(double start, double end, double step) {
    std::vector<double> result;
    for (double value = start; value <= end; value += step) {
        result.push_back(value);
    }
    return result;
}