#ifndef SHIELD_TestedOBCLocalSimulation_H    // Check if the symbol SHIELD_TestedOBCLocalSimulation_H is not defined
#define SHIELD_TestedOBCLocalSimulation_H    // Define the symbol SHIELD_TestedOBCLocalSimulation_H

#include "ICommChannel.h"
#include "FileReader.h"
#include "CommonStructures.h"
#include "SimpleDataGeneration.h"
#include "FullCatalogTestDataGeneration.h"
using namespace MessagesDefinitions;
/// <summary>
/// Implementation of the ICommChannel interface.
/// Reading a local files instead of an actual communication channel.
/// can use for testing or a unsynchronized run.
/// </summary>
class TestedOBCLocalSimulation : public ICommChannel
{
public:
	TestedOBCLocalSimulation()
	{
		m_state = InnerStateMachine::StateStart;
		testID = 0;
	}
	virtual ~TestedOBCLocalSimulation()
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

	void init(const std::string& catalogFilePath);

protected:
	InnerStateMachine m_state;
	void getTestData();

	void getAncasData();
	void getSboAncasData();
	void getCatchData();
	void calculateTheTcaWithSmallTimeStep();
	void calculateTheTcaWithSmallTimeStepAroundPoint(double timePoint, double segmentSize);

	//Parameters related to the test message and data
	MessageHeader m_header;
	sFileData m_fileData;
	TestRecipe m_params;
	unsigned int testID;
	unsigned char* m_messageBuffer;

	//Simple Test Data generation
	SimpleDataGeneration m_SimpleDataGeneration;
	FullCatalogTestDataGeneration m_fullCatalogTestDataGeneration;
	bool m_fullCatalog;
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


};
#endif //SHIELD_TestedOBCLocalSimulation_H