#ifndef TESTGENERATIONINFO_H
#define TESTGENERATIONINFO_H

#include <string>
#include "../Enums/TleFormatEnum.h"
#include "../Enums/TimeUnitEnum.h"
#include "AlgorithmEnum.h"

class TestGenerationInfo {
public:
    // Constructors
    TestGenerationInfo();
    TestGenerationInfo(const Algorithm& algorithm, const TLEFormat& tleFormat, const std::string& elem1,
        const std::string& elem2, double timeInterval, const TimeUnit& timeIntervalUnit,
        int iterations, int catchPolynomialDegree, double sgp4TimeStep);

    // Getters
    Algorithm getAlgorithm() const;
    TLEFormat getTleFormat() const;
    std::string getElem1() const;
    std::string getElem2() const;
    double getTimeInterval() const;
    TimeUnit getTimeIntervalUnit() const;
    int getIterations() const;
    int getCatchPolynomialDegree() const;
    double getSgp4TimeStep() const;

    // Setters
    void setAlgorithm(const Algorithm& algorithm);
    void setTleFormat(const TLEFormat& tleFormat);
    void setElem1(const std::string& elem1);
    void setElem2(const std::string& elem2);
    void setTimeInterval(double timeInterval);
    void setTimeIntervalUnit(const TimeUnit& timeIntervalUnit);
    void setIterations(int iterations);
    void setCatchPolynomialDegree(int catchPolynomialDegree);
    void setSgp4TimeStep(double sgp4TimeStep);

private:
    Algorithm algorithm;
    TLEFormat tleFormat;
    std::string elem1;
    std::string elem2;
    double timeInterval;
    TimeUnit timeIntervalUnit;
    int iterations;
    int catchPolynomialDegree;
    double sgp4TimeStep;
};

#endif