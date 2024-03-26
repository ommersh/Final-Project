#ifndef LAB_H
#define LAB_H

#include <memory>

#include "CommManager.h" 
#include <TestDataGenerationManager.h>
#include "../TestManager/ResultManager/ResultManager.h"

class Lab {
public:
    static Lab& GetInstance();

    CommManager& GetCommManager() const;
    ResultManager& GetResultManager() const;
    TestDataGenerationManager& GetTestDataGeneratorManager() const;


private:
    Lab();

    ~Lab();

    // Private copy constructor and assignment operator to prevent duplication
    Lab(const Lab&);
    Lab& operator=(const Lab&);

    // Members
    std::unique_ptr<CommManager> m_commManager;
    std::unique_ptr<ResultManager> m_resultManager;
    std::unique_ptr<TestDataGenerationManager> m_dataGenerator;


};

#endif // LAB_H