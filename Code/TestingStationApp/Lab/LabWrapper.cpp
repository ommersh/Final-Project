// labdll.cpp
#include "lab.h"
//#include <string>
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

        __declspec(dllexport) TestInfo Lab_GetTestInfo(Lab* lab, int testId)
        { 
            return lab->GetTestInfo(testId);
        }

        __declspec(dllexport) void Lab_DeleteTest(Lab* lab, int testId) 
        {
            lab->DeleteTest(testId); 
        }

        __declspec(dllexport) unsigned int Lab_CreateTest(Lab* lab, const std::string& name, double timeInterval, int iterations, AlgorithmsEnums::Algorithm alg, int catchPolynomDeg, int numOfTimePoints, const std::string& elemDataOne, const std::string& elemDataTwo, SatelliteDataFormat format)
        {
            return lab->CreateTest("name", timeInterval, iterations, alg, catchPolynomDeg, numOfTimePoints, "data1", "data2", format);
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
}
