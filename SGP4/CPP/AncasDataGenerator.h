#ifndef Ancas_Data_Generator_H
#define Ancas_Data_Generator_H

#include <TcaCalculation.h>

class AncasDataGenerator {
public:
	void GenerateTimePoint(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[]);
};


#endif