#include "CommManager.h"
#include "Utilities.h"
#include "EventLogger.h"

CommManager::CommManager()
{
    m_commChannel = nullptr;
    m_crcError = false;
    m_lastReceivedResultsMessage = { 0 };
}

void CommManager::Init(ICommChannel* commChannel)
{
    m_commChannel = commChannel;
    m_crcError = false;
    EventLogger::getInstance().log("Comm Manager Initialized", "CommManager");

}

TestResults::TestResult CommManager::GetLastReceivedTestResult() {
    return m_lastReceivedResultsMessage.results;
}

bool CommManager::GetIsCrcError() {
    return m_crcError;
}

bool CommManager::GetNextMessage() {
    unsigned char buffer[sizeof(MessagesDefinitions::TestResultsMessage)];
    unsigned int size = 0;
    bool messageReceived = false;
    std::string logString = "";

    m_crcError = false;
    if (m_commChannel->getNextMessage(buffer, sizeof(MessagesDefinitions::TestResultsMessage), &size))
    {
        logString = "Received Buffer With Size " + std::to_string(size);
        EventLogger::getInstance().log(logString, "CommManager");

        //check if we got the full header
        if (size >= sizeof(MessagesDefinitions::MessageHeader)) {

            //get the message header
            MessagesDefinitions::MessageHeader header;
            memcpy(&header, buffer, sizeof(MessagesDefinitions::MessageHeader));

            //check if its a test results message
            if (header.opcode == MessagesDefinitions::TestResultsMessageOpcode)
            {
                //check if we got the full results message
                int expectedSize = sizeof(MessagesDefinitions::TestResultsMessage);
                if (size == expectedSize)
                {
                    //check the crc
                    unsigned int crc = CRC32::calculate(buffer + MessagesDefinitions::MESSAGE_HEADER_SIZE, size - MessagesDefinitions::MESSAGE_HEADER_SIZE);
                    if (header.crc == crc)
                    {
                        //save the results message
                        memcpy(&m_lastReceivedResultsMessage, buffer, sizeof(MessagesDefinitions::TestResultsMessage));
                        messageReceived = true;
                        logString = "Full Message Received For Test - " + std::to_string(m_lastReceivedResultsMessage.results.testID);
                        EventLogger::getInstance().log(logString, "CommManager");
                    }
                    else
                    {
                        m_crcError = true;
                        logString = "CRC Error!! Expected CRC: " + std::to_string(header.crc) + "Calculated CRC : " + std::to_string(crc);
                        EventLogger::getInstance().log(logString, "CommManager");
                    }
                }
            }
        }
    }

    return messageReceived; 
}

bool CommManager::SendMessage(const TestRecipe& recipe, TcaCalculation::sPointData* testData)
{
    MessagesDefinitions::MessageHeader header;
    int dataSize;
    int size;
    int offset;
    bool messageSentSuccessfully = false;
    //Prepare the buffer we want to send
    //Get the buffer size
    dataSize = (recipe.numberOfPoints * sizeof(TcaCalculation::sPointData));
    size = MessagesDefinitions::MESSAGE_HEADER_SIZE + sizeof(TestRecipe) + dataSize;
    //create the buffer
    unsigned char* buffer = new unsigned char[size];
    //check the buffer
    if (nullptr != buffer)
    {
        //Fill the header
        header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
        header.dataSize = dataSize;

        //copy everything to the buffer:

        offset = sizeof(MessagesDefinitions::MessageHeader);
        
        //copy the recipe
        memcpy(buffer + offset, reinterpret_cast<const unsigned char*>(&recipe), sizeof(TestRecipe));
        offset += sizeof(TestRecipe);

        //copy the data
        memcpy(buffer + offset,reinterpret_cast<unsigned char*>(testData), dataSize);

        //calculate the crc
        header.crc = CRC32::calculate(buffer + sizeof(MessagesDefinitions::MessageHeader), dataSize + sizeof(TestRecipe));
        //copy the header
        memcpy(buffer, reinterpret_cast<unsigned char*>(&header), sizeof(MessagesDefinitions::MessageHeader));

        //Send the message
        //if (true == sendMessageInchunks(buffer, size))
        if(true == m_commChannel->sendMessage(buffer, size))
        {
            messageSentSuccessfully = true;
            EventLogger::getInstance().log("Message Sent Successfully", "CommManager");

        }
        delete[] buffer;
    }
    else
    {
        //Failed to send message! handle the error
        messageSentSuccessfully = false;
    }
    return messageSentSuccessfully;
}

bool CommManager::SendMessageInchunks(unsigned char* buffer, unsigned int size) {
    unsigned int bytesSent = 0; // Tracks the number of bytes sent

    while (bytesSent < size) {
        // Calculate the size of the next chunk
        unsigned int chunkSize = (((MAX_MESSAGE_SIZE) < (size - bytesSent)) ? (MAX_MESSAGE_SIZE) : (size - bytesSent));

        // Send the current chunk
        if (false == m_commChannel->sendMessage(buffer + bytesSent, chunkSize))
        {
            //Failed to send the current chunk
            return false;
        }
        // Update the total number of bytes sent
        bytesSent += chunkSize;
    }
    return true;

}

bool CommManager::CheckConnection()
{
    return m_commChannel->checkConnection();
}
