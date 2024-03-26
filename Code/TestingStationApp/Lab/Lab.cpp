#include "lab.h"


/// <summary>
/// Initialize members
/// </summary>
Lab::Lab() {
    m_commManager = std::make_unique<CommManager>();
    m_resultManager = std::make_unique<ResultManager>();
    m_dataGenerator = std::make_unique<TestDataGenerationManager>();
}

/// <summary>
/// Destructor
/// </summary>
Lab::~Lab() {

}

Lab& Lab::GetInstance() {
    static Lab instance;
    return instance;
}

CommManager& Lab::GetCommManager() const {
    return *m_commManager;
}

ResultManager& Lab::GetResultManager() const {
    return *m_resultManager;
}

TestDataGenerationManager& Lab::GetTestDataGeneratorManager() const {
    return *m_dataGenerator;
}