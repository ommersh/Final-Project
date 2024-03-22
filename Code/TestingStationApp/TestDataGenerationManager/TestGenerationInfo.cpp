#include "TestGenerationInfo.h"

// Constructors
TestGenerationInfo::TestGenerationInfo() = default;

TestGenerationInfo::TestGenerationInfo(const Algorithm& algorithm, const SatelliteDataFormat& tleFormat, const std::string& elem1,
    const std::string& elem2, double timeInterval, const TimeUnit& timeIntervalUnit,
    int iterations, int catchPolynomialDegree, double sgp4TimeStep)
    : algorithm(algorithm), tleFormat(tleFormat), elem1(elem1), elem2(elem2),
    timeInterval(timeInterval), timeIntervalUnit(timeIntervalUnit),
    iterations(iterations), catchPolynomialDegree(catchPolynomialDegree),
    sgp4TimeStep(sgp4TimeStep) {}

// Getters
Algorithm TestGenerationInfo::getAlgorithm() const { return algorithm; }
SatelliteDataFormat TestGenerationInfo::getTleFormat() const { return tleFormat; }
std::string TestGenerationInfo::getElem1() const { return elem1; }
std::string TestGenerationInfo::getElem2() const { return elem2; }
double TestGenerationInfo::getTimeInterval() const { return timeInterval; }
TimeUnit TestGenerationInfo::getTimeIntervalUnit() const { return timeIntervalUnit; }
int TestGenerationInfo::getIterations() const { return iterations; }
int TestGenerationInfo::getCatchPolynomialDegree() const { return catchPolynomialDegree; }
double TestGenerationInfo::getSgp4TimeStep() const { return sgp4TimeStep; }

// Setters
void TestGenerationInfo::setAlgorithm(const Algorithm& algorithm) { this->algorithm = algorithm; }
void TestGenerationInfo::setTleFormat(const SatelliteDataFormat& tleFormat) { this->tleFormat = tleFormat; }
void TestGenerationInfo::setElem1(const std::string& elem1) { this->elem1 = elem1; }
void TestGenerationInfo::setElem2(const std::string& elem2) { this->elem2 = elem2; }
void TestGenerationInfo::setTimeInterval(double timeInterval) { this->timeInterval = timeInterval; }
void TestGenerationInfo::setTimeIntervalUnit(const TimeUnit& timeIntervalUnit) { this->timeIntervalUnit = timeIntervalUnit; }
void TestGenerationInfo::setIterations(int iterations) { this->iterations = iterations; }
void TestGenerationInfo::setCatchPolynomialDegree(int catchPolynomialDegree) { this->catchPolynomialDegree = catchPolynomialDegree; }
void TestGenerationInfo::setSgp4TimeStep(double sgp4TimeStep) { this->sgp4TimeStep = sgp4TimeStep; }