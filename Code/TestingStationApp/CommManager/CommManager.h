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
    CommManager(ICommChannel* commChannel)
        : m_commChannel(commChannel)
    {
        listening = false;
    }
    void SetListeningState(bool listen);
    bool getNextMessage();
    void sendMessage(const TestRecipe& recipe, TcaCalculation::sPointData* testData);
    TestResults::TestResult getLastReceivedTestResult() {
        return m_lastReceivedResultsMessage.results;
    }

private:
    bool listening = false;
    std::unique_ptr<ICommChannel> m_commChannel;
    MessagesDefinitions::TestResultsMessage m_lastReceivedResultsMessage;
};

#endif // COMM_MANAGER_H