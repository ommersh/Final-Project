#ifndef Catch_Data_Generator_H
#define Catch_Data_Generator_H
#include <TcaCalculation.h>

class CatchDataGenerator {
public:
	void GenerateTimePoint(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
private:
	double* calculateXforN(int n, double Gamma);
	double calculateXj(double a, double b, int j, int n);
};


#endif