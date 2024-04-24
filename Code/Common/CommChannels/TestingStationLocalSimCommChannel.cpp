#include "TestingStationLocalSimCommChannel.h"

#include "ITcaAlgorithm.h"
#include "IRootsFindAlg.h"
#include "SBO_ANCAS/ISinglePointPropogator.h"
#include "SBO_ANCAS/SboAncas.h"
#include "ANCAS/ANCAS.h"
#include "CATCH/CATCH.h"
#include "CATCH/CompanionMatrixRootsFinderEigen.h"
#include "SBO_ANCAS/SGP4SinglePointGenerator.h"



bool TestingStationLocalSimCommChannel::getNextMessage(unsigned char* buffer, unsigned int maxSize, unsigned int* size)
{
	bool messageReceived = false;
	if (maxSize >= sizeof(MessagesDefinitions::TestResultsMessage))
	{
		if (true == m_runLocalTest)
		{
			runTest();
			m_runLocalTest = false;
			messageReceived = true;
			m_resultsMessage.header.opcode = MessagesDefinitions::TestResultsMessageOpcode;
			m_resultsMessage.header.dataSize = 0;
			*size = sizeof(MessagesDefinitions::TestResultsMessage);
			memcpy(buffer, reinterpret_cast<unsigned char*>(&m_resultsMessage), *size);
		}


	}
	return messageReceived;

}


bool TestingStationLocalSimCommChannel::sendMessage(unsigned char* buffer, unsigned int size)
{

	unsigned int offset = 0;

	if (size >= (sizeof(MessagesDefinitions::MessageHeader) + sizeof(TestRecipe)))
	{
		//get the message header
		MessagesDefinitions::MessageHeader header;
		memcpy(&header, buffer, sizeof(MessagesDefinitions::MessageHeader));
		offset += sizeof(MessagesDefinitions::MessageHeader);
		if (header.opcode == MessagesDefinitions::TestRequestMessageOpcode)
		{

			memcpy(reinterpret_cast<unsigned char*>(&m_testRecipe), buffer + offset, sizeof(TestRecipe));
			offset += sizeof(TestRecipe);
			m_pointsData = new TcaCalculation::sPointData[header.dataSize / sizeof(TcaCalculation::sPointData)];
			int expectedSize = header.dataSize;
			if (nullptr != m_pointsData && (size - offset) == expectedSize)
			{
				memcpy(reinterpret_cast<unsigned char*>(m_pointsData), buffer + offset, size - offset);
				m_runLocalTest = true;
			}
		}
	}
	return true;
}

void TestingStationLocalSimCommChannel::reset()
{
	m_runLocalTest = false;
}


void TestingStationLocalSimCommChannel::runTest()
{
	CATCH algCatch;
	CompanionMatrixRootsFinderEigen companionMatrixRootsFinderEigen;
	SGP4SinglePointGenerator singlePointGenerator;
	ANCAS algAncas;
	SboAncas algSboAncas;
	m_resultsMessage.results = { 0 };
	ITcaAlgorithm* Algoritm;

	m_resultsMessage.results.degree = m_testRecipe.catchPolynomialDegree;
	m_resultsMessage.results.testedAlgorithm = m_testRecipe.testedAlgorithm;
	m_resultsMessage.results.catchRootsAlg = m_testRecipe.catchRootsAlg;
	m_resultsMessage.results.testID = m_testRecipe.testID;
	m_resultsMessage.results.numberOfRuns = m_testRecipe.numberOfIterations;

	m_resultsMessage.results.segmentSizeSec = m_testRecipe.segmentSizeSec;
	m_resultsMessage.results.timeIntervalSizeSec = m_testRecipe.timeIntervalSizeSec;
	m_resultsMessage.results.numberOfPointsPerSegment = m_testRecipe.numberOfPointsPerSegment;
	m_resultsMessage.results.initialNumberOfPoints = m_testRecipe.numberOfPoints;
	m_resultsMessage.results.TminFactor = m_testRecipe.TminFactor;

	memcpy(m_resultsMessage.results.testName, m_testRecipe.testName, MAX_TEST_NAME_SIZE);

	switch (m_testRecipe.testedAlgorithm)
	{
	default:
	case AlgorithmsEnums::Algorithm::ANCAS:
		Algoritm = &algAncas;
		break;
	case AlgorithmsEnums::Algorithm::CATCH:
		algCatch.init(&companionMatrixRootsFinderEigen, m_testRecipe.catchPolynomialDegree);
		Algoritm = &algCatch;
		break;
	case AlgorithmsEnums::Algorithm::SBO_ANCAS:
		singlePointGenerator.init(m_testRecipe.elsetrec1, m_testRecipe.elsetrec2, m_testRecipe.startTime1Min, m_testRecipe.startTime2Min);
		algSboAncas.init(&singlePointGenerator, m_testRecipe.TOLd, m_testRecipe.TOLt);
		Algoritm = &algSboAncas;
		break;
	}

	startTimer();

	m_resultsMessage.results.tca = Algoritm->runAlgorithm(m_pointsData, m_testRecipe.numberOfPoints - 1);

	stopTimer();

	m_resultsMessage.results.runTimeMicro = getTimeInMicroSec();

	long double runTimeMicro;
	long double avgTimeMicro = m_resultsMessage.results.runTimeMicro;
	long double minTimeMicro = m_resultsMessage.results.runTimeMicro;
	//Do additional iterations
	TcaCalculation::TCA tca;

	for (unsigned int i = 1; i < m_testRecipe.numberOfIterations; i++)
	{
		switch (m_testRecipe.testedAlgorithm)
		{
		default:
		case AlgorithmsEnums::Algorithm::ANCAS:
			Algoritm = &algAncas;
			break;
		case AlgorithmsEnums::Algorithm::CATCH:
			algCatch.init(&companionMatrixRootsFinderEigen, m_testRecipe.catchPolynomialDegree);
			Algoritm = &algCatch;
			break;
		case AlgorithmsEnums::Algorithm::SBO_ANCAS:
			singlePointGenerator.init(m_testRecipe.elsetrec1, m_testRecipe.elsetrec2, m_testRecipe.startTime1Min, m_testRecipe.startTime2Min);
			algSboAncas.init(&singlePointGenerator, m_testRecipe.TOLd, m_testRecipe.TOLt);
			Algoritm = &algSboAncas;
			break;
		}
		startTimer();

		tca = Algoritm->runAlgorithm(m_pointsData, m_testRecipe.numberOfPoints - 1);

		stopTimer();

		runTimeMicro = getTimeInMicroSec();

		if (runTimeMicro < minTimeMicro)
		{
			minTimeMicro = runTimeMicro;
		}
		avgTimeMicro += runTimeMicro;
	}
	m_resultsMessage.results.avgTimeMicro = avgTimeMicro / m_testRecipe.numberOfIterations;
	m_resultsMessage.results.minTimeMicro = minTimeMicro;
}


#include <chrono>
void TestingStationLocalSimCommChannel::startTimer()
{
	m_endTime = -1;
	m_timeInMicro = -1;
	// Get the current time point
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	// Get the duration in microseconds
	auto duration = now.time_since_epoch();
	long int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	m_startTime = microseconds;
}

void TestingStationLocalSimCommChannel::stopTimer()
{
	// Get the current time point
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	// Get the duration in microseconds
	auto duration = now.time_since_epoch();
	long int microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	m_endTime = microseconds;

	m_timeInMicro = m_endTime - m_startTime;
}

double TestingStationLocalSimCommChannel::getTimeInMicroSec()
{
	return m_timeInMicro;
}