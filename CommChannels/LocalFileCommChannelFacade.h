#ifndef SHIELD_LocalFileCommChannelFacade_H    // Check if the symbol SHIELD_LocalFileCommChannelFacade_H is not defined
#define SHIELD_LocalFileCommChannelFacade_H    // Define the symbol SHIELD_LocalFileCommChannelFacade_H

#include "ICommChannel.h"
#include "FileReader.h"
#include "TestDefinitions.h"

using namespace MessagesDefinitions;

class LocalFileCommChannelFacade : public ICommChannel
{
public:
	LocalFileCommChannelFacade()
	{
		m_state = InnerStateMachine::StateStart;
	}
	virtual ~LocalFileCommChannelFacade()
	{

	}
	bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
	void sendMessage(unsigned char* buffer, unsigned int size) override;
	void reset() override;

	enum InnerStateMachine
	{
		StateStart = 0,

		StateGetAncasData,
		StateSendAncasData,
		StateWaitForAncasEnd,

		StateGetCatchData,
		StateSendCatchData,
		StateWaitForCatchEnd,
	};
protected:
	InnerStateMachine m_state;
	void getAncasData();
	void getCatchData();
	sFileData m_fileData;
	TestParameters::TestParams m_params;

	int m_offset;
	int m_sizeToCompy;
	void startPrint();
	void printResult(string algName, int degree, string testName, int numberOfPoints, double runTime, TcaCalculation::TCA tca);

};
#endif //SHIELD_LocalFileCommChannelFacade_H