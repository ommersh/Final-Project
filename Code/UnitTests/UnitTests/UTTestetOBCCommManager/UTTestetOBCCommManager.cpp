#include "UTTestetOBCCommManager.h"
#include "TestUtils.h"


using ::testing::_;
using ::testing::DoAll; 
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::SetArgReferee;
bool comparePointData(TcaCalculation::sPointData pointA, TcaCalculation::sPointData pointB)
{
	bool equal = true;
	equal &= (pointA.r1x == pointB.r1x);
	equal &= (pointA.r1y == pointB.r1y);
	equal &= (pointA.r1z == pointB.r1z);
	equal &= (pointA.r2x == pointB.r2x);
	equal &= (pointA.r2y == pointB.r2y);
	equal &= (pointA.r2z == pointB.r2z);
	equal &= (pointA.time == pointB.time);
	equal &= (pointA.v1x == pointB.v1x);
	equal &= (pointA.v1y == pointB.v1y);
	equal &= (pointA.v1z == pointB.v1z);
	equal &= (pointA.v2x == pointB.v2x);
	equal &= (pointA.v2y == pointB.v2y);
	equal &= (pointA.v2z == pointB.v2z);
	return equal;
}
//Test a real test case
TEST_F(TestCommManagerParser, TEST_test_SimpleFullMessageInBuffer)
{
	//Let create the data!
	unsigned int bufferSize = 0;
	static const int DATA_SIZE = 200;
	TestRecipe recipe = { 0 };
	recipe.testID = 1;
	recipe.numberOfPoints = DATA_SIZE;
	TcaCalculation::sPointData testData[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
	{
		testData[i].r1x = i;
		testData[i].r1y = i;
		testData[i].r1z = i;
		testData[i].r2x = i;
		testData[i].r2y = i;
		testData[i].r2z = i;
		testData[i].time = i;
		testData[i].v1x = i;
		testData[i].v1y = i;
		testData[i].v1z = i;
		testData[i].v2x = i;
		testData[i].v2y = i;
		testData[i].v2z = i;
	}
	m_buffer = createMessage(recipe, testData, &bufferSize);
	EXPECT_FALSE(nullptr == m_buffer);

	//call the parser
	EXPECT_TRUE(parser._parseBuffer(m_buffer, bufferSize));

	//get the results buffer
	unsigned int resultBufferSize = 0;
	unsigned char* resultBuffer = nullptr;
	resultBuffer = parser.getTheResultBuffer(&resultBufferSize);
	EXPECT_FALSE(nullptr == resultBuffer);
	EXPECT_TRUE(resultBufferSize == bufferSize);

	unsigned int offset = 0;
	TestRecipe resultRecipe = { 0 };
	MessagesDefinitions::MessageHeader header;
	offset = 0;
	memcpy(reinterpret_cast<unsigned char*>(&header), resultBuffer + offset, MessagesDefinitions::MESSAGE_HEADER_SIZE);
	offset += MessagesDefinitions::MESSAGE_HEADER_SIZE;
	EXPECT_TRUE(header.opcode == MessagesDefinitions::TestRequestMessageOpcode);

	memcpy(reinterpret_cast<unsigned char*>(&resultRecipe), resultBuffer + offset, sizeof(TestRecipe));
	offset += sizeof(TestRecipe);
	EXPECT_EQ(resultRecipe.testID, recipe.testID);
	EXPECT_TRUE(0 == memcmp(reinterpret_cast<unsigned char*>(&resultRecipe), reinterpret_cast<unsigned char*>(&recipe), sizeof(TestRecipe)));

	//get the data
	int numberOfPoints = header.dataSize / sizeof(TcaCalculation::sPointData);
	EXPECT_EQ(numberOfPoints, DATA_SIZE);

	TcaCalculation::sPointData* ResultsPointsData = new TcaCalculation::sPointData[numberOfPoints];
	EXPECT_FALSE(nullptr == ResultsPointsData);

	memcpy(reinterpret_cast<unsigned char*>(ResultsPointsData), resultBuffer + offset, header.dataSize);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		EXPECT_TRUE(comparePointData(ResultsPointsData[i], testData[i]));
	}

	if (nullptr != ResultsPointsData)
	{
		delete[] ResultsPointsData;
	}
}

TEST_F(TestCommManagerParser, TEST_test_FragmentedMessage)
{
	//Let create the data!
	unsigned int bufferSize = 0;
	static const int DATA_SIZE = 200;
	TestRecipe recipe = { 0 };
	recipe.testID = 1;
	recipe.numberOfPoints = DATA_SIZE;
	TcaCalculation::sPointData testData[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
	{
		testData[i].r1x = i;
		testData[i].r1y = i;
		testData[i].r1z = i;
		testData[i].r2x = i;
		testData[i].r2y = i;
		testData[i].r2z = i;
		testData[i].time = i;
		testData[i].v1x = i;
		testData[i].v1y = i;
		testData[i].v1z = i;
		testData[i].v2x = i;
		testData[i].v2y = i;
		testData[i].v2z = i;
	}
	m_buffer = createMessage(recipe, testData, &bufferSize);
	EXPECT_FALSE(nullptr == m_buffer);

	//call the parser
	unsigned char* b = m_buffer;
	unsigned int size = 150;
	EXPECT_FALSE(parser._parseBuffer(b, size));
	b = m_buffer + size;
	EXPECT_TRUE(parser._parseBuffer(b, bufferSize - size));


	//get the results buffer
	unsigned int resultBufferSize = 0;
	unsigned char* resultBuffer = nullptr;
	resultBuffer = parser.getTheResultBuffer(&resultBufferSize);
	EXPECT_FALSE(nullptr == resultBuffer);
	EXPECT_TRUE(resultBufferSize == bufferSize);

	unsigned int offset = 0;
	TestRecipe resultRecipe = { 0 };
	MessagesDefinitions::MessageHeader header;
	offset = 0;
	memcpy(reinterpret_cast<unsigned char*>(&header), resultBuffer + offset, MessagesDefinitions::MESSAGE_HEADER_SIZE);
	offset += MessagesDefinitions::MESSAGE_HEADER_SIZE;
	EXPECT_TRUE(header.opcode == MessagesDefinitions::TestRequestMessageOpcode);

	memcpy(reinterpret_cast<unsigned char*>(&resultRecipe), resultBuffer + offset, sizeof(TestRecipe));
	offset += sizeof(TestRecipe);
	EXPECT_EQ(resultRecipe.testID, recipe.testID);
	EXPECT_TRUE(0 == memcmp(reinterpret_cast<unsigned char*>(&resultRecipe), reinterpret_cast<unsigned char*>(&recipe), sizeof(TestRecipe)));

	//get the data
	int numberOfPoints = header.dataSize / sizeof(TcaCalculation::sPointData);
	EXPECT_EQ(numberOfPoints, DATA_SIZE);

	TcaCalculation::sPointData* ResultsPointsData = new TcaCalculation::sPointData[numberOfPoints];
	EXPECT_FALSE(nullptr == ResultsPointsData);

	memcpy(reinterpret_cast<unsigned char*>(ResultsPointsData), resultBuffer + offset, header.dataSize);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		EXPECT_TRUE(comparePointData(ResultsPointsData[i], testData[i]));
	}

	if (nullptr != ResultsPointsData)
	{
		delete[] ResultsPointsData;
	}
}

TEST_F(TestCommManagerParser, TEST_test_BigFragmentedMessage)
{
	//Let create the data!
	unsigned int bufferSize = 0;
	static const int DATA_SIZE = 6500;
	TestRecipe recipe = { 0 };
	recipe.testID = 1;
	recipe.numberOfPoints = DATA_SIZE;
	TcaCalculation::sPointData testData[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
	{
		testData[i].r1x = i;
		testData[i].r1y = i;
		testData[i].r1z = i;
		testData[i].r2x = i;
		testData[i].r2y = i;
		testData[i].r2z = i;
		testData[i].time = i;
		testData[i].v1x = i;
		testData[i].v1y = i;
		testData[i].v1z = i;
		testData[i].v2x = i;
		testData[i].v2y = i;
		testData[i].v2z = i;
	}
	m_buffer = createMessage(recipe, testData, &bufferSize);
	EXPECT_FALSE(nullptr == m_buffer);


	//call the parser on a lot of fragmented pieces
	unsigned int fragMaxSize = 1500;
	unsigned int fragSize = 1500;
	unsigned int bufferOffset = 0;
	unsigned char* buffer = m_buffer + bufferOffset;
	bufferOffset += fragSize;
	while (false == parser._parseBuffer(buffer, fragSize) && (bufferOffset <= bufferSize) )
	{
		if (bufferSize - bufferOffset < fragMaxSize)
			fragSize = bufferSize - bufferOffset;
		else
			fragSize = fragMaxSize;
		
		buffer = m_buffer + bufferOffset;
		bufferOffset += fragSize;
	}

	//get the results buffer
	unsigned int resultBufferSize = 0;
	unsigned char* resultBuffer = nullptr;
	resultBuffer = parser.getTheResultBuffer(&resultBufferSize);
	EXPECT_FALSE(nullptr == resultBuffer);
	EXPECT_TRUE(resultBufferSize == bufferSize);

	unsigned int offset = 0;
	TestRecipe resultRecipe = { 0 };
	MessagesDefinitions::MessageHeader header;
	offset = 0;
	memcpy(reinterpret_cast<unsigned char*>(&header), resultBuffer + offset, MessagesDefinitions::MESSAGE_HEADER_SIZE);
	offset += MessagesDefinitions::MESSAGE_HEADER_SIZE;
	EXPECT_TRUE(header.opcode == MessagesDefinitions::TestRequestMessageOpcode);

	memcpy(reinterpret_cast<unsigned char*>(&resultRecipe), resultBuffer + offset, sizeof(TestRecipe));
	offset += sizeof(TestRecipe);
	EXPECT_EQ(resultRecipe.testID, recipe.testID);
	EXPECT_TRUE(0 == memcmp(reinterpret_cast<unsigned char*>(&resultRecipe), reinterpret_cast<unsigned char*>(&recipe), sizeof(TestRecipe)));

	//get the data
	int numberOfPoints = header.dataSize / sizeof(TcaCalculation::sPointData);
	EXPECT_EQ(numberOfPoints, DATA_SIZE);

	TcaCalculation::sPointData* ResultsPointsData = new TcaCalculation::sPointData[numberOfPoints];
	EXPECT_FALSE(nullptr == ResultsPointsData);

	memcpy(reinterpret_cast<unsigned char*>(ResultsPointsData), resultBuffer + offset, header.dataSize);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		EXPECT_TRUE(comparePointData(ResultsPointsData[i], testData[i]));
	}

	if (nullptr != ResultsPointsData)
	{
		delete[] ResultsPointsData;
	}
}

TEST_F(TestCommManagerParser, TEST_test_SimpleFullMessageInMiddleOfBuffer)
{
	//Let create the data!
	unsigned int bufferSize = 0;
	static const int DATA_SIZE = 200;
	TestRecipe recipe = { 0 };
	recipe.testID = 1;
	recipe.numberOfPoints = DATA_SIZE;
	TcaCalculation::sPointData testData[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
	{
		testData[i].r1x = i;
		testData[i].r1y = i;
		testData[i].r1z = i;
		testData[i].r2x = i;
		testData[i].r2y = i;
		testData[i].r2z = i;
		testData[i].time = i;
		testData[i].v1x = i;
		testData[i].v1y = i;
		testData[i].v1z = i;
		testData[i].v2x = i;
		testData[i].v2y = i;
		testData[i].v2z = i;
	}
	m_buffer = createMessage(recipe, testData, &bufferSize);
	EXPECT_FALSE(nullptr == m_buffer);
	//create new bigger buffer with some garbage
	static const int ADDITIONAL_SIZE = 1500;
	unsigned char* bigBuffer = new unsigned char[bufferSize + ADDITIONAL_SIZE];
	EXPECT_FALSE(nullptr == bigBuffer);
	//fil the start with garbage
	memset(bigBuffer, 0x22, ADDITIONAL_SIZE);
	//copy everything to the new buffer
	memcpy(bigBuffer + ADDITIONAL_SIZE, m_buffer, bufferSize);
	//call the parser
	EXPECT_TRUE(parser._parseBuffer(bigBuffer, bufferSize + ADDITIONAL_SIZE));

	//get the results buffer
	unsigned int resultBufferSize = 0;
	unsigned char* resultBuffer = nullptr;
	resultBuffer = parser.getTheResultBuffer(&resultBufferSize);
	EXPECT_FALSE(nullptr == resultBuffer);
	EXPECT_TRUE(resultBufferSize == bufferSize);

	unsigned int offset = 0;
	TestRecipe resultRecipe = { 0 };
	MessagesDefinitions::MessageHeader header;
	offset = 0;
	memcpy(reinterpret_cast<unsigned char*>(&header), resultBuffer + offset, MessagesDefinitions::MESSAGE_HEADER_SIZE);
	offset += MessagesDefinitions::MESSAGE_HEADER_SIZE;
	EXPECT_TRUE(header.opcode == MessagesDefinitions::TestRequestMessageOpcode);

	memcpy(reinterpret_cast<unsigned char*>(&resultRecipe), resultBuffer + offset, sizeof(TestRecipe));
	offset += sizeof(TestRecipe);
	EXPECT_EQ(resultRecipe.testID, recipe.testID);
	EXPECT_TRUE(0 == memcmp(reinterpret_cast<unsigned char*>(&resultRecipe), reinterpret_cast<unsigned char*>(&recipe), sizeof(TestRecipe)));

	//get the data
	int numberOfPoints = header.dataSize / sizeof(TcaCalculation::sPointData);
	EXPECT_EQ(numberOfPoints, DATA_SIZE);

	TcaCalculation::sPointData* ResultsPointsData = new TcaCalculation::sPointData[numberOfPoints];
	EXPECT_FALSE(nullptr == ResultsPointsData);

	memcpy(reinterpret_cast<unsigned char*>(ResultsPointsData), resultBuffer + offset, header.dataSize);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		EXPECT_TRUE(comparePointData(ResultsPointsData[i], testData[i]));
	}

	if (nullptr != ResultsPointsData)
	{
		delete[] ResultsPointsData;
	}
	if (nullptr == bigBuffer)
	{
		delete[] bigBuffer;
	}
}

TEST_F(TestCommManagerParser, TEST_test_FragmentedMessageOnHeader)
{
	for (int i = 0; i < MessagesDefinitions::MESSAGE_HEADER_SIZE; i++)
	{
		//Let create the data!
		unsigned int bufferSize = 0;
		static const int DATA_SIZE = 200;
		TestRecipe recipe = { 0 };
		recipe.testID = 1;
		recipe.numberOfPoints = DATA_SIZE;
		TcaCalculation::sPointData testData[DATA_SIZE] = { 0 };
		for (int i = 0; i < DATA_SIZE; i++)
		{
			testData[i].r1x = i;
			testData[i].r1y = i;
			testData[i].r1z = i;
			testData[i].r2x = i;
			testData[i].r2y = i;
			testData[i].r2z = i;
			testData[i].time = i;
			testData[i].v1x = i;
			testData[i].v1y = i;
			testData[i].v1z = i;
			testData[i].v2x = i;
			testData[i].v2y = i;
			testData[i].v2z = i;
		}
		m_buffer = createMessage(recipe, testData, &bufferSize);
		EXPECT_FALSE(nullptr == m_buffer);

		//call the parser
		unsigned char* b = m_buffer;
		unsigned int size = i;
		EXPECT_FALSE(parser._parseBuffer(b, size));
		b = m_buffer + size;
		EXPECT_TRUE(parser._parseBuffer(b, bufferSize - size));


		//get the results buffer
		unsigned int resultBufferSize = 0;
		unsigned char* resultBuffer = nullptr;
		resultBuffer = parser.getTheResultBuffer(&resultBufferSize);
		EXPECT_FALSE(nullptr == resultBuffer);
		EXPECT_TRUE(resultBufferSize == bufferSize);

		unsigned int offset = 0;
		TestRecipe resultRecipe = { 0 };
		MessagesDefinitions::MessageHeader header;
		offset = 0;
		memcpy(reinterpret_cast<unsigned char*>(&header), resultBuffer + offset, MessagesDefinitions::MESSAGE_HEADER_SIZE);
		offset += MessagesDefinitions::MESSAGE_HEADER_SIZE;
		EXPECT_TRUE(header.opcode == MessagesDefinitions::TestRequestMessageOpcode);

		memcpy(reinterpret_cast<unsigned char*>(&resultRecipe), resultBuffer + offset, sizeof(TestRecipe));
		offset += sizeof(TestRecipe);
		EXPECT_EQ(resultRecipe.testID, recipe.testID);
		EXPECT_TRUE(0 == memcmp(reinterpret_cast<unsigned char*>(&resultRecipe), reinterpret_cast<unsigned char*>(&recipe), sizeof(TestRecipe)));

		//get the data
		int numberOfPoints = header.dataSize / sizeof(TcaCalculation::sPointData);
		EXPECT_EQ(numberOfPoints, DATA_SIZE);

		TcaCalculation::sPointData* ResultsPointsData = new TcaCalculation::sPointData[numberOfPoints];
		EXPECT_FALSE(nullptr == ResultsPointsData);

		memcpy(reinterpret_cast<unsigned char*>(ResultsPointsData), resultBuffer + offset, header.dataSize);

		for (int i = 0; i < DATA_SIZE; i++)
		{
			EXPECT_TRUE(comparePointData(ResultsPointsData[i], testData[i]));
		}

		if (nullptr != ResultsPointsData)
		{
			delete[] ResultsPointsData;
			ResultsPointsData = nullptr;
		}
		if (nullptr != m_buffer)
		{
			delete[] m_buffer;
			m_buffer = nullptr;
		}
		parser._resetParser();
	}
}

TEST_F(TestCommManagerParser, TEST_test_WrongCRC)
{
	//Let create the data!
	unsigned int bufferSize = 0;
	static const int DATA_SIZE = 200;
	TestRecipe recipe = { 0 };
	recipe.testID = 1;
	recipe.numberOfPoints = DATA_SIZE;
	TcaCalculation::sPointData testData[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
	{
		testData[i].r1x = i;
		testData[i].r1y = i;
		testData[i].r1z = i;
		testData[i].r2x = i;
		testData[i].r2y = i;
		testData[i].r2z = i;
		testData[i].time = i;
		testData[i].v1x = i;
		testData[i].v1y = i;
		testData[i].v1z = i;
		testData[i].v2x = i;
		testData[i].v2y = i;
		testData[i].v2z = i;
	}
	MessagesDefinitions::MessageHeader header;
	//Create message with the wrong crc:
	m_buffer = createMessage(recipe, testData, &bufferSize);
	EXPECT_FALSE(nullptr == m_buffer);
	//Over write the header
	memcpy(&header, m_buffer, sizeof(MessagesDefinitions::MessageHeader));
	header.crc = header.crc - 0x1234;
	memcpy(m_buffer , &header, sizeof(MessagesDefinitions::MessageHeader));

	//call the parser
	EXPECT_FALSE(parser._parseBuffer(m_buffer, bufferSize));

}
