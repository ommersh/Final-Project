#include "CatchTests.h"
#include "TestUtils.h"



TEST_F(CPPTests, TEST_interpolation_matrix_when_N_is_2)
{
    //The interpolation matrix is define as
    //M(j,k) = 2 / PjPkN * cos (j pi k / N)
    // PjPk =:
    //      4 on the N,N/0,0/N,0/0,N values
    //      2 on the N/0 row or column
    //      1 else
    // we will test a low degree matrix
    // for easier calculations
    // the Matrix is N+1xN+1
    // so for N = 2
    // We will have a 3x3 matrix:
    // 
    // [ 1 / 4   , 1 / 2            , 1 / 4             ]   [ 1 / 4   , 1 / 2   , 1 / 4     ]
    // [ 1 / 2   , cos ( pi / 2)    , cos ( pi ) / 2    ] = [ 1 / 2   , 0       , - 1 / 2   ]
    // [ 1 / 4   , cos ( pi ) / 2   , cos (2 pi ) / 4   ]   [ 1 / 4   , - 1 / 2 , 1 / 4     ]
    double expectedResults[3][3] = {
            {0.25   , 0.5    , 0.25     },
            {0.5    , 0      , -0.5     },
            {0.25   , -0.5   , 0.25     }
    };
    // the interpulation matrix is calculated in the constructor
    MockCpp cpp;
    cpp.Init(2);
    //get the resulted matrix
    double interpolationMatrix[CATCH_MAX_DEGREE + 1][CATCH_MAX_DEGREE + 1];
    cpp.getInterpulationMatrix(interpolationMatrix);

    //compare the result
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            double value = interpolationMatrix[i][j];
            double expectedValue = expectedResults[i][j];
            EXPECT_TRUE(TestUtils::CompareValues(value, expectedValue, maxError));
        }
    }
    
}


TEST_F(CPPTests, TEST_Fitting_the_CPP)
{
    //the interpulation matrix for N = 2 is:
    // [ 1 / 4   , 1 / 2   , 1 / 4     ]  
    // [ 1 / 2   , 0       , - 1 / 2   ] 
    // [ 1 / 4   , - 1 / 2 , 1 / 4     ]   
    // the interpulation matrix is calculated in the constructor
    MockCpp cpp;
    cpp.Init(2);
    
    double gx1[3] = { 1,0,0 };
    double expectedResults1[3] = { 0.25, -0.5, 0.25 } ;

    cpp.fitCPP(0, 1, gx1);
    //compare the result
    for (int i = 0; i <= 2; i++)
    {
        double value = cpp.coefficients[i];
        double expectedValue = expectedResults1[i];
        EXPECT_TRUE(TestUtils::CompareValues(value, expectedValue, maxError));
    }

    double gx2[3] = { 0,1,0 };
    double expectedResults2[3] = { 0.5, 0, -0.5 };

    cpp.fitCPP(0, 1, gx2);
    //compare the result
    for (int i = 0; i <= 2; i++)
    {
        double value = cpp.coefficients[i];
        double expectedValue = expectedResults2[i];
        EXPECT_TRUE(TestUtils::CompareValues(value, expectedValue, maxError));
    }

    double gx3[3] = { 0,0,1 };
    double expectedResults3[3] = { 0.25, 0.5, 0.25 };

    cpp.fitCPP(0, 1, gx3);
    //compare the result
    for (int i = 0; i <= 2; i++)
    {
        double value = cpp.coefficients[i];
        double expectedValue = expectedResults3[i];
        EXPECT_TRUE(TestUtils::CompareValues(value, expectedValue, maxError));
    }

}


TEST_F(CPPTests, TEST_GetValue)
{
    //the interpulation matrix for N = 2 is:
    // [ 1 / 4   , 1 / 2   , 1 / 4     ]  
    // [ 1 / 2   , 0       , - 1 / 2   ] 
    // [ 1 / 4   , - 1 / 2 , 1 / 4     ]   
    // the interpulation matrix is calculated in the constructor
    MockCpp cpp;
    cpp.Init(2);

    double gx1[3] = { 1,0,0 };
    double expectedResults1[3] = { 0.25, -0.5, 0.25 };

    cpp.fitCPP(0, 1, gx1);
    //compare the result
    for (int i = 0; i <= 2; i++)
    {
        double value = cpp.coefficients[i];
        double expectedValue = expectedResults1[i];
        EXPECT_TRUE(TestUtils::CompareValues(value, expectedValue, maxError));
    }

    //  so the coefficients are 1/4,-1/2,1/4
    //  to get the value at a point x
    //  we use Eq.10
    //  g(x) =sum fron j = 0 to N of
    //  aj * Tj( (2x - ( a + b) ) / ( b - a ) )
    //  and Tj(y) = cos ( j * arccos(y)
    // in our case:
    // T0(y) = cos(0) = 1

    //for x = 0.5:
    double x = 0.5;
    //  for (2x - ( a + b) ) / ( b - a ) )
    //  we get the value 1-1/1
    //  so we will get T0 = 1 + T1(0) + T2(0)
    //  arc cos 0 = pi/2
    //  cos(pi/2) = 0, cos(2pi/2)=-1
    double expectedResults = 0.25*1 + -0.5*0 + 0.25*( - 1);
    double results = cpp.getValue(x);
    EXPECT_TRUE(TestUtils::CompareValues(results, expectedResults, maxError));

    //lets test another case:
    //
    x = 0.25;
    //  for (2x - ( a + b) ) / ( b - a ) )
    //  we get the value 0.5 - 1 = -0.5
    //  so we will get T0 = 1 + T1(-0.5) + T2(-0.5)
    //  arc cos -0.5 = 2pi/3
    //  cos(2pi/3) = -0.5, cos(4pi/3) = -0.5
    expectedResults = 0.25 * 1 + -0.5 * (-0.5) + 0.25 * (-0.5);
    results = cpp.getValue(x);
    EXPECT_TRUE(TestUtils::CompareValues(results, expectedResults, maxError));

}


#include "FileReader.h"
#include "CompanionMatrixRootsFinderEigen.h"
#include "SimpleDataGeneration.h"

//Test a real test case
TEST_F(CATCHTestCase, TEST_test_case_STARLINK5447_UNICORN2N_Eigen_Lib)
{
    TcaCalculation::sPointData* data;
    elsetrec elsetrec1;
    elsetrec elsetrec2;
    double startTime1Min;
    double startTime2Min;
    SimpleDataGeneration SimpleDataGeneration;
    double maxErrorTime = 2;
    double maxErrorDistance = 0.0001;
    int degree = 15;
    //run catch
    double expectedResultsDistance = 0.0118;
    double expectedResultsTime = 577578;


    CompanionMatrixRootsFinderEigen rootsFinder;
    CATCH Catch;
    Catch.Init(&rootsFinder, degree);

    char Obj1le1[] = "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993";
    char Obj1le2[] = "2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489";

    char Obj2le1[] = "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990";
    char Obj2le2[] = "2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268";
    elsetrec1 = { 0 };
    elsetrec2 = { 0 };
    startTime1Min = 0;
    startTime2Min = 0;
    double segmentSize;
    SimpleDataGeneration.GenearateDataFromTle(Obj1le1, Obj1le2, Obj2le1, Obj2le2, 14, 16, elsetrec1, elsetrec2, startTime1Min, startTime2Min, segmentSize);


    int size = SimpleDataGeneration.m_numberOfPoints;
    data = SimpleDataGeneration.m_pointsDataCATCH;

    if (data != nullptr)
    {
        TCA tca = Catch.RunAlgorithm(data, size-1);
        delete[] SimpleDataGeneration.m_pointsDataANCAS;
        delete[] SimpleDataGeneration.m_pointsDataCATCH;

        EXPECT_TRUE(TestUtils::CompareValues(tca.distance, expectedResultsDistance, maxErrorDistance));
        EXPECT_TRUE(TestUtils::CompareValues(tca.time, expectedResultsTime, maxErrorTime));

    }
    else
    {
        //if we got here we couldnt read the input file
        //and the test failed
        EXPECT_TRUE(false);
    }
}