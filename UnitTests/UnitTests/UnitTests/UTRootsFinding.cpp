#include "UTRootsFinding.h"


//Tests for the ANCAS roots finding - 3rd degree polynomials

TEST_F(RootFindingTests, TEST_3_Real_Solutions) 
{
    MockANCAS rootsFinder;
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
    EXPECT_EQ(true, CheckDoubleInArray(roots, expectedRoot1, numberOfRoots, 0));
    EXPECT_EQ(true, CheckDoubleInArray(roots, expectedRoot2, numberOfRoots, 0));
    EXPECT_EQ(true, CheckDoubleInArray(roots, expectedRoot3, numberOfRoots, 0));
}
