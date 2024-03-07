#ifndef SHIELD_ANCASTests_H    // Check if the symbol SHIELD_ANCASTests_H is not defined
#define SHIELD_ANCASTests_H    // Define the symbol SHIELD_ANCASTests_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ANCAS.h"




class MockANCASCubicPolynomial : public ANCASCubicPolynomial
{
public:
    MockANCASCubicPolynomial()
    {

    }
   

};

class MockANCAS : public ANCAS
{
public:
    MockANCAS()
    {

    }
    int testGetRootsInInterval(ANCASCubicPolynomial P, double result[3])
    {
        return getRootsInInterval(P, result);
    }
    MOCK_METHOD(void, calculateCubicRoots, (double a, double b, double c, double d,
        double* roots, int& numberOfRoots));

};

class ANCASCubicPolynomialTests : public ::testing::Test {
protected:
    double maxError  = 0.0001;
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

class ANCASTests : public ::testing::Test {
protected:
    double maxError = 0.0001;
    MockANCAS ancas;
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

class ANCASTestCase : public ::testing::Test {
protected:
    ANCAS ancas;
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};
#endif //SHIELD_ANCASTests_H