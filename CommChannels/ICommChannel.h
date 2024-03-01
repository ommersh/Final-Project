#ifndef SHIELD_ICommChannel_H    // Check if the symbol SHIELD_ICommChannel_H is not defined
#define SHIELD_ICommChannel_H    // Define the symbol SHIELD_ICommChannel_H

namespace MessagesDefinitions
{
	static const unsigned short TestRequestMessageOpcode = 0x1234;
	static const short TestResultsMessageOpcode = 0x4321;

	struct MessageHeader
	{
		unsigned short opcode;
		unsigned int dataSize;
	};
	
}



class ICommChannel
{
public:
	ICommChannel()
	{

	}
	virtual ~ICommChannel()
	{

	}
	virtual bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int * size) = 0;
	virtual void sendMessage(unsigned char* buffer, unsigned int size) = 0;
	virtual void reset() = 0;

	
protected:

};
#endif //SHIELD_ICommChannel_H