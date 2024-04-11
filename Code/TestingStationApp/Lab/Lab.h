#ifndef LAB_H
#define LAB_H

#include "ResultManager.h"
#include "DatabaseManager.h"
#include "CommManager.h"
#include "TestDataGenerationManager.h"
using namespace std;



class Lab {
public:
    static Lab& GetInstance();

    TestInfo GetTestInfo(int testId);
    void DeleteTest(int testId);

    int CreateTest(std::string name, double timeInterval, int iterations, Algorithm alg, int catchPolynomDeg, int numOfTimePoints, std::string elemDataOne, string elemDataTwo, SatelliteDataFormat format);
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
};


#endif // LAB_H