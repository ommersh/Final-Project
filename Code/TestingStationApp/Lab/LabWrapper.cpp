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
            UserTestData userData;
            strncpy_s(userData.testName, testInfo.recipe.testName, MAX_TEST_NAME_SIZE);
            userData.catchPolynomialDegree = testInfo.recipe.catchPolynomialDegree;
            userData.numberOfPointsPerSegment = testInfo.recipe.numberOfPointsPerSegment;
            userData.catchRootsAlg = testInfo.recipe.catchRootsAlg;
            userData.testedAlgorithm = testInfo.recipe.testedAlgorithm;
            userData.numberOfIterations = testInfo.recipe.numberOfIterations;
            userData.TminFactor = testInfo.recipe.TminFactor;
            userData.timeIntervalSizeSec = testInfo.recipe.timeIntervalSizeSec;
            userData.TOLdKM = testInfo.recipe.TOLd;
            userData.TOLtSec = testInfo.recipe.TOLt;
            userData.testID = testInfo.recipe.testID;
            return userData;
        }

        __declspec(dllexport) void Lab_DeleteTest(Lab* lab, int testId) 
        {
            lab->DeleteTest(testId); 
        }

        //__declspec(dllexport) unsigned int Lab_CreateTest(Lab* lab, const std::string& name, double timeInterval, int iterations, AlgorithmsEnums::Algorithm alg, int catchPolynomDeg, int numOfTimePoints, const std::string& elemDataOne, const std::string& elemDataTwo, SatelliteDataFormat format)
        //{
        //    return lab->CreateTest("name", timeInterval, iterations, alg, catchPolynomDeg, numOfTimePoints, "data1", "data2", format);
        //}

        __declspec(dllexport) int Lab_CreateTest(Lab* lab, UserTestData userData)
        {
            TestInfo testInfo;
            //CopyTestData(userTestData, testInfo);
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
    }


    //void CopyTestData(const UserTestData& userData, TestInfo& testInfo)
    //{
    //    strncpy_s(testInfo.recipe.testName, userData.testName, MAX_TEST_NAME_SIZE);
    //    testInfo.recipe.catchPolynomialDegree = userData.catchPolynomialDegree;
    //    testInfo.recipe.numberOfPointsPerSegment = userData.numberOfPointsPerSegment;
    //    testInfo.recipe.catchRootsAlg = userData.catchRootsAlg;
    //    testInfo.recipe.testedAlgorithm = userData.testedAlgorithm;
    //    testInfo.recipe.numberOfIterations = userData.numberOfIterations;
    //    testInfo.recipe.TminFactor = userData.TminFactor;
    //    testInfo.recipe.timeIntervalSizeSec = userData.timeIntervalSizeSec;
    //    testInfo.recipe.TOLd = userData.TOLdKM;
    //    testInfo.recipe.TOLt = userData.TOLtSec;
    //}
}
