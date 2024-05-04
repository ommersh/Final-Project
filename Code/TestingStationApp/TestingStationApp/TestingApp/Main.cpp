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
void runAlgorithmsTimesTest(int TMIN_FACTOR, int NUMBER_OF_POINTS_PER_SEGMENT, double TIME_INTERVAL_SIZE_SEC,int howManyShouldIRun);

int main()
{

    Lab& lab = Lab::GetInstance();
    
    //Test algorithms run time
   // runAlgorithmsTimesTest(2, 16, 7 * 24 * 60 * 60,10000);

    //Test constant time interval with different number of points
    /*for (int tminFactor = 1; tminFactor < 8; tminFactor++)
    {
        runAlgorithmsTimesTest(tminFactor, 16, 7 * 24 * 60 * 60, 30);
    }*/

    //Test constant number of points with changing time interval
    
    const int secPerDay = 24 * 60 * 60;
    int TminF, numOfDay;
    TminF = 24;
    numOfDay = 1;
    runAlgorithmsTimesTest(TminF, 16, numOfDay* secPerDay, 30);
    TminF = 12;
    numOfDay = 2;
    runAlgorithmsTimesTest(TminF, 16, numOfDay * secPerDay, 30);
    TminF = 8;
    numOfDay = 3;
    runAlgorithmsTimesTest(TminF, 16, numOfDay * secPerDay, 30);
    TminF = 6;
    numOfDay = 4;
    runAlgorithmsTimesTest(TminF, 16, numOfDay * secPerDay, 30);
    TminF = 4;
    numOfDay = 6;
    runAlgorithmsTimesTest(TminF, 16, numOfDay * secPerDay, 30);
    TminF = 3;
    numOfDay = 8;
    runAlgorithmsTimesTest(TminF, 16, numOfDay * secPerDay, 30);
    TminF = 2;
    numOfDay = 12;
    runAlgorithmsTimesTest(TminF, 16, numOfDay * secPerDay, 30);
    
   // runAlgorithmsTimesTest(4, 16, 7 * 24 * 60 * 60);
    //runAlgorithmsTimesTest(8, 16, 7 * 24 * 60 * 60);

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


void runAlgorithmsTimesTest(int TMIN_FACTOR, int NUMBER_OF_POINTS_PER_SEGMENT, double TIME_INTERVAL_SIZE_SEC, int howManyShouldIRun)
{
    /*
    static const int TMIN_FACTOR = 2;
    static const int NUMBER_OF_POINTS_PER_SEGMENT = 16;
    static const double TIME_INTERVAL_SIZE_SEC = 7 * 24 * 60 * 60;//A Week
    */
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
            tempString = tleLine1 + "\n" + tleLine2;
            strncpy(firstElemData, tempString.c_str(), tempString.size());
            firstElemData[tempString.size()] = '\0';
            //Runnig over the catalog
            int catalogSize = catalogReader.getCatalogSize();
            int numberOfCases = min(catalogSize, howManyShouldIRun);
            for (int i = 1; i < numberOfCases; i++)
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
                    //handle the name
                    std::string tempName = firstElemName + '_' + secondElemName;
                    tempName.erase(std::remove(tempName.begin(), tempName.end(), ' '), tempName.end());//remove spaces
                    tempName.erase(std::remove(tempName.begin(), tempName.end(), '\t'), tempName.end());//remove tabs
                    tempName.erase(std::remove(tempName.begin(), tempName.end(), '\n'), tempName.end());//remove new lines

                    //Do the test :^)
                    info = { 0 };
                    info.format = SatelliteDataFormat::Text;
                    //Set the name to the test number
                    tempName = std::to_string(i - 1);
                    strcpy_s(info.recipe.testName, tempName.c_str());
                    info.status = TestStatus::Queued;
                    strcpy_s(info.firstElemData, firstElemData);
                    strcpy_s(info.secondElemData, secondElemData);
                    info.recipe.catchPolynomialDegree = NUMBER_OF_POINTS_PER_SEGMENT - 1;
                    info.recipe.numberOfPointsPerSegment = NUMBER_OF_POINTS_PER_SEGMENT;
                    info.recipe.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = TMIN_FACTOR;
                    info.recipe.timeIntervalSizeSec = TIME_INTERVAL_SIZE_SEC;
                    info.recipe.TOLd = 10e-9;
                    info.recipe.TOLt = 10e-5;


                    //set the algorithm
                    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::ANCAS;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = TMIN_FACTOR;
                    //Run the test
                    info.recipe.testID = lab.CreateTest(info);

                    //set the algorithm
                    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = TMIN_FACTOR;
                    //Run the test
                    info.recipe.testID = lab.CreateTest(info);

                    //set the algorithm
                    info.recipe.testedAlgorithm = AlgorithmsEnums::Algorithm::CATCH;
                    info.recipe.numberOfIterations = 1;
                    info.recipe.TminFactor = TMIN_FACTOR;
                    //Run the test
                    info.recipe.testID = lab.CreateTest(info);


                    //Wait for the tests to end before creating more tests
                    //Even if we waste some time, we dont want to fill the memory with waiting tests
                    //And the emulator is slow
                    int id = info.recipe.testID;
                    TestInfo tempinfo = { 0 };
                    do
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        tempinfo = lab.GetTestInfo(id);
                    } while (tempinfo.status != TestStatus::Completed && id >= 0);
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
