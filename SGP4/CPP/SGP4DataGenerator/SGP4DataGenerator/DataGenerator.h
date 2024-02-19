#include "SGP4.h"
#ifndef Time_Points_Generator_H
#define Time_Points_Generator_H

class DataGenerator{
public:
	virtual double* GenerateTimePoint(int n, double tEnd, double gamma) = 0;
	void GenearateDiffVectorFor2OrbitalElementsCSV(double* timeInMinutes, int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, std::string& fileName); 
	void InitOrbitalElementsFromXml(std::string& xmlFile, elsetrec& satrec);
	void InitOrbitalElementsFromTLE(char longstr1[130], char longstr2[130], elsetrec& satrec);
	std::string GetDataFromXmlNode(std::string xmlContent, std::string nodeName);
	double GetEpochDayInYear(int year, int month, int day, int hour, int minute, int second, double fraction);
	void saveDataInCSVFile(double* time_points, double** r1, double** v1, double** r2, double** v2, double* f, double* df, int length, std::string& name);
	//todo: add static
	//todo: add the other funcs
};

#endif