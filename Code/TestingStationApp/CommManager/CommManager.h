#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

//#include "TimePoint.h"
#include <vector>
#include <memory>
#include "../../Common/CommChannels/LocalFileCommChannelFacade.h"


class CommManager {
public:
    CommManager()
    {
        m_commChannel = std::make_unique<LocalFileCommChannelFacade>();
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