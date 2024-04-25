#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

#include <vector>
#include <memory>
#include "ICommChannel.h"

#include "TestRecipe.h"
#include "TcaCalculation.h"
#include "CommonStructures.h"

/// <summary>
/// Manage communicotion with the TestedObcApp, using an instance of CommChannel. 
/// Handle incoming and outgoing messages
/// </summary>
class CommManager {
public:
    CommManager();
    /// <summary>
    /// Initialize the comm manager, with the given comm channel
    /// </summary>
    /// <param name="commChannel"></param>
    void Init(ICommChannel* commChannel);
    /// <summary>
    /// Get the next message from the comm channel
    /// If a message exist, read the message, parse and save it
    /// Return  true if a full message was found
    /// </summary>
    /// <returns>True if a full message was found</returns>
    bool GetNextMessage();
    /// <summary>
    /// Send a test request message to the testedOBCApp
    /// Return true if the message sent successfully 
    /// </summary>
    /// <param name="recipe">The test recipe</param>
    /// <param name="testData">The test data</param>
    /// <returns>True if the message sent successfully</returns>
    bool SendMessage(const TestRecipe& recipe, TcaCalculation::sPointData* testData);
    /// <summary>
    /// Get the last received test results from the testedOBCApp
    /// </summary>
    /// <returns></returns>
    TestResults::TestResult GetLastReceivedTestResult();
    /// <summary>
    /// Return true if the last getNextMessage call failed because of crc error
    /// </summary>
    /// <returns></returns>
    bool GetIsCrcError();
private:
    ICommChannel* m_commChannel;
    MessagesDefinitions::TestResultsMessage m_lastReceivedResultsMessage;
    bool m_crcError;
    /// <summary>
    /// Function for sending the message in blocks with size MAX_MESSAGE_SIZE
    /// Not used
    /// </summary>
    /// <param name="buffer"></param>
    /// <param name="size"></param>
    /// <returns></returns>
    bool SendMessageInchunks(unsigned char* buffer, unsigned int size);
};

#endif // COMM_MANAGER_H