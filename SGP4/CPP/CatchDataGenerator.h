#ifndef Catch_Data_Generator_H
#define Catch_Data_Generator_H

class CatchDataGenerator {
public:
	double* GenerateTimePoint(int n, double tEnd, double gamma);
private:
	double* calculateXforN(int n, double Gamma);
	double calculateXj(double a, double b, int j, int n);
};


#endif