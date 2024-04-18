#ifndef LAB_H
#define LAB_H

#include "ResultManager.h"
#include "DatabaseManager.h"
#include "CommManager.h"
#include "TestDataGenerationManager.h"
#include "TestingStationLocalSimCommChannel.h"
#include "TestManager.h"
#include "WinUdpCommChannel.h"
#include <memory>

using namespace std;

struct UserTestData{
    //Test identifiers 
    int testID;
    char testName[MAX_TEST_NAME_SIZE];

    //Test Data
    //For runnig a test
    int catchPolynomialDegree;			// Degree of the polynomial, should be numberOfPointsPerSegment - 1
    int	numberOfPointsPerSegment;		// Use for data generations
    AlgorithmsEnums::CatchRootsAlg catchRootsAlg;        // what variation to use for catch roots finding
    AlgorithmsEnums::Algorithm testedAlgorithm;
    unsigned int numberOfIterations;
    int TminFactor; //2/4/8 what we divide the smaller iteration by to get Gamma
    double timeIntervalSizeSec;
    double TOLdKM; // tolerance of distance for SBO ANCAS in KM
    double TOLtSec; // tolerance of time for SBO ANCAS in SEC
    char orbitingElementData1[400];
    char orbitingElementData2[400];



    //For displaying the info
    double julianDate; // the julian date of the first point
    int initialNumberOfPoints;
    double segmentSizeSec; //Gamma in secons
    SatelliteDataFormat format;
    //We can hold the inputs strings

    //Test Results
    //The results and run time of the first run
    double timeOfTcaFromStartingPointSec;
    double distanceOfTcaKM;
    int numberOfPointsTheAlgUsed;
    double runTimeMicro;

    //Repeated tests results - the iterations
    double avgRunTimeMicro;
    double minRunTimeMicro;

};


class Lab {
public:
    static Lab& GetInstance();

    TestInfo GetTestInfo(int testId);
    void DeleteTest(int testId);

    //int CreateTest(std::string name, double timeInterval, int iterations, AlgorithmsEnums::Algorithm alg, int catchPolynomDeg, int numOfTimePoints, std::string elemDataOne, string elemDataTwo, SatelliteDataFormat format);
    int CreateTest(TestInfo testInfo);

    std::set<int> getAllTestIds();

private:
    Lab();
    ~Lab();

    // Private copy constructor and assignment operator to prevent duplication
    Lab(const Lab&);
    Lab& operator=(const Lab&);

    //// Members
    CommManager m_commManager;
    ResultManager m_resultManager;
    TestDataGenerationManager m_dataGenerator;
    DatabaseManager m_databaseManager; 
    TestManager m_testManager;
    TestingStationLocalSimCommChannel m_testingStationLocalSimCommChannel;
    WinTUdpCommChannel m_WinTUdpCommChannel;
};


#endif // LAB_H