#include "RootFinderTests.h"
#include "TestUtils.h"
#include <iostream>


//Tests for the ANCAS roots finding - 3rd degree polynomials
static const double MAX_ERROR = 0.0001;

TEST_F(RootFindingTests, TEST_3_Real_Solutions)
{
    MockANCAS4CubicRoots rootsFinder;
    //for a polynomial ax^3 + bx^2 + cx + d
    //P(x) = (x+1)(x−0)(x−2)
    //Roots: -1 0 2
    //P(x) = x^3 −x^2 −2x
    double a = 1;
    double b = -1;
    double c = -2;
    double d = 0;
    double roots[3];
    int numberOfRoots = 0;
    rootsFinder.UTcalculateCubicRoots(a, b, c, d, roots, numberOfRoots);
    //check the expedted result
    double expectedRoot1 = -1;
    double expectedRoot2 = 0;
    double expectedRoot3 = 2;
    double expectedNumberOfRoot = 3;

    EXPECT_EQ(expectedNumberOfRoot, numberOfRoots);
    EXPECT_EQ(true, TestUtils::CheckDoubleInArray(roots, expectedRoot1, numberOfRoots, MAX_ERROR));
    EXPECT_EQ(true, TestUtils::CheckDoubleInArray(roots, expectedRoot2, numberOfRoots, MAX_ERROR));
    EXPECT_EQ(true, TestUtils::CheckDoubleInArray(roots, expectedRoot3, numberOfRoots, MAX_ERROR));
}

TEST_F(RootFindingTests, TEST_2_Real_Solutions)
{
    MockANCAS4CubicRoots rootsFinder;
    //for a polynomial ax^3 + bx^2 + cx + d
    //P(x)=(x−1)^2(x + 2)
    //Roots: 1,-2
    //P(x)=x^3 −3x + 2
    double a = 1;
    double b = 0;
    double c = -3;
    double d = 2;
    double roots[3];
    int numberOfRoots = 0;
    rootsFinder.UTcalculateCubicRoots(a, b, c, d, roots, numberOfRoots);
    //check the expedted result
    double expectedRoot1 = 1;
    double expectedRoot2 = -2;
    double expectedNumberOfRoot = 2;

    EXPECT_EQ(expectedNumberOfRoot, numberOfRoots);
    EXPECT_EQ(true, TestUtils::CheckDoubleInArray(roots, expectedRoot1, numberOfRoots, MAX_ERROR));
    EXPECT_EQ(true, TestUtils::CheckDoubleInArray(roots, expectedRoot2, numberOfRoots, MAX_ERROR));
}


TEST_F(RootFindingTests, TEST_1_Real_Solutions)
{
    MockANCAS4CubicRoots rootsFinder;
    //for a polynomial ax^3 + bx^2 + cx + d
    //Roots: 1
    //P(x)=x^3 −1
    double a = 1;
    double b = 0;
    double c = 0;
    double d = -1;
    double roots[3];
    int numberOfRoots = 0;
    rootsFinder.UTcalculateCubicRoots(a, b, c, d, roots, numberOfRoots);
    //check the expedted result
    double expectedRoot1 = 1;
    double expectedNumberOfRoot = 1;

    EXPECT_EQ(expectedNumberOfRoot, numberOfRoots);
    EXPECT_EQ(true, TestUtils::CheckDoubleInArray(roots, expectedRoot1, numberOfRoots, MAX_ERROR));
}
