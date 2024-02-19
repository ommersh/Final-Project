#ifndef Ancas_Data_Generator_H
#define Ancas_Data_Generator_H

#include "DataGenerator.h"

class AncasDataGenerator: public DataGenerator {
public:
	double* GenerateTimePoint(int n, double tEnd, double gamma) override;
};


#endif