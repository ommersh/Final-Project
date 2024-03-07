#ifndef SHIELD_LocalFileCommChannelFacade_H    // Check if the symbol SHIELD_LocalFileCommChannelFacade_H is not defined
#define SHIELD_LocalFileCommChannelFacade_H    // Define the symbol SHIELD_LocalFileCommChannelFacade_H

#include "ICommChannel.h"
#include "FileReader.h"
#include "CommonStructures.h"

using namespace MessagesDefinitions;
/// <summary>
/// Implementation of the ICommChannel interface.
/// Reading a local files instead of an actual communication channel.
/// can use for testing or a unsynchronized run.
/// </summary>
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

	/// <summary>
	/// Internal state machine for the class.
	/// Create a cyclic implementation of getting an input message for running ANCAS and CATCH repeatedly.
	/// </summary>
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
	TestParameters::TestRecipe m_params;

	int m_offset;
	int m_sizeToCompy;
	void startPrint();
	/// <summary>
	/// Instead of sending a message print the results
	/// </summary>
	/// <param name="algName"></param>
	/// <param name="degree"></param>
	/// <param name="testName"></param>
	/// <param name="numberOfPoints"></param>
	/// <param name="runTime"></param>
	/// <param name="tca"></param>
	void printResult(string algName, int degree, string testName, int numberOfPoints, double runTime, TcaCalculation::TCA tca);

};
#endif //SHIELD_LocalFileCommChannelFacade_H