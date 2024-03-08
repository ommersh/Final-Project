#ifndef LAB_H
#define LAB_H

#include <memory>

#include "CommManager.h" 
#include "../TestManager/TestManager.h"

class Lab {
public:
    static Lab& getInstance();

    CommManager& getCommManager() const;

    TestManager& getTestManager() const;

private:
    Lab();

    ~Lab();

    // Private copy constructor and assignment operator to prevent duplication
    Lab(const Lab&);
    Lab& operator=(const Lab&);

    // Members
    std::unique_ptr<CommManager> m_commManager;
    std::unique_ptr<TestManager> m_testManager;

};

#endif // LAB_H