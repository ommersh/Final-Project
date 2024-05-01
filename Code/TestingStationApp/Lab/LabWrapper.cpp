// labdll.cpp
#include "lab.h"
#include <cstring>

namespace LabWrap {

    struct TestIdsStruct {
        int* ids; // Pointer to an array of IDs
        int size; // Number of IDs
    };

    extern "C" {

        __declspec(dllexport) Lab* Lab_Create()
        { 
            return  &Lab::GetInstance();
        }

        //void Lab_Destroy(Lab* lab) { delete lab; }

        __declspec(dllexport) UserTestData Lab_GetTestInfo(Lab* lab, int testId)
        { 
            TestInfo testInfo = lab->GetTestInfo(testId);
            UserTestData userData = UserTestData();
            userData.testID = testInfo.recipe.testID;
            strncpy_s(userData.testName, testInfo.recipe.testName, strlen(testInfo.recipe.testName));
            userData.testName[strlen(testInfo.recipe.testName) + 1] = '\0';
            //Test Data
            //For runnig a test
            userData.catchPolynomialDegree = testInfo.recipe.catchPolynomialDegree;
            userData.numberOfPointsPerSegment = testInfo.recipe.numberOfPointsPerSegment;
            userData.catchRootsAlg = testInfo.recipe.catchRootsAlg;
            userData.testedAlgorithm = testInfo.recipe.testedAlgorithm;
            userData.numberOfIterations = testInfo.recipe.numberOfIterations;
            userData.TminFactor = testInfo.recipe.TminFactor;
            userData.timeIntervalSizeSec = testInfo.recipe.timeIntervalSizeSec;
            userData.TOLdKM = testInfo.recipe.TOLd;
            userData.TOLtSec = testInfo.recipe.TOLt;

            //For displaying the info
            //userData.julianDate = 0;
            userData.initialNumberOfPoints = testInfo.recipe.numberOfPoints;
            userData.segmentSizeSec = testInfo.recipe.segmentSizeSec;
            userData.format = testInfo.format;
            //Test Results
            //The results and run time of the first run
            userData.timeOfTcaFromStartingPointSec = testInfo.timeOfTcaFromStartingPointSec;
            userData.distanceOfTcaKM = testInfo.distanceOfTcaKM;
            userData.numberOfPointsTheAlgUsed = testInfo.numberOfPointsTheAlgUsed;
            userData.runTimeMicro = testInfo.runTimeMicro;
            userData.avgRunTimeMicro = testInfo.avgRunTimeMicro;
            userData.minRunTimeMicro = testInfo.minRunTimeMicro;


            userData.format = testInfo.format;
            userData.status = testInfo.status;
            userData.initialNumberOfPoints = testInfo.recipe.numberOfPoints;
            return userData;
        }

        __declspec(dllexport) void Lab_DeleteTest(Lab* lab, int testId) 
        {
            lab->DeleteTest(testId); 
        }


        __declspec(dllexport) int Lab_CreateTest(Lab* lab, UserTestData userData)
        {
            TestInfo testInfo;
            strncpy_s(testInfo.recipe.testName, userData.testName, MAX_TEST_NAME_SIZE);
            testInfo.recipe.catchPolynomialDegree = userData.catchPolynomialDegree;
            testInfo.recipe.numberOfPointsPerSegment = userData.numberOfPointsPerSegment;
            testInfo.recipe.catchRootsAlg = userData.catchRootsAlg;
            testInfo.recipe.testedAlgorithm = userData.testedAlgorithm;
            testInfo.recipe.numberOfIterations = userData.numberOfIterations;
            testInfo.recipe.TminFactor = userData.TminFactor;
            testInfo.recipe.timeIntervalSizeSec = userData.timeIntervalSizeSec;
            testInfo.recipe.TOLd = userData.TOLdKM;
            testInfo.recipe.TOLt = userData.TOLtSec;
            testInfo.format = userData.format;
            testInfo.avgRunTimeMicro = 0;
            testInfo.minRunTimeMicro = 0;
            testInfo.runTimeMicro = 0;
            testInfo.realDistance = 0;
            testInfo.realTCA = 0;
            strncpy_s(testInfo.firstElemData, userData.orbitingElementData1, 262);
            strncpy_s(testInfo.secondElemData, userData.orbitingElementData2, 262);

            return lab->CreateTest(testInfo);
        }

        __declspec(dllexport) TestIdsStruct Lab_GetAllTestIds(Lab* lab)
        {
            set<int> testIds = lab->getAllTestIds();

            TestIdsStruct testidStruct;
            testidStruct.size = testIds.size();
            testidStruct.ids = new int[testidStruct.size];
            int i = 0;
            for (int id : testIds)
            {
                testidStruct.ids[i++] = id;
            }
            return testidStruct;
        }

        __declspec(dllexport) bool Lab_CheckConnection()
        {
            return Lab::GetInstance().CheckConnection();
        }
    }
}
