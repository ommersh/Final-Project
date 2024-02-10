#include "ANCASTests.h"
#include "TestUtils.h"

TEST_F(ANCASCubicPolynomialTests, TEST_Coefficients_creation)
{
    MockANCASCubicPolynomial cubicPolynomial;
    double f[4] = { 0,1,2,3 };
    double Tau[4] = { 0,1.0/3.0,2.0/3.0,1 };
    double expectedResult[4];

    cubicPolynomial.createCoefficients(f, Tau);

    //Eq.1(f) 
    expectedResult[0] = f[0];

    //Eq.1(g)
    expectedResult[1] = 3;

    //Eq.1(h)
    expectedResult[2] = 0;

    //Eq.1(i)
    expectedResult[3] = 0;


    EXPECT_TRUE(TestUtils::CompareValues(cubicPolynomial.coefficients[0], expectedResult[0],maxError));
    EXPECT_TRUE(TestUtils::CompareValues(cubicPolynomial.coefficients[1], expectedResult[1], maxError));
    EXPECT_TRUE(TestUtils::CompareValues(cubicPolynomial.coefficients[2], expectedResult[2], maxError));
    EXPECT_TRUE(TestUtils::CompareValues(cubicPolynomial.coefficients[3], expectedResult[3], maxError));

    //check the GetValue function
    double x, expectedFx,Fx;
    x = 0;
    expectedFx = 0;
    Fx = cubicPolynomial.getValue(x);
    EXPECT_TRUE(TestUtils::CompareValues(expectedFx, Fx, maxError));

    x = 1;
    expectedFx = 3;
    Fx = cubicPolynomial.getValue(x);
    EXPECT_TRUE(TestUtils::CompareValues(expectedFx, Fx, maxError));

    x = 2;
    expectedFx = 6;
    Fx = cubicPolynomial.getValue(x);
    EXPECT_TRUE(TestUtils::CompareValues(expectedFx, Fx, maxError));
}

using ::testing::_;
using ::testing::DoAll; 
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::SetArgReferee;

TEST_F(ANCASTests, TEST_getRootsInInterval_3_roots_in_interval)
{
    double roots[3] = { 0.2,0.3,0.4 };
    double expectedResults[3] = { 0.2,0.3,0.4 };
    double results[3] = { 0 };

    int numberOfRoots = 0,expectedNumberOfRoots = 3;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 3),
            SetArgReferee<5>(3), // Set the number of roots found to 3
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[0], numberOfRoots, maxError));
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[1], numberOfRoots, maxError));
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[2], numberOfRoots, maxError));
}

TEST_F(ANCASTests, TEST_getRootsInInterval_3_roots_1_not_in_interval)
{
    double roots[3] = { 2,0.3,0.4 };
    double expectedResults[2] = { 0.3,0.4 };
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 2;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 3),
            SetArgReferee<5>(3), // Set the number of roots found to 3
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[0], numberOfRoots, maxError));
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[1], numberOfRoots, maxError));
}

TEST_F(ANCASTests, TEST_getRootsInInterval_3_roots_2_not_in_interval)
{
    double roots[3] = { 2,0.3,4 };
    double expectedResults[1] = { 0.3 };
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 1;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 3),
            SetArgReferee<5>(3), // Set the number of roots found to 3
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[0], numberOfRoots, maxError));
}

TEST_F(ANCASTests, TEST_getRootsInInterval_3_roots_3_not_in_interval)
{
    double roots[3] = { 2,3,4 };
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 0;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 3),
            SetArgReferee<5>(3), // Set the number of roots found to 3
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
}

TEST_F(ANCASTests, TEST_getRootsInInterval_2_roots_in_interval)
{
    double roots[2] = { 0.2,0.3};
    double expectedResults[2] = { 0.2,0.3};
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 2;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 2),
            SetArgReferee<5>(2), // Set the number of roots found to 2
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[0], numberOfRoots, maxError));
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[1], numberOfRoots, maxError));
}

TEST_F(ANCASTests, TEST_getRootsInInterval_2_roots_1_not_in_interval)
{
    double roots[2] = { 2,0.3 };
    double expectedResults[1] = { 0.3 };
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 1;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 2),
            SetArgReferee<5>(2), // Set the number of roots found to 2
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[0], numberOfRoots, maxError));
}

TEST_F(ANCASTests, TEST_getRootsInInterval_2_roots_2_not_in_interval)
{
    double roots[2] = { 2,3 };
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 0;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 2),
            SetArgReferee<5>(2), // Set the number of roots found to 2
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
}

TEST_F(ANCASTests, TEST_getRootsInInterval_1_roots_in_interval)
{
    double roots[1] = { 0.2 };
    double expectedResults[2] = { 0.2 };
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 1;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 1),
            SetArgReferee<5>(1), // Set the number of roots found to 2
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
    EXPECT_TRUE(TestUtils::CheckDoubleInArray(results, expectedResults[0], numberOfRoots, maxError));
}

TEST_F(ANCASTests, TEST_getRootsInInterval_1_roots_not_in_interval)
{
    double roots[1] = { 2 };
    double expectedResults[2] = { 0.2 };
    double results[3] = { 0 };

    int numberOfRoots = 0, expectedNumberOfRoots = 0;
    ANCASCubicPolynomial p;
    p.coefficients[0] = 1;
    p.coefficients[1] = 2;
    p.coefficients[2] = 3;
    p.coefficients[3] = 4;

    // Set expectations
    EXPECT_CALL(ancas, calculateCubicRoots(4.0, 3.0, 2.0, 1.0, _, _))
        .WillOnce(DoAll(
            SetArrayArgument<4>(roots, roots + 1),
            SetArgReferee<5>(1), // Set the number of roots found to 2
            Return())); // Since it's a void function, we just use Return()

    numberOfRoots = ancas.testGetRootsInInterval(p, results);

    //test the results
    EXPECT_EQ(numberOfRoots, expectedNumberOfRoots);
}