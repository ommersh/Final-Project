#include "CatchDataGenerator.h"
#include <math.h>
#define M_PI 3.14159265358979323846


double* CatchDataGenerator::GenerateTimePoint(int n, double tEnd, double gamma)
{
    double* timePoints = new double[n * (static_cast<int>(tEnd / gamma) + 1)];
    timePoints[0] = 0;
    double* catchPoints = calculateXforN(n, gamma);
    double a = 0;
    double b = gamma;
    int index = 1;
    while (b < tEnd) {
        for (int i = 1; i < n; ++i) {
            timePoints[index++] = a + catchPoints[i];
        }
        a = b;
        b = b + gamma;
    }
    return timePoints;

}

double* CatchDataGenerator::calculateXforN(int n, double Gamma)
{
    double* times = new double[n];
    for (int i = 0; i < n; ++i) {
        int j = n - 1 - i;
        times[i] = calculateXj(0, Gamma, j, n);
    }
    return times;
}

double CatchDataGenerator::calculateXj(double a, double b, int j, int n)
{
    double x = ((b - a) / 2) * cos(M_PI * j / (n - 1)) + (b + a) / 2;
    return x;
}
