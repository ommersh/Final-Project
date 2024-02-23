#ifndef SHIELD_TestDefinitions_H    // Check if the symbol SHIELD_TestDefinitions_H is not defined
#define SHIELD_TestDefinitions_H    // Define the symbol SHIELD_TestDefinitions_H

#include "TCA_Calculation.h"

namespace TestParameters
{
    enum CatchRootsAlg {
        EigenCompanionMatrix
    };

    enum Algorithm {
        CATCH,
        ANCAS
    };

    /// <summary>
    /// Parameters for running a test with a TCA finding algorithm
    /// </summary>
    struct TestParams {
        int degree;                         // Degree of the polynomial
        CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
        Algorithm testedAlgorithm;

        // Constructor for easy initialization
        TestParams(int deg, CatchRootsAlg algType, Algorithm testAlg)
            : degree(deg), catchRootsAlg(algType), testedAlgorithm(testAlg) {}
    };
}


namespace TestResults
{
   
    struct TestResult {
        TcaCalculation::TCA tca;
        double runTimeMicro;
    };
}











#endif //SHIELD_TestDefinitions_H