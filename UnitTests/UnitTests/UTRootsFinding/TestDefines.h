#ifndef SHIELD_TestDefines_H    // Check if the symbol SHIELD_TestDefines_H is not defined
#define SHIELD_TestDefines_H    // Define the symbol SHIELD_TestDefines_H

#include "gtest/gtest.h"
#include "ANCAS.h"




class MockANCAS : public ANCAS
{
public:
    MockANCAS()
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


#endif //SHIELD_TestDefines_H