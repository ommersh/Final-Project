#include "FullCatalogTestDataGeneration.h"
#include "Factory.h"

bool FullCatalogTestDataGeneration::Init(const std::string& catalogFilePath)
{
	m_state = eStartCase;
	m_testID = 0;
	m_minNumberOfPointsInSegment = Factory::GetReference()->GetConfigurationManager()->GetMinNumberOfPointsPerSegment();
	m_maxNumberOfPointsInSegment = Factory::GetReference()->GetConfigurationManager()->GetMaxNumberOfPointsPerSegment();

	m_currentNumberOfPointsInSegment = m_minNumberOfPointsInSegment;
	m_firstObjectIndex = 0;
	m_secondObjectIndex = 1;

	m_numberOfiterations = Factory::GetReference()->GetConfigurationManager()->GetNumberOfIterations();
	m_numberOfDays = Factory::GetReference()->GetConfigurationManager()->GetTimeFrameSizeInDays();

	m_sboAncasTolDKm = Factory::GetReference()->GetConfigurationManager()->GetTOLd();
	m_sboAncasTolTSec = Factory::GetReference()->GetConfigurationManager()->GetTOLt();
	m_TMinFactor = Factory::GetReference()->GetConfigurationManager()->GetTminFactor();

	m_runAncas = Factory::GetReference()->GetConfigurationManager()->GetRunAncas();
	m_runSboAncas = Factory::GetReference()->GetConfigurationManager()->GetRunSboAncas();
	m_runCatch = Factory::GetReference()->GetConfigurationManager()->GetRunCatch();

	m_testVariation = Factory::GetReference()->GetConfigurationManager()->GetFullCatalogTestDataVariation();
	m_fullCatalogTestTypeVariation = Factory::GetReference()->GetConfigurationManager()->GetFullCatalogTestTypeVariation();
	if (m_fullCatalogTestTypeVariation == AppConfiguration::FullCatalogTestTypeVariation::eTimeInterval)
	{
		m_runCatch = false;//cant run catch with the changing number of points per interval
	}
	m_calculateWithSmallTimestep = false;
	m_timeStepSec = m_sboAncasTolTSec / 10;
	m_timeIntervalSec = 5;

	m_inputFile.open(catalogFilePath);

	if (!m_inputFile.is_open()) {
		std::cout << "Failed to open catalog file: " << catalogFilePath << std::endl;
		return false;
	}
	m_catalogSize = getCatalogSize();
	if (m_catalogSize < 2)
	{
		std::cout << "The Catalog Size is too small" << std::endl;
		return false;
	}
	switch (m_testVariation)
	{
	case AppConfiguration::FullCatalogTestDataVariation::eAllWithAll:
		m_numberOfCases = (m_catalogSize - 1) * m_catalogSize / 2.0 - 1;
		break;
	default:
	case AppConfiguration::FullCatalogTestDataVariation::eOneWithAll:
		m_numberOfCases = m_catalogSize - 1;
		break;
	}

	
	m_casesCounter = 0;
	std::cout << "The Catalog Size is: " << m_catalogSize << std::endl;
	return true;
}

void FullCatalogTestDataGeneration::getNextTestData(sFileData& fileData, TestRecipe& TestRecipe)
{
	switch (m_fullCatalogTestTypeVariation)
	{
	case AppConfiguration::FullCatalogTestTypeVariation::eTimeInterval:
		getNextTimeIntervalTestData(fileData, TestRecipe);
		break;
	case AppConfiguration::FullCatalogTestTypeVariation::eCatchDegree:
		getNextCatchDegreeTestData(fileData, TestRecipe);
		break;
	default:
	case AppConfiguration::FullCatalogTestTypeVariation::ePointsInInterval:
		getNextPointsInIntervalTestData(fileData, TestRecipe);
		break;
	}
}

void FullCatalogTestDataGeneration::getNextPointsInIntervalTestData(sFileData& fileData, TestRecipe& TestRecipe)
{
	//start: initialzie everything
//do everything for every catalog match
//start with the first objects
//start at degree = first
//crate data set for the degree
	switch (m_state)
	{
	case eStartCase:
		//We start by getting the 2 Elsetrec Objects for the current test
		initElsetrecObjects();
		m_currentNumberOfPointsInSegment = m_minNumberOfPointsInSegment;

	case eStartVariation:
		//free the memory
		clearMemory();
		generateDataSet();
		stateUpdate();
		break;
	case eAncas:
		getAncasData(fileData, TestRecipe);
		stateUpdate();
		break;
	case eCatch:
		getCatchData(fileData, TestRecipe);
		stateUpdate();
		break;
	case eSboAncas:
		getSboAncasData(fileData, TestRecipe);
	case eEndVariation:
		//we start by updating the segment size/Catch degree
		m_currentNumberOfPointsInSegment++;
		if (m_currentNumberOfPointsInSegment <= m_maxNumberOfPointsInSegment)
		{
			//continue with the current case
			m_state = eStartVariation;

		}
		else
		{
			//get ready for the next case
			moveToTheNextTest();
			printPercentage();

		}
		break;
	case eEnded:
	default:
		break;
	}
}


void FullCatalogTestDataGeneration::getNextTimeIntervalTestData(sFileData& fileData, TestRecipe& TestRecipe)
{
	//start: initialzie everything
	//do everything for every catalog match
	//start with the first objects
	//start at degree = first
	//crate data set for the degree
	static const int STARTING_NUMBER_OF_DAYS = 1;
	static const int MAX_NUMBER_OF_DAYS = 14;
	static const int POINTS_PER_INTERVAL = 6000;
	static const double SECONDS_IN_DAY = 24 * 60 * 60;
	static double segmentsPerDay = 1;

	switch (m_state)
	{
	case eStartCase:
		//We start by getting the 2 Elsetrec Objects for the current test
		initElsetrecObjects();
		generateDataSet();
		//we want the same number of points
		//so we will start with a large number per segment
		segmentsPerDay = SECONDS_IN_DAY / m_segmentSizeSec;
		m_numberOfDays = STARTING_NUMBER_OF_DAYS;
	case eStartVariation:
		//free the memory
		m_currentNumberOfPointsInSegment = POINTS_PER_INTERVAL/(segmentsPerDay* m_numberOfDays);
		clearMemory();
		generateDataSet();
		stateUpdate();
		break;
	case eAncas:
		getAncasData(fileData, TestRecipe);
		stateUpdate();
		break;
	case eCatch:
		getCatchData(fileData, TestRecipe);
		stateUpdate();
		break;
	case eSboAncas:
		getSboAncasData(fileData, TestRecipe);
	case eEndVariation:
		//we start by updating the segment size/Catch degree
		m_numberOfDays++;
		if (m_numberOfDays <= MAX_NUMBER_OF_DAYS)
		{
			//continue with the current case
			m_state = eStartVariation;

		}
		else
		{
			//get ready for the next case
			moveToTheNextTest();
			printPercentage();
		}
		break;
	case eEnded:
	default:
		break;
	}
}

void FullCatalogTestDataGeneration::getNextCatchDegreeTestData(sFileData& fileData, TestRecipe& TestRecipe)
{
	//start: initialzie everything
	//do everything for every catalog match
	//start with the first objects
	//start at degree = first
	//crate data set for the degree
	static const int DEGREES_ARRAY[3] = { 7,15,31 };
	static int index;
	switch (m_state)
	{
	case eStartCase:
		//We start by getting the 2 Elsetrec Objects for the current test
		initElsetrecObjects();
		index = 0;

	case eStartVariation:
		m_currentNumberOfPointsInSegment = DEGREES_ARRAY[index] + 1;
		//free the memory
		clearMemory();
		generateDataSet();
		stateUpdate();
		break;
	case eAncas:
		getAncasData(fileData, TestRecipe);
		stateUpdate();
		break;
	case eCatch:
		getCatchData(fileData, TestRecipe);
		stateUpdate();
		break;
	case eSboAncas:
		getSboAncasData(fileData, TestRecipe);
	case eEndVariation:
		//we start by updating the segment size/Catch degree
		index++;
		if (index < 3)
		{
			//continue with the current case
			m_state = eStartVariation;

		}
		else
		{
			//get ready for the next case
			moveToTheNextTest();
			printPercentage();

		}
		break;
	case eEnded:
	default:
		break;
	}
}




void FullCatalogTestDataGeneration::handleTestResults(TestResults::TestResult results)
{
	switch (results.testedAlgorithm)
	{
	case AlgorithmsEnums::Algorithm::SBO_ANCAS:
		if (true == m_calculateWithSmallTimestep)
		{
			calculateWithSmallTimestep(results.tca.time);
		}
		break;
	default:
		break;
	}
}

void FullCatalogTestDataGeneration::moveToTheNextTest()
{
	switch (m_testVariation)
	{
	case AppConfiguration::FullCatalogTestDataVariation::eAllWithAll:
		m_secondObjectIndex++;
		if (m_secondObjectIndex < m_catalogSize)
		{
			m_casesCounter++;
			m_state = eStartCase;
		}
		else
		{
			m_state = eEnded;
		}
		break;
	default:
	case AppConfiguration::FullCatalogTestDataVariation::eOneWithAll:
		m_secondObjectIndex++;
		if (m_secondObjectIndex < m_catalogSize)
		{
			m_casesCounter++;
			m_state = eStartCase;
		}
		else
		{
			m_firstObjectIndex++;
			if (m_firstObjectIndex < m_catalogSize)
			{
				m_secondObjectIndex = m_firstObjectIndex + 1;
				m_casesCounter++;
				m_state = eStartCase;
			}
			else
			{
				m_state = eEnded;
			}
		}
		break;
	}
	
}



void FullCatalogTestDataGeneration::initElsetrecObjects()
{
	std::string tempName;
	//Get the m_firstObjectIndex
	getTLE(m_firstObjectIndex);
	m_dataGenerator.InitOrbitalElementsFromTLE(m_tleLine1, m_tleLine2,m_elsetrec1);

	tempName = m_name;  // Copying m_name to tempName
	tempName += '_';    // Appending '_' to tempName

	//Get the m_secondObjectIndex
	getTLE(m_secondObjectIndex);
	m_dataGenerator.InitOrbitalElementsFromTLE(m_tleLine1, m_tleLine2, m_elsetrec2);
	//remove spaces from the name
	//m_name.erase(std::remove(m_name.begin(), m_name.end(), ' '), m_name.end());
	//m_name.erase(std::remove(m_name.begin(), m_name.end(), '\t'), m_name.end());

	tempName += m_name;  // Append m_name to tempName

	tempName.erase(std::remove(tempName.begin(), tempName.end(), ' '), tempName.end());
	tempName.erase(std::remove(tempName.begin(), tempName.end(), '\t'), tempName.end());
	tempName.erase(std::remove(tempName.begin(), tempName.end(), '\n'), tempName.end());

	//Get the test name
	strncpy(m_testName, tempName.c_str(), MAX_TEST_NAME_SIZE - 1);
}

void FullCatalogTestDataGeneration::getAncasData(sFileData& fileData, TestRecipe& TestRecipe)
{

	fileData.size = m_simpleDataGeneration.m_numberOfPoints;
	fileData.data = m_simpleDataGeneration.m_pointsDataANCAS;

	//create the test parameters
	TestRecipe.catchPolynomialDegree = 3;
	TestRecipe.numberOfPoints = fileData.size;
	TestRecipe.testedAlgorithm = AlgorithmsEnums::Algorithm::ANCAS;
	TestRecipe.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(TestRecipe.testName, MAX_TEST_NAME_SIZE, m_testName);
#else
	// Standard C function, less safe but portable
	strncpy(TestRecipe.testName, m_testName, MAX_TEST_NAME_SIZE);
	TestRecipe.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	TestRecipe.testID = m_testID++;
	TestRecipe.numberOfIterations = m_numberOfiterations;

	TestRecipe.elsetrec1 = m_elsetrec1;
	TestRecipe.elsetrec2 = m_elsetrec2;
	TestRecipe.startTime1Min = m_startDataElem1;
	TestRecipe.startTime2Min = m_startDataElem2;

	TestRecipe.numberOfPointsPerSegment = m_currentNumberOfPointsInSegment;
	TestRecipe.segmentSizeSec = m_segmentSizeSec;
	TestRecipe.timeIntervalSizeSec = m_numberOfDays * 24 * 60 * 60;
	TestRecipe.TminFactor = m_TMinFactor;
}

void FullCatalogTestDataGeneration::getCatchData(sFileData& fileData, TestRecipe& TestRecipe)
{

	fileData.size = m_simpleDataGeneration.m_numberOfPoints;
	fileData.data = m_simpleDataGeneration.m_pointsDataCATCH;

	TestRecipe.catchPolynomialDegree = m_currentNumberOfPointsInSegment - 1;
	TestRecipe.numberOfPoints = fileData.size;
	TestRecipe.testedAlgorithm = AlgorithmsEnums::Algorithm::CATCH;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(TestRecipe.testName, MAX_TEST_NAME_SIZE, m_testName);
#else
	// Standard C function, less safe but portable
	strncpy(TestRecipe.testName, m_testName, MAX_TEST_NAME_SIZE);
	TestRecipe.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	TestRecipe.testID = m_testID++;
	TestRecipe.numberOfIterations = m_numberOfiterations;
	TestRecipe.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;

	TestRecipe.elsetrec1 = m_elsetrec1;
	TestRecipe.elsetrec2 = m_elsetrec2;
	TestRecipe.startTime1Min = m_startDataElem1;
	TestRecipe.startTime2Min = m_startDataElem2;

	TestRecipe.numberOfPointsPerSegment = m_currentNumberOfPointsInSegment;
	TestRecipe.segmentSizeSec = m_segmentSizeSec;
	TestRecipe.timeIntervalSizeSec = m_numberOfDays * 24 * 60 * 60;
	TestRecipe.TminFactor = m_TMinFactor;
}

void FullCatalogTestDataGeneration::getSboAncasData(sFileData& fileData, TestRecipe& TestRecipe)
{

	fileData.size = m_simpleDataGeneration.m_numberOfPoints;
	fileData.data = m_simpleDataGeneration.m_pointsDataANCAS;

	//create the test parameters
	TestRecipe.catchPolynomialDegree = 3;
	TestRecipe.numberOfPoints = fileData.size;
	TestRecipe.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS;
	TestRecipe.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(TestRecipe.testName, MAX_TEST_NAME_SIZE, m_testName);
#else
	// Standard C function, less safe but portable
	strncpy(TestRecipe.testName, m_testName, MAX_TEST_NAME_SIZE);
	TestRecipe.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	TestRecipe.testID = m_testID++;
	TestRecipe.numberOfIterations = m_numberOfiterations;

	TestRecipe.TOLd = m_sboAncasTolDKm;
	TestRecipe.TOLt = m_sboAncasTolTSec;

	TestRecipe.elsetrec1 = m_elsetrec1;
	TestRecipe.elsetrec2 = m_elsetrec2;
	TestRecipe.startTime1Min = m_startDataElem1;
	TestRecipe.startTime2Min = m_startDataElem2;

	TestRecipe.numberOfPointsPerSegment = m_currentNumberOfPointsInSegment;
	TestRecipe.segmentSizeSec = m_segmentSizeSec;
	TestRecipe.timeIntervalSizeSec = m_numberOfDays * 24 * 60 * 60;
	TestRecipe.TminFactor = m_TMinFactor;
}

void FullCatalogTestDataGeneration::generateDataSet()
{
	//Generate the data
	m_simpleDataGeneration.GenearateDataFromElsetrec(m_numberOfDays, m_currentNumberOfPointsInSegment, m_elsetrec1, m_elsetrec2, m_startDataElem1, m_startDataElem2, m_segmentSizeSec);
}

void FullCatalogTestDataGeneration::clearMemory()
{
	if (nullptr != m_simpleDataGeneration.m_pointsDataANCAS)
	{
		delete[] m_simpleDataGeneration.m_pointsDataANCAS;
	}
	if (nullptr != m_simpleDataGeneration.m_pointsDataCATCH)
	{
		delete[] m_simpleDataGeneration.m_pointsDataCATCH;
	}
}



void FullCatalogTestDataGeneration::getTLE(int position) {
	if (!m_inputFile.is_open()) {
		std::cerr << "File is not open" << std::endl;
		return;
	}

	m_inputFile.clear();  // Clear any error flags
	m_inputFile.seekg(0, std::ios::beg);  // Seek to the beginning of the file

	std::string line;
	int lineCount = 0;
	int satelliteCount = 0;

	while (getline(m_inputFile, line)) {


		if (lineCount++ % 3 == 0) {
			if (satelliteCount++ == position) {
				// Copy the satellite name to the global variable
				m_name = line;
				if (getline(m_inputFile, line)) {
					strncpy(m_tleLine1, line.c_str(), sizeof(m_tleLine1) - 1);
					m_tleLine1[sizeof(m_tleLine1) - 1] = '\0';

					if (getline(m_inputFile, line)) {
						strncpy(m_tleLine2, line.c_str(), sizeof(m_tleLine2) - 1);
						m_tleLine2[sizeof(m_tleLine2) - 1] = '\0';
					}
					else {
						std::cerr << "Failed to read TLE line 2" << std::endl;
					}
				}
				else {
					std::cerr << "Failed to read TLE line 1" << std::endl;
				}
				return;
			}
		}
	}

	if (satelliteCount < position) {
		std::cerr << "Position exceeds number of satellites in file" << std::endl;
	}
}

int FullCatalogTestDataGeneration::getCatalogSize()
{
	if (!m_inputFile.is_open()) {
		std::cerr << "File is not open" << std::endl;
		return 0;
	}

	m_inputFile.clear();  // Clear any error flags
	m_inputFile.seekg(0, std::ios::beg);  // Seek to the beginning of the file

	std::string line;
	int lineCount = 0;
	int satelliteCount = 0;
	while (getline(m_inputFile, line)) {
		if (lineCount++ % 3 == 2) {
			satelliteCount++;

		}
	}
	return satelliteCount;
}

void FullCatalogTestDataGeneration::printPercentage()
{
	double percentage = static_cast<double>(m_casesCounter) / m_numberOfCases * 100.0;

	// Move cursor to the beginning of the line with '\r'
	// Set text color to green with ANSI escape code "\033[32m"
	// Reset text color to default with "\033[0m" at the end
	std::cout << "\r\033[32m" // Set text color to green
		<< std::fixed << std::setprecision(10) << percentage << "%"
		<< "\033[0m" // Reset text color to default
		<< std::flush; // Flush the stream to ensure immediate output
}


void FullCatalogTestDataGeneration::calculateWithSmallTimestep(double timePoint)
{
	TestResults::TestResult results = { 0 };
	results.numberOfRuns = 1;
	results.catchRootsAlg = AlgorithmsEnums::CatchRootsAlg::EigenCompanionMatrix;
	results.testedAlgorithm = AlgorithmsEnums::Algorithm::SBO_ANCAS;
	results.testID = m_testID++;
	results.degree = 0;
	//Find the real TCA
	Factory::GetReference()->GetTimer()->startTimer();
	results.tca = m_simpleDataGeneration.FindTcaWithSmallTimeStepArountPoint(m_elsetrec1, m_elsetrec2, m_startDataElem1, m_startDataElem2, m_timeStepSec, timePoint, m_timeIntervalSec);
	Factory::GetReference()->GetTimer()->stopTimer();
	results.runTimeMicro = Factory::GetReference()->GetTimer()->getTimeInMicroSec();
	results.minTimeMicro = results.runTimeMicro;
	results.avgTimeMicro = results.runTimeMicro;

#ifdef _WIN32
	// Safe function available on Windows
	strcpy_s(results.testName, MAX_TEST_NAME_SIZE, m_testName);
#else
	// Standard C function, less safe but portable
	strncpy(results.testName, m_testName, MAX_TEST_NAME_SIZE);
	results.testName[MAX_TEST_NAME_SIZE - 1] = '\0'; // Ensure null-termination
#endif
	Factory::GetReference()->GetResultsLogger()->log(results, m_timeIntervalSec, m_timeStepSec, "SmallTimeStep");
}

void FullCatalogTestDataGeneration::stateUpdate()
{
	switch (m_state)
	{
	case eStartCase:
	case eStartVariation:
		if (m_runAncas)
			m_state = eAncas;
		else if (m_runCatch)
			m_state = eCatch;
		else if (m_runSboAncas)
			m_state = eSboAncas;
		else
			m_state = eEndVariation;
		break;
	case eAncas:
		if (m_runCatch)
			m_state = eCatch;
		else if (m_runSboAncas)
			m_state = eSboAncas;
		else
			m_state = eEndVariation;
		break;
	case eCatch:
		if (m_runSboAncas)
			m_state = eSboAncas;
		else
			m_state = eEndVariation;
		break;
	case eSboAncas:
		m_state = eEndVariation;
		break;
	case eEndVariation:
	case eEnded:
	default:
		break;
	}
}