#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

//#include "TimePoint.h"
#include <vector>
#include <memory>
#include "../../Common/CommChannels/ICommChannel.h"


class CommManager {
public:
    CommManager(ICommChannel* commChannel)
        : m_commChannel(commChannel)
    {
        listening = false;
    }
    void SetListeningState(bool listen);
    bool getNextMessage();
    //void sendMessage(const TestRecipe& recipe, double[] timePoints)
private:
    bool listening = false;
    std::unique_ptr<ICommChannel> m_commChannel;
};

#endif // COMM_MANAGER_H