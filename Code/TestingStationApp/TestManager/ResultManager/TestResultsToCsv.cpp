#include "TestResultsToCsv.h"

// Constructor
TestResultsToCsv::TestResultsToCsv() {


    // Get the current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    struct tm timeinfo;

    // Convert to local time
#ifdef _WIN32
// Use localtime_s on Windows
    localtime_s(&timeinfo, &time_t);
#else
// Use localtime_r on Linux and other UNIX-like systems
    localtime_r(&time_t, &timeinfo);
#endif

    // Format the time into a string with the desired format
    std::ostringstream oss;
    oss << std::put_time(&timeinfo, "%d-%m-%Y_%H-%M-%S");
    std::string timeStr = oss.str();

    // Construct the filename using the date and time string
    std::string filename = "Logger/TestResults_" + timeStr + ".csv";

    std::cout << "Test Results filename: " << filename << std::endl;


    m_fileName = filename;

    outFile.open(filename, std::ios::app); // Append mode
    if (!outFile.is_open()) {
        std::cout << "Failed to open " << filename << " for writing." << std::endl;
    }
    else {
        std::cout << "Opened " << filename << " for writing." << std::endl;

        // Check if the file is new or empty by checking its size
        outFile.seekp(0, std::ios::end); // Move the write pointer to the end of the file
        if (outFile.tellp() == 0) { // If the write pointer is at position 0, the file is empty
            // File is new or empty, write the headers
            SetFileHeader();
        }
        // If the file is not empty, do not write headers
    }
}

// Destructor
TestResultsToCsv::~TestResultsToCsv() {
    if (outFile.is_open()) {
        std::cout << "Closing the results file: " << m_fileName << std::endl;
        outFile.close();
    }
}

void TestResultsToCsv::SaveInfo(TestInfo testInfo, int FullTolReached, int ToldReached, int ToltReached, int NoRootsFound) {
    std::string algName ="";
    switch (testInfo.recipe.testedAlgorithm)
    {
    case AlgorithmsEnums::ANCAS:
        algName = "ANCAS";
        break;
    case AlgorithmsEnums::CATCH:
        algName = "CATCH";
        break;
    case AlgorithmsEnums::SBO_ANCAS:
        algName = "SBO ANCAS";
        break;
    case AlgorithmsEnums::SBO_ANCAS_ES:
        algName = "SBO ANCAS ES";
        break;
    default:
        algName = "????";
        break;
    }
    SaveInfo(testInfo, algName, FullTolReached, ToldReached, ToltReached, NoRootsFound);
}

// Method to log a single row of results
void TestResultsToCsv::SaveInfo(TestInfo testInfo, std::string algName, int FullTolReached, int ToldReached, int ToltReached, int NoRootsFound)
{
    if (outFile.is_open()) {
        outFile << std::fixed << std::setprecision(30)
            << testInfo.recipe.testID << ","                                            //0     "Test ID"
            << sanitize(testInfo.recipe.testName) << ","                                //1     "Test Name"
            << algName << ","                                                           //2     "Algorithm Name"
            << testInfo.recipe.numberOfPointsPerSegment << ","                          //3     "numberOfPointsPerSegment"
            << testInfo.recipe.catchPolynomialDegree << ","                             //4     "Catch Polynomial Degree"
            << testInfo.recipe.timeIntervalSizeSec << ","                               //5     "Time Interval[Sec]"
            << testInfo.recipe.segmentSizeSec << ","                                    //6     "Segment Size[Sec]"
            << testInfo.recipe.TminFactor << ","                                        //7     "Tmin Factor"
            << testInfo.recipe.numberOfPoints << ","                                    //8     "Number Of Input Points"
            << testInfo.recipe.numberOfIterations << ","                                //9     "Number Of Iterations"
            << testInfo.recipe.TOLd << ","                                              //10    "TOLd[Km]"
            << testInfo.recipe.TOLt << ","                                              //11    "TOLt[Sec]"
            << testInfo.runTimeMicro / 1000000 << ","                                   //12    "Run Time[Sec]"
            << testInfo.runTimeMicro << ","                                             //13    "Run Time[Micro]"
            << testInfo.avgRunTimeMicro << ","                                          //14    "Avg Run Time[Micro]"
            << testInfo.minRunTimeMicro << ","                                          //15    "Min Run Time[Micro]"
            << testInfo.distanceOfTcaKM << ","                                          //16    "TCA Distance[Km]"
            << testInfo.timeOfTcaFromStartingPointSec << ","                            //17    "TCA Time[Sec]"
            << testInfo.realDistance << ","                                             //18    "Real TCA Distance[Km]"
            << testInfo.realTCA << ","                                                  //19    "Real TCA[Sec]"
            << fabs(testInfo.realTCA - testInfo.timeOfTcaFromStartingPointSec) << ","   //20    "Error Time[Sec]"
            << fabs(testInfo.realDistance - testInfo.distanceOfTcaKM) << ","            //21    "Error Distance[Km]"
            << testInfo.numberOfPointsTheAlgUsed << ","                                  //22    "Number of Points Used"

            << FullTolReached << ","      //23
            << ToldReached << ","         //24
            << ToldReached << ","         //25
            << NoRootsFound << ""         //26

            << std::endl;
    }
    else {
        std::cerr << "Attempt to write to a closed file." << std::endl;
    }
}


void TestResultsToCsv::SetFileHeader()
{
    if (outFile.is_open()) {
        outFile 
            << "Test ID" << ","                     //0
            << "Test Name" << ","                   //1
            << "Algorithm Name" << ","              //2
            << "numberOfPointsPerSegment" << ","    //3
            << "Catch Polynomial Degree" << ","     //4
            << "Time Interval[Sec]" << ","          //5
            << "Segment Size[Sec]" << ","           //6
            << "Tmin Factor" << ","                 //7
            << "Number Of Input Points" << ","      //8
            << "Number Of Iterations" << ","        //9
            << "TOLd[Km]" << ","                    //10
            << "TOLt[Sec]" << ","                   //11
            << "Run Time[Sec]" << ","               //12
            << "Run Time[Micro]" << ","             //13
            << "Avg Run Time[Micro]" << ","         //14
            << "Min Run Time[Micro]" << ","         //15
            << "TCA Distance[Km]" << ","            //16
            << "TCA[Sec]" << ","                    //17
            << "Real TCA Distance[Km]" << ","       //18
            << "Real TCA[Sec]" << ","               //19
            << "Error Time[Sec]" << ","             //20
            << "Error Distance[Km]" << ","          //21
            << "Number of Points Used" << ","       //22

            << "FullTolReached" << ","      //23
            << "ToldReached" << ","         //24
            << "ToldReached" << ","         //25
            << "NoRootsFound" << ""         //26


            << std::endl;
    }
}

std::string TestResultsToCsv::sanitize(const std::string& str) {
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    return result;
}