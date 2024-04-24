#ifndef SHIELD_CommManager_H    // Check if the symbol SHIELD_CommManager_H is not defined
#define SHIELD_CommManager_H    // Define the symbol SHIELD_CommManager_H

#include "CommonStructures.h"
#include "ICommChannel.h"
#include <cstdint>

/// <summary>
/// Manager for the communication between the TestingStationApp and the TestedOBCApp.
/// Using a ICommChannel receive raw data, parse it into the relevant test data and send the results at the end.
/// </summary>
class CommManager
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="commChannel">The communication channel to the TestingStationApp</param>
	CommManager(ICommChannel& commChannel);
	virtual ~CommManager();
	/// <summary>
	/// Initialize the structs and communication channel if necessary
	/// </summary>
	virtual void init();
	/// <summary>
	/// Get the next available test command from the communication channel(if exist)
	/// </summary>
	/// <returns>true if a messgae received</returns>
	virtual bool getTheNextTest();
	/// <summary>
	/// Get the test parameters from the last message received
	/// </summary>
	/// <returns></returns>
	virtual TestRecipe getTheTestParameters();
	/// <summary>
	///Get the test data from the last message received
	/// </summary>
	/// <returns></returns>
	virtual TcaCalculation::sPointData* getTheTestData();
	/// <summary>
	/// Call the function when the test end to free all the memory if necessary
	/// </summary>
	virtual void endTest();
	/// <summary>
	/// Send the results back to the TestingStationApp
	/// </summary>
	/// <param name="testResults"></param>
	virtual void sendTestResults(TestResults::TestResult testResults);
protected:
	ICommChannel& m_commChannel;
	TestRecipe m_testParameters;
	TcaCalculation::sPointData* m_pointsData;

	//Parser related stuff
	
	/// <summary>
	/// Initialize the parser
	/// </summary>
	/// <param name="opcode">The incoming message expected OpCode</param>
	void initParser(unsigned char opcode[MessagesDefinitions::OPCODE_SIZE]);
	/// <summary>
	/// Parse the incoming buffer, return true if a full message was found
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	bool parseBuffer(unsigned char* buffer, int32_t size);
	/// <summary>
	/// Reset the parser states and variables
	/// </summary>
	void resetParser();
	/// <summary>
	/// State for the internal parser state machine
	/// </summary>
	enum ParserState{
		LookingForOpCode,
		CollectingHeader,
		CollectingData,
		Done
	};
	/// <summary>
	/// The current parser state
	/// </summary>
	ParserState m_parserState;
	/// <summary>
	/// The incoming Opcode
	/// </summary>
	unsigned char m_opCodeBuffer[MessagesDefinitions::OPCODE_SIZE];
	/// <summary>
	/// The expected Opcode
	/// </summary>
	unsigned char m_opCode[MessagesDefinitions::OPCODE_SIZE];
	/// <summary>
	/// The incoming header
	/// </summary>
	unsigned char m_headerBuffer[MessagesDefinitions::MESSAGE_HEADER_SIZE];
	/// <summary>
	/// The current index of the Opcode buffer
	/// </summary>
	uint32_t m_opCodeBufferIndex;
	/// <summary>
	/// The current index of the Header buffer
	/// </summary>
	uint32_t m_headerBufferIndex;
	/// <summary>
	/// The incoming parsed header
	/// </summary>
	MessagesDefinitions::MessageHeader m_messageHeader;
	/// <summary>
	/// The incoming message size
	/// </summary>
	uint32_t m_messageSize;
	/// <summary>
	/// The incoming message buffer
	/// </summary>
	unsigned char* m_messageBuffer;
	/// <summary>
	/// The current index of the incoming message buffer
	/// </summary>
	uint32_t  m_messageBufferIndex;


};
#endif //SHIELD_CommManager_H