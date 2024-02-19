#include <math.h>
#include "AncasDataGenerator.h"
#define M_PI 3.14159265358979323846

double* AncasDataGenerator::GenerateTimePoint(int n, double tEnd, double gamma)
{
    double* timePoints = new double[n * (static_cast<int>(tEnd / gamma) + 1)];
    timePoints[0] = 0;
    double Ancas_jump = gamma / (n - 1);
    double a = 0;
    double b = gamma;
    int index = 1;
    while (b < tEnd) {
        for (int i = 1; i < n; ++i) {
            timePoints[index++] = a + i * Ancas_jump;
        }
        a = b;
        b = b + gamma;
    }
    return timePoints;
}
