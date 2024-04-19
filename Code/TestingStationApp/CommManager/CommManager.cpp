#include "CommManager.h"

/// <summary>
/// Get the next massage from the OCB
/// </summary>
/// <returns></returns>
bool CommManager::getNextMessage() {
    unsigned char buffer[sizeof(MessagesDefinitions::TestResultsMessage)];
    unsigned int size = 0;
    bool messageReceived = false;

    if (m_commChannel->getNextMessage(buffer, sizeof(MessagesDefinitions::TestResultsMessage), &size))
    {
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
                    //save the results message
                    memcpy(&m_lastReceivedResultsMessage, buffer, sizeof(MessagesDefinitions::TestResultsMessage));
                    messageReceived = true;
                }
            }
        }
    }

    return messageReceived; 
}

bool CommManager::sendMessage(const TestRecipe& recipe, TcaCalculation::sPointData* testData)
{
    MessagesDefinitions::MessageHeader header;
    int dataSize;
    int size;
    int offset;
    bool messageSentSuccessfully = false;
    //Prepare the buffer we want to send
    //Get the buffer size
    dataSize = (recipe.numberOfPoints * sizeof(TcaCalculation::sPointData));
    size = sizeof(MessagesDefinitions::MessageHeader) + sizeof(TestRecipe) + dataSize;
    //create the buffer
    unsigned char* buffer = new unsigned char[size];
    //check the buffer
    if (nullptr != buffer)
    {
        //Fill the header
        header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
        header.dataSize = dataSize;

        //copy everything to the buffer
        offset = 0;
        //copy the header
        memcpy(buffer, &header, sizeof(MessagesDefinitions::MessageHeader));
        offset += sizeof(MessagesDefinitions::MessageHeader);
        
        //copy the recipe
        memcpy(buffer + offset, reinterpret_cast<const unsigned char*>(&recipe), sizeof(TestRecipe));
        offset += sizeof(TestRecipe);

        //copy the data
        memcpy(buffer + offset,reinterpret_cast<unsigned char*>(testData), dataSize);

        //Send the message
        if (true == sendMessageInchunks(buffer, size))
        {
            messageSentSuccessfully = true;
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

bool CommManager::sendMessageInchunks(unsigned char* buffer, unsigned int size) {
    unsigned int bytesSent = 0; // Tracks the number of bytes sent

    while (bytesSent < size) {
        // Calculate the size of the next chunk
        unsigned int chunkSize = (((MAX_MESSAGE_SIZE) < (size - bytesSent)) ? (MAX_MESSAGE_SIZE) : (size - bytesSent));

        // Send the current chunk
        if (false == m_commChannel->sendMessage(buffer + bytesSent, chunkSize))
        {
            //std::cerr << "Failed to send the current chunk" << "\n";
            return false;
        }

        // Update the total number of bytes sent
        bytesSent += chunkSize;
    }
    return true;

}