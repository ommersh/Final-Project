#include "SGP4.h"
#include "CatchDataGenerator.h"
#include "AncasDataGenerator.h"
#ifndef Time_Points_Generator_H
#define Time_Points_Generator_H

class DataGenerator {
public:
	DataGenerator()
	{
		m_ancasDataGenerator = AncasDataGenerator();
		m_catchDataGenerator = CatchDataGenerator();
	}
	void GenearateDiffVectorFor2OrbitalElementsCSV(double* timeInMinutes, int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, std::string& fileName);
	void CalculateRelativeVectorsForTwoElements(double* timeInMinutes, int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, double** r1Arr, double** r2Arr,
		double** v1Arr, double** v2Arr, double* f, double* df);
	void InitOrbitalElementsFromXml(std::string& xmlFile, elsetrec& satrec);
	void InitOrbitalElementsFromTLE(char longstr1[130], char longstr2[130], elsetrec& satrec);
	std::string GetDataFromXmlNode(std::string xmlContent, std::string nodeName);
	double GetEpochDayInYear(int year, int month, int day, int hour, int minute, int second, double fraction);
	void saveDataInCSVFile(double* time_points, double** r1, double** v1, double** r2, double** v2, double* f, double* df, int length, std::string& name);
	double* GenerateTimePointForAncas(int n, double tEnd, double gamma);
	double* GenerateTimePointForCatch(int n, double tEnd, double gamma);

	//todo: add static
	//todo: add the other funcs

private:
	AncasDataGenerator m_ancasDataGenerator;
	CatchDataGenerator m_catchDataGenerator;

};
#endif