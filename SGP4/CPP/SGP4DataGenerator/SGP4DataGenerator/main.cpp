// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "sgp4.h"
#include <vector>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "DataGenerator.h"
#include "main.h"
#include "AncasDataGenerator.h"
using namespace std;


int main() {
    auto element1a = new elsetrec();
    auto element1b = new elsetrec();
    std::string xmlPath = "C:/LEMUR2Philary.xml";
    auto dataGen = new AncasDataGenerator();

    char tle1[] = "1 55013U 23001E   24049.69635494  .00021483  00000+0  64507-3 0  9997";
    char tle2[] = "2 55013  97.4413 114.2250 0005503 213.0707 147.0193 15.34443181 62694";

    dataGen->InitOrbitalElementsFromTLE(tle1, tle2, *element1a);
    cout << "Print tle parameters";
    //cout << "intldesg: " << element1a->intldesg << endl;
    cout << "epochyr: " << element1a->epochyr << endl;
    cout << "epochdays: " << element1a->epochdays << endl;
    cout << "ndot: " << element1a->ndot << endl;
    cout << "nddot: " << element1a->nddot << endl;
    cout << "bstar: " << element1a->bstar << endl;
    cout << "ephtype: " << element1a->ephtype << endl;
    cout << "elnum: " << element1a->elnum << endl;

    cout << "satnum: " << element1a->satnum << endl;
    cout << "inclo: " << element1a->inclo << endl;
    cout << "nodeo: " << element1a->nodeo << endl;
    cout << "ecco: " << element1a->ecco << endl;
    cout << "argpo: " << element1a->argpo << endl;
    cout << "mo: " << element1a->mo << endl;
    cout << "no_kozai: " << element1a->no_kozai << endl;
    cout << "revnum: " << element1a->revnum << endl;

    //printf_s(element1a->satnum, element1a->inclo, element1a->nodeo, element1a->ecco, element1a->argpo, element1a->mo, element1a->no_kozai, element1a->revnum);
    cout << "\n";

    cout << "Print Xml parameters";

    dataGen->InitOrbitalElementsFromXml(xmlPath, *element1a);

    //cout << "intldesg: " << element1a->intldesg << endl;
    //cout << "epochyr: " << element1a->epochyr << endl;
    cout << "epochdays: " << element1a->epochdays << endl;
    cout << "ndot: " << element1a->ndot << endl;
    cout << "nddot: " << element1a->nddot << endl;
    cout << "bstar: " << element1a->bstar << endl;
    cout << "ephtype: " << element1a->ephtype << endl;
    cout << "elnum: " << element1a->elnum << endl;

    cout << "satnum: " << element1a->satnum << endl;
    cout << "inclo: " << element1a->inclo << endl;
    cout << "nodeo: " << element1a->nodeo << endl;
    cout << "ecco: " << element1a->ecco << endl;
    cout << "argpo: " << element1a->argpo << endl;
    cout << "mo: " << element1a->mo << endl;
    cout << "no_kozai: " << element1a->no_kozai << endl;
    cout << "revnum: " << element1a->revnum << endl;

    //printf_s(element1a->intldesg, element1a->epochyr, element1a->epochdays, element1a->ndot,  element1a->nddot, element1a->bstar, element1a->ephtype, element1a->elnum);
    //printf_s(element1a->satnum, element1a->inclo, element1a->nodeo, element1a->ecco, element1a->argpo, element1a->mo, element1a->no_kozai, element1a->revnum);

    delete dataGen, element1a, element1b;

    return 0;
}
