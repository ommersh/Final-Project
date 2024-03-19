#ifndef SHIELD_LocalFileCommChannelFacade_H    // Check if the symbol SHIELD_LocalFileCommChannelFacade_H is not defined
#define SHIELD_LocalFileCommChannelFacade_H    // Define the symbol SHIELD_LocalFileCommChannelFacade_H

#include "ICommChannel.h"
#include "FileReader.h"
#include "CommonStructures.h"
#include "SimpleDataGeneration.h"
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
		testID = 0;
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

		StateGetTestData,
		StateSendTestData,
		StateWaitForTestEnd,

		StateGetCatchData,
		StateSendCatchData,
		StateWaitForCatchEnd,
	};
protected:
	InnerStateMachine m_state;
	void getAncasData();
	void getSboAncasData();
	void getCatchData();
	MessageHeader m_header;

	sFileData m_fileData;
	TestParameters::TestRecipe m_params;
	unsigned int testID;
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
	void printResult(string algName, TestResults::TestResult results);
	unsigned char* m_messageBuffer;


	SimpleDataGeneration m_SimpleDataGeneration;
};
#endif //SHIELD_LocalFileCommChannelFacade_H