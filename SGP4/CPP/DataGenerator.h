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
	void GenearateDiffVectorFor2OrbitalElementsCSV(int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, std::string& fileName, TcaCalculation::sPointData elementsVectors[], double& startTime1, double& startTime2);
	void CalculateRelativeVectorsForTwoElements(int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, TcaCalculation::sPointData elementsVectors[], double& startTime1, double& startTime2);
	void InitOrbitalElementsFromXml(std::string& xmlFile, elsetrec& satrec);
	void InitOrbitalElementsFromTLE(char longstr1[130], char longstr2[130], elsetrec& satrec);
	std::string GetDataFromXmlNode(std::string xmlContent, std::string nodeName);
	double GetEpochDayInYear(int year, int month, int day, int hour, int minute, int second, double fraction);
	void saveDataInCSVFile(int length, std::string& name, TcaCalculation::sPointData elementsVectors[]);
	void GenerateTimePointForAncas(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
	void GenerateTimePointForCatch(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
	static double GetGamma(elsetrec elsetrec1, elsetrec elsetrec2, double factor);
	void GetStartTimeOfOrbElem(elsetrec elsetrec1, elsetrec elsetrec2, double& elemOneTime, double& ElemTwoTime);
	//todo: add static

private:
	AncasDataGenerator m_ancasDataGenerator;
	CatchDataGenerator m_catchDataGenerator;

};
#endif