#ifndef SHIELD_UTSboAncasTests_H    // Check if the symbol SHIELD_UTSboAncasTests_H is not defined
#define SHIELD_UTSboAncasTests_H    // Define the symbol SHIELD_UTSboAncasTests_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "CommManager.h"
#include "Utilities.h"

class MockICommChannel : public ICommChannel
{
public:
    MockICommChannel()
    {

    }
    bool getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size) override
    {
        return false;
    }
    
    bool sendMessage(unsigned char* buffer, unsigned int size) override
    {
        return false;
    }
    void reset() override
    {
        ;
    }
    bool checkConnection() override
    {
        return false;

    }
};


class MockCommManagerParser : public CommManager
{
public:
    MockICommChannel channel;
    MockCommManagerParser() : CommManager(channel)
    {

    }

    void _initParser(unsigned char opcode[MessagesDefinitions::OPCODE_SIZE])
    {
        InitParser(opcode);
    }
    bool _parseBuffer(unsigned char* buffer, int size)
    {
        return ParseBuffer(buffer, size);
    }
    void _resetParser()
    {
        ResetParser();
    }
    unsigned char* getTheResultBuffer(unsigned int* bufferSize)
    {
        *bufferSize = m_messageSize;
        return m_messageBuffer;
    }
};


class TestCommManagerParser : public ::testing::Test {
protected:
    MockCommManagerParser parser;
    unsigned char* m_buffer;
    void SetUp() override {
        // Setup code
        unsigned short opCode = MessagesDefinitions::TestRequestMessageOpcode;
        parser._initParser(reinterpret_cast<unsigned char*>(&opCode));
    }

    void TearDown() override {
        // Teardown code
        if (nullptr != m_buffer)
        {
            delete[] m_buffer;
            m_buffer = nullptr;
        }
        parser._resetParser();
    }

    unsigned char* createMessage(TestRecipe recipe, TcaCalculation::sPointData* testData, unsigned int* bufferSize)
    {
        MessagesDefinitions::MessageHeader header;
        int dataSize;
        int size;
        int offset;
        bool messageSentSuccessfully = false;
        //Prepare the buffer we want to send
        //Get the buffer size
        dataSize = (recipe.numberOfPoints * sizeof(TcaCalculation::sPointData));
        size = sizeof(MessagesDefinitions::MessageHeader) + sizeof(TestRecipe) + dataSize;
        *bufferSize = size;
        //create the buffer
        unsigned char* buffer = new unsigned char[size];
        //check the buffer
        if (nullptr != buffer)
        {
            //Fill the header
            header.opcode = MessagesDefinitions::TestRequestMessageOpcode;
            header.dataSize = dataSize;

            //copy everything to the buffer
            //copy the header
            offset = sizeof(MessagesDefinitions::MessageHeader);

            //copy the recipe
            memcpy(buffer + offset, reinterpret_cast<unsigned char*>(&recipe), sizeof(TestRecipe));
            offset += sizeof(TestRecipe);

            //copy the data
            memcpy(buffer + offset, reinterpret_cast<unsigned char*>(testData), dataSize);

            header.crc = CRC32::calculate(buffer + sizeof(MessagesDefinitions::MessageHeader), size - sizeof(MessagesDefinitions::MessageHeader));

            memcpy(buffer, &header, sizeof(MessagesDefinitions::MessageHeader));


            //Send the message
            return buffer;
        }
        else
        {
            //Failed to send message! handle the error
        }
        return nullptr;
    }

};


#endif //SHIELD_ANCASTests_H