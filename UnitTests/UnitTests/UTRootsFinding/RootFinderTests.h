#ifndef SHIELD_RootFinderTests_H    // Check if the symbol SHIELD_RootFinderTests_H is not defined
#define SHIELD_RootFinderTests_H    // Define the symbol SHIELD_RootFinderTests_H

#include "gtest/gtest.h"
#include "ANCAS.h"




class MockANCAS4CubicRoots : public ANCAS
{
public:
    MockANCAS4CubicRoots()
    {

    }
    void UTcalculateCubicRoots(double a, double b, double c, double d, double* roots, int& numberOfRoots)
    {
        calculateCubicRoots(a, b, c, d, roots, numberOfRoots);
    }

};

class RootFindingTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};


#endif //SHIELD_RootFinderTests_H