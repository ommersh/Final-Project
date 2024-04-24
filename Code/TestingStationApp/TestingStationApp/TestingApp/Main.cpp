#include "../../Database/DatabaseManager.h"
#include "Lab.h"
#include "Utilities.h"

void printTestInfo(const TestInfo& test) {
    //std::cout << "Name: " << test.name << std::endl;
    std::cout << "Format: " << int(test.format) << std::endl;
    std::cout << "Time Interval: " << test.recipe.timeIntervalSizeSec << std::endl;
    std::cout << "Iterations: " << test.recipe.numberOfIterations<< std::endl;
    std::cout << "Algorithm: " << int(test.recipe.testedAlgorithm) << std::endl;
    std::cout << "Catch Polynom Degree: " << test.recipe.catchPolynomialDegree << std::endl;
    //std::cout << "First Element Data: " << test.firstElemData << std::endl;
    //std::cout << "Second Element Data: " << test.secondElemData << std::endl;
    std::cout << "Number of Time Points: " << test.recipe.numberOfPoints << std::endl;
    std::cout << "Real TCA: " << test.realTCA << std::endl;
    std::cout << "Real Distance: " << test.realDistance << std::endl;
    std::cout << "Distance: " << test.distance << std::endl;
    std::cout << "TCA: " << test.TCA << std::endl;
    std::cout << "Run Time: " << test.runTime << std::endl;
    std::cout << "Status: " << int(test.status) << std::endl;
}


int main()
{

    Lab& lab = Lab::GetInstance();
    while (1)
    {
    //Initialize the data
    TestInfo info = { 0 };
    info.format = SatelliteDataFormat::Text;
    strcpy_s(info.name, "STARLINK5447_UNICORN2N");
    info.status = TestStatus::InProgress;
    /*
STARLINK5447_UNICORN2N

    char Obj1le1[] = "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993";
    char Obj1le2[] = "2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489";

    char Obj2le1[] = "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990";
    char Obj2le2[] = "2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268";
    */
    strcpy_s(info.firstElemData, "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993\n2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489");
    strcpy_s(info.secondElemData, "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990\n2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268");
    strcpy_s(info.recipe.testName, "STARLINK5447_UNICORN2N");
    info.recipe.catchPolynomialDegree = 15;
    info.recipe.numberOfPointsPerSegment = 16;
    info.recipe.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
    info.recipe.numberOfIterations = 10;
    info.recipe.TminFactor = 2;
    info.recipe.timeIntervalSizeSec = 28 * 24 * 60 * 60;//A Week
    info.recipe.TOLd = 10e-9;
    info.recipe.TOLt = 10e-5;
    
    //set the algorithm
    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::ANCAS;
    //Run the test
    lab.CreateTest(info);

    //set the algorithm
    //info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::CATCH;
    //Run the test
    //lab.CreateTest(info);
    /*
    //set the algorithm
    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS;
    //Run the test
    lab.CreateTest(info);
    */



        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    while (1) { ; }
    return 0;
}
