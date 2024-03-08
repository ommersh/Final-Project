#include "lab.h"


/// <summary>
/// Initialize members
/// </summary>
Lab::Lab() {
    m_commManager = std::make_unique<CommManager>();
    m_testManager = std::make_unique<TestManager>();
}

/// <summary>
/// Destructor
/// </summary>
Lab::~Lab() {

}

Lab& Lab::getInstance() {
    static Lab instance;
    return instance;
}

CommManager& Lab::getCommManager() const {
    return *m_commManager;
}

TestManager& Lab::getTestManager() const {
    return *m_testManager;
}