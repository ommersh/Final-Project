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
    MockCpp cpp(2);
    //get the resulted matrix
    double** interpulationMatrix = cpp.getInterpulationMatrix();

    //compare the result
    for (int i = 0; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            double value = interpulationMatrix[i][j];
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
    MockCpp cpp(2);
    
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
    MockCpp cpp(2);

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
#include "CompanionMatrixRootsFinder.h"

//Test a real test case
TEST_F(CATCHTestCase, TEST_test_case_LEMUR2_COSMOS)
{
    double maxErrorTime = 2;
    double maxErrorDistance = 0.0001;
    int degree = 15;
    //run catch
    FileReader fr;
    sFileData fileData = fr.readDataFromFile("TestCaseData/LEMUR2_COSMOS_GAUSS.csv");
    if (fileData.size == -1)
    {
        fileData = fr.readDataFromFile("../UTCatch/TestCaseData/LEMUR2_COSMOS_GAUSS.csv");
    }
    double* timePoints = fileData.timePoints;
    int lastPointIndex = fileData.size;
    double expectedResultsDistance = 0.0838348;
    double expectedResultsTime = 177096;

    CompanionMatrixRootsFinder rootsFinder(degree);
    CATCH Catch(&rootsFinder, degree);

    if (fileData.data != nullptr)
    {
        TCA tca = Catch.runAlgorithm(fileData.data, timePoints, lastPointIndex);
        delete[] fileData.data, fileData.timePoints;
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