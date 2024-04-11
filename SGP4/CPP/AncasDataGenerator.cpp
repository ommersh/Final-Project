#include <math.h>
#include "AncasDataGenerator.h"
#include <TcaCalculation.h>
#define M_PI 3.14159265358979323846

void AncasDataGenerator::GenerateTimePoint(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[])
{
    elementsVectors[0].time = 0;
    double Ancas_jump = gamma / (n - 1);
    double a = 0;
    double b = gamma;
    int index = 1;
    while (b < tEnd) {
        for (int i = 1; i < n; ++i) {
            elementsVectors[index++].time = a + i * Ancas_jump;
        }
        a = b;
        b = b + gamma;
    }
}
