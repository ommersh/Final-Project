#ifndef SHIELD_TestingStationLocalSimCommChannel_H    // Check if the symbol SHIELD_TestingStationLocalSimCommChannel_H is not defined
#define SHIELD_TestingStationLocalSimCommChannel_H    // Define the symbol SHIELD_TestingStationLocalSimCommChannel_H


#include "ICommChannel.h"
#include "CommonStructures.h"
#include "TestRecipe.h"

/// <summary>
/// Interface for a communication channel
/// Including 3 function to implement:
/// getNextMessage: get the next message, return false if no message received
/// sendMessage: Send a message
/// reset: should be called after finishing with the last message and before receiving the next one
/// </summary>
class TestingStationLocalSimCommChannel : public ICommChannel
{
public:
	TestingStationLocalSimCommChannel()
	{
		m_runLocalTest = false;
		m_resultsMessage = { 0 };
		m_testRecipe = { 0 };
		m_pointsData = nullptr;
		m_runLocalTest = 0;
		m_startTime = 0;
		m_endTime = 0;
		m_timeInMicro = 0;
	}
	virtual ~TestingStationLocalSimCommChannel()
	{

	}
	/// <summary>
	/// Get the next message, return false if no message received
	/// </summary>
	/// <param name="buffer">Buffer for the outbut, for the received message</param>
	/// <param name="maxSize">The buffer size, the maximun number of bytes we can copy into the buffer</param>
	/// <param name="size">The actual message size copied into the buffer</param>
	/// <returns>Return true if a message received and false if not</returns>
	virtual bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override;
	/// <summary>
	/// Send a message
	/// </summary>
	/// <param name="buffer">Input buffer, message to send</param>
	/// <param name="size">The size of the message we send</param>
	virtual bool sendMessage(unsigned char* buffer, unsigned int size) override;
	/// <summary>
	/// Reset the channel between messages
	/// </summary>
	virtual void reset() override;
	/// <summary>
	/// Check if we are connected successfully
	/// </summary>
	/// <returns></returns>
	virtual bool checkConnection() override
	{
		return true;
	}
protected:
	MessagesDefinitions::TestResultsMessage m_resultsMessage;
	TestRecipe m_testRecipe;
	TcaCalculation::sPointData* m_pointsData;
	bool m_runLocalTest;
	void runTest();
	long int m_startTime;
	long int m_endTime;
	double m_timeInMicro;
	void startTimer();
	void stopTimer();
	double getTimeInMicroSec();
};
#endif //SHIELD_TestingStationLocalSimCommChannel_H
