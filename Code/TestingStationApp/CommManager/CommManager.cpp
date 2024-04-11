#include "CommManager.h"

void CommManager::SetListeningState(bool listen)
{
    listening = listen;
}


/// <summary>
/// Get the next massage from the OCB
/// </summary>
/// <returns></returns>
bool CommManager::getNextMessage() {
    unsigned char buffer[1024];
    unsigned int offset = 0;
    unsigned int size = 0;
    unsigned int remainingDataSize;
    bool messageReceived = false;

    while (listening)
    {
        m_commChannel->getNextMessage(buffer, 1024, &size); //todo: implement
    }
    return messageReceived; // Placeholder implementation
}

//void CommManager::sendMessage(const TestRecipe& recipe, double[] timePoints) {
//    // Implement logic to send the message with the given recipe and time points
//}