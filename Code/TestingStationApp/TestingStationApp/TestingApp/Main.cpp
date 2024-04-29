#include "../../Database/DatabaseManager.h"
#include "Lab.h"
#include "Utilities.h"

void printTestInfo(const TestInfo& test) {
    std::cout << "ID: " << test.recipe.testID << std::endl;
    std::cout << "Name: " << test.recipe.testName << std::endl;
    std::cout << "Format: " << int(test.format) << std::endl;
    std::cout << "Time Interval: " << test.recipe.timeIntervalSizeSec << std::endl;
    std::cout << "Iterations: " << test.recipe.numberOfIterations<< std::endl;
    std::cout << "Algorithm: " << int(test.recipe.testedAlgorithm) << std::endl;
    std::cout << "Catch Polynom Degree: " << test.recipe.catchPolynomialDegree << std::endl;
    std::cout << "numberOfPointsPerSegment: " << test.recipe.numberOfPointsPerSegment << std::endl;

    //std::cout << "First Element Data: " << test.firstElemData << std::endl;
    //std::cout << "Second Element Data: " << test.secondElemData << std::endl;
    std::cout << "Number of Time Points: " << test.recipe.numberOfPoints << std::endl;
    std::cout << "Real TCA: " << test.realTCA << std::endl;
    std::cout << "Real Distance: " << test.realDistance << std::endl;
    std::cout << "timeOfTcaFromStartingPointSec: " << test.timeOfTcaFromStartingPointSec << std::endl;
    std::cout << "distanceOfTcaKM: " << test.distanceOfTcaKM << std::endl;
    std::cout << "numberOfPointsTheAlgUsed: " << test.numberOfPointsTheAlgUsed << std::endl;
    std::cout << "runTimeMicro: " << test.runTimeMicro << std::endl;
    std::cout << "avgRunTimeMicro: " << test.avgRunTimeMicro << std::endl;
    std::cout << "minRunTimeMicro: " << test.minRunTimeMicro << std::endl;

    std::cout << "Segment Size: " << test.recipe.segmentSizeSec << std::endl;
    std::cout << "Tmin Factor: " << test.recipe.TminFactor << std::endl;
    std::cout << "TOLd: " << test.recipe.TOLd << std::endl;
    std::cout << "TOLt: " << test.recipe.TOLt << std::endl;


    std::cout << "Status: " << int(test.status) << std::endl;
}
void runSboAncasVariationsTest();

int main()
{

    Lab& lab = Lab::GetInstance();
    /*
    std::cout << "connection " << lab.CheckConnection() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "connection " << lab.CheckConnection() << std::endl;
    */
    /*
    int i = 1;
    while (i--)
    {
        
        //Initialize the data
        TestInfo info = { 0 };
        info.format = SatelliteDataFormat::Text;
        strcpy_s(info.recipe.testName, "STARLINK5447_UNICORN2N");
        info.status = TestStatus::Queued;
        */
        /*
    STARLINK5447_UNICORN2N

        char Obj1le1[] = "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993";
        char Obj1le2[] = "2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489";

        char Obj2le1[] = "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990";
        char Obj2le2[] = "2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268";
        */
        /*
        strcpy_s(info.firstElemData, "1 54779U 22175X   24081.08811856 -.00001209  00000+0 -57887-4 0  9993\n2 54779  53.2184  15.1482 0001476 102.2379 257.8779 15.08836826 69489");
        strcpy_s(info.secondElemData, "1 58642U 23185N   24081.15647041  .00022282  00000+0  15749-2 0  9990\n2 58642  97.6346 149.4102 0018842 223.6176 136.3561 15.04756794 13268");
        strcpy_s(info.recipe.testName, "STARLINK5447_UNICORN2N");
        info.recipe.catchPolynomialDegree = 15;
        info.recipe.numberOfPointsPerSegment = 16;
        info.recipe.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
        info.recipe.numberOfIterations = 1;
        info.recipe.TminFactor = 2;
        info.recipe.timeIntervalSizeSec = 7 * 24 * 60 * 60;//A Week
        info.recipe.TOLd = 10e-9;
        info.recipe.TOLt = 10e-5;

        //set the algorithm
        info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::ANCAS;
        info.recipe.numberOfIterations = 5;
        //Run the test
        info.recipe.testID = lab.CreateTest(info);
        printTestInfo(info);
        */
        /*
        //set the algorithm
        info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::CATCH;
        info.recipe.numberOfIterations = 1;
        //Run the test
        lab.CreateTest(info);
        */
        /*
        //set the algorithm
        info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS;
        info.recipe.numberOfIterations = 5;
        //Run the test
        lab.CreateTest(info);
        



        std::this_thread::sleep_for(std::chrono::seconds(1));
        */
        // 
        //check that the results was updated(hoping 8 seconds is enough!)
        /*
        TestInfo results = { 0 };
        results = lab.GetTestInfo(info.recipe.testID);
        printTestInfo(results);
        */
        /*
        std::this_thread::sleep_for(std::chrono::seconds(2));
        //Update status
        results = { 0 };
        lab.updateTestStatus(TestStatus::Failed, info.recipe.testID);
        results = lab.GetTestInfo(info.recipe.testID);
        printTestInfo(results);

        std::this_thread::sleep_for(std::chrono::seconds(2));
        //Update status
        results = { 0 };
        lab.updateTestStatus(TestStatus::Completed, info.recipe.testID);
        results = lab.GetTestInfo(info.recipe.testID);
        printTestInfo(results);*/
    /**}
    */
    runSboAncasVariationsTest();
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
    return 0;
}



void runSboAncasVariationsTest()
{
    Lab& lab = Lab::GetInstance();
    TestInfo info = { 0 };
    CatalogReader catalogReader;
    std::string tleLine1;
    std::string tleLine2;
    std::string tempString;
    std::string firstElemName;
    std::string secondElemName;
    char firstElemData[262];
    char secondElemData[262];
    if (true == catalogReader.Init("gpCatalog.txt"))
    {
        if (true == catalogReader.getTLE(0))
        {
            tleLine1 = catalogReader.m_tleLine1;
            tleLine2 = catalogReader.m_tleLine2;
            firstElemName = catalogReader.m_name;
            tempString = tleLine1 +"\n" + tleLine2;
            strncpy(firstElemData, tempString.c_str(), tempString.size());
            firstElemData[tempString.size()] = '\0';
            //Runnig over the catalog
            int catalogSize = catalogReader.getCatalogSize();
            for (int i = 1; i < catalogSize; i++)
            {
                //Get the second element data
                if (true == catalogReader.getTLE(i))
                {
                    tleLine1 = catalogReader.m_tleLine1;
                    tleLine2 = catalogReader.m_tleLine2;
                    secondElemName = catalogReader.m_name;
                    tempString = tleLine1 + "\n" + tleLine2;
                    strncpy(secondElemData, tempString.c_str(), tempString.size());
                    secondElemData[tempString.size()] = '\0';
                    //Do the test :^)
                    info = { 0 };
                    info.format = SatelliteDataFormat::Text;
                    strcpy_s(info.recipe.testName, catalogReader.m_testName);
                    info.status = TestStatus::Queued;
                    strcpy_s(info.firstElemData, firstElemData);
                    strcpy_s(info.secondElemData, secondElemData);
                    info.recipe.catchPolynomialDegree = 15;
                    info.recipe.numberOfPointsPerSegment = 16;
                    info.recipe.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = 2;
                    info.recipe.timeIntervalSizeSec = 7 * 24 * 60 * 60;//A Week
                    info.recipe.TOLd = 10e-9;
                    info.recipe.TOLt = 10e-5;


                    //set the algorithm
                    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::ANCAS;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = 2;
                    //Run the test
                    info.recipe.testID = lab.CreateTest(info);

                    //set the algorithm
                    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = 2;
                    //Run the test
                    info.recipe.testID = lab.CreateTest(info);

                    //set the algorithm
                    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS_ES;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = 2;
                    //Run the test
                    info.recipe.testID = lab.CreateTest(info);

                    std::this_thread::sleep_for(std::chrono::milliseconds(700));
                    catalogReader.printPercentage(i, catalogSize);
                }
                else {
                    //failed to get the data! that bad!
                    std::cout << "Failed To Get Data!" << std::endl;
                }
            }
        }
    }
    ;
}
