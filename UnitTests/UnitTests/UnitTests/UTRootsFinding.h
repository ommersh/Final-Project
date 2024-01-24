#include <gtest/gtest.h>
#include "ANCAS.h"

bool CheckDoubleInArray(double* array, double value, int arraySize,double errSize)
{
    bool result = false;
    if (nullptr != array)
    {
        for (int i = 0; i < arraySize; i++)
        {
            if (abs(array[i] - value) <= errSize)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}

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