#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

#include <vector>
#include <memory>
#include "ICommChannel.h"

#include "TestRecipe.h"
#include "TcaCalculation.h"
#include "CommonStructures.h"

class CommManager {
public:
    CommManager()
    {
        m_commChannel = nullptr;
    }
    void init(ICommChannel* commChannel)
    {
        m_commChannel = commChannel;
    }
    bool getNextMessage();
    bool sendMessage(const TestRecipe& recipe, TcaCalculation::sPointData* testData);
    TestResults::TestResult getLastReceivedTestResult() {
        return m_lastReceivedResultsMessage.results;
    }

private:
    ICommChannel* m_commChannel;
    MessagesDefinitions::TestResultsMessage m_lastReceivedResultsMessage;
};

#endif // COMM_MANAGER_H