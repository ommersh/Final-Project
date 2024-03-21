#ifndef SHIELD_SimpleDataGeneration_H    // Check if the symbol SHIELD_SimpleDataGeneration_H is not defined
#define SHIELD_SimpleDataGeneration_H    // Define the symbol SHIELD_SimpleDataGeneration_H

#include "TcaCalculation.h"
#include "SGP4.h"
#include "DataGenerator.h"

class SimpleDataGeneration
{
private:
    DataGenerator m_dataGenerator;
public:
    TcaCalculation::sPointData* m_pointsDataANCAS;
    TcaCalculation::sPointData* m_pointsDataCATCH;
    int m_numberOfPoints;

    const double PI = 3.14159265358979323846;
    const double seconds_in_day = 86400.0;

    void GenearateDataFromTle(char* longstr1Obj1, char* longstr2bj1, char* longstr1Obj2, char* longstr2bj2,
        int numberOfDays, int numberOfPointsInSegment, elsetrec& elsetrec1, elsetrec& elsetrec2, double& startDataElem1, double& startDataElem2);

    void GenearateDataFromElsetrec(int numberOfDays, int numberOfPointsInSegment, elsetrec& elsetrec1, elsetrec& elsetrec2, double& startDataElem1, double& startDataElem2);


    TcaCalculation::TCA FindTcaWithSmallTimeStep(int numberOfDays, elsetrec& elsetrec1, elsetrec& elsetrec2, double& startDataElem1, double& startDataElem2,double stepSize);
    TcaCalculation::TCA FindTcaWithSmallTimeStepArountPoint(elsetrec& elsetrec1, elsetrec& elsetrec2, double& startDataElem1, double& startDataElem2, double stepSize, double timePoint,double segmentSize);

private:
    void GenerateTimePointsForANCAS(int numberOfPointsInSegment, double tEnd, double gamma, int numberOfPoints);
    void GenerateTimePointsForCatch(int numberOfPointsInSegment, double tEnd, double gamma, int numberOfPoints);
    double* calculateXforN(int n, double Gamma);
    double calculateXj(double a, double b, int j, int n);  
};



#endif //SHIELD_SimpleDataGeneration_H