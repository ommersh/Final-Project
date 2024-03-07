#ifndef SHIELD_ICommChannel_H    // Check if the symbol SHIELD_ICommChannel_H is not defined
#define SHIELD_ICommChannel_H    // Define the symbol SHIELD_ICommChannel_H

/// <summary>
/// Interface for a communication channel
/// Including 3 function to implement:
/// getNextMessage: get the next message, return false if no message received
/// sendMessage: Send a message
/// reset: should be called after finishing with the last message and before receiving the next one
/// </summary>
class ICommChannel
{
public:
	ICommChannel()
	{

	}
	virtual ~ICommChannel()
	{

	}
	/// <summary>
	/// Get the next message, return false if no message received
	/// </summary>
	/// <param name="buffer">Buffer for the outbut, for the received message</param>
	/// <param name="maxSize">The buffer size, the maximun number of bytes we can copy into the buffer</param>
	/// <param name="size">The actual message size copied into the buffer</param>
	/// <returns>Return true if a message received and false if not</returns>
	virtual bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int * size) = 0;
	/// <summary>
	/// Send a message
	/// </summary>
	/// <param name="buffer">Input buffer, message to send</param>
	/// <param name="size">The size of the message we send</param>
	virtual void sendMessage(unsigned char* buffer, unsigned int size) = 0;
	/// <summary>
	/// Reset the channel between messages
	/// </summary>
	virtual void reset() = 0;

	
protected:

};
#endif //SHIELD_ICommChannel_H