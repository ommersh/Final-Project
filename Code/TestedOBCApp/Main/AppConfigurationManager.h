#ifndef SHIELD_AppConfigurationManager_H    // Check if the symbol SHIELD_AppConfigurationManager_H is not defined
#define SHIELD_AppConfigurationManager_H    // Define the symbol SHIELD_AppConfigurationManager_H
#include "ini.h"
#include "INIReader.h"


namespace AppConfiguration
{
	enum TimerType
	{
		ChronoTimer
	};

	enum CommChannelType
	{
		LocalSimulation,
		Tcp,
		WinUdp,
	};

	//Default values
	//Tolt 10-5 Told 10-9
	static const double SBO_ANCAS_TOL_D_KM = 10e-9;
	static const double SBO_ANCAS_TOL_T_SEC = 10e-5;

	//FullCatalogTest parameters
	enum FullCatalogTestDataVariation
	{
		eAllWithAll,
		eOneWithAll
	};

	enum FullCatalogTestTypeVariation
	{
		ePointsInInterval,
		eTimeInterval,
		eCatchDegree
	};


	static const int MIN_NUMBER_OF_POINTS_PER_SEGMENT = 16;
	static const int MAX_NUMBER_OF_POINTS_PER_SEGMENT = 16;
	static const int NUMBER_OF_ITERATIONS = 1;
	static const int TIME_FRAME_SIZE_IN_DAYS = 7;
	static const int T_MIN_FACTOR= 2;

	static const int LOCAL_PORT = 8888;
	static const int DEST_PORT = 8889;
	static const std::string IP_ADDRESS = "127.0.0.1";
}



/// <summary>
/// Class to manage app configureations(like type of timer)
/// </summary>
class AppConfigurationManager
{
public:
	void Init(const std::string& filename);
	AppConfiguration::TimerType GetTimerType();

	AppConfiguration::CommChannelType GetCommChannelType();

	int GetSourcePort();

	int GetDestPort();

	std::string GetLocalIpAddress();

	std::string GetDestIpAddress();

	//For the internal sim
	double GetTOLd();
	//For the internal sim
	double GetTOLt();
	//For the internal sim
	AppConfiguration::FullCatalogTestDataVariation GetFullCatalogTestDataVariation();
	//For the internal sim
	AppConfiguration::FullCatalogTestTypeVariation GetFullCatalogTestTypeVariation();
	//For the internal sim
	int GetMinNumberOfPointsPerSegment();
	//For the internal sim
	int GetMaxNumberOfPointsPerSegment();
	//For the internal sim
	int GetNumberOfIterations();
	//For the internal sim
	int GetTimeFrameSizeInDays();
	//For the internal sim
	int GetTminFactor();
	//For the internal sim
	bool GetRunAncas();
	//For the internal sim
	bool GetRunSboAncas();
	//For the internal sim
	bool GetRunCatch();

private:
	void setDefaults();

	double m_TOLd;
	double m_TOLt;

	AppConfiguration::TimerType m_timerType;
	AppConfiguration::CommChannelType m_commChannelType;

	//Connection params
	int m_localPort;
	int m_destPort;
	std::string m_localIpAddress;
	std::string m_destIpAddress;


	//FullCatalogTest parameters
	AppConfiguration::FullCatalogTestDataVariation m_fullCatalogTestDataVariation;
	AppConfiguration::FullCatalogTestTypeVariation m_fullCatalogTestTypeVariation;
	int m_minNumberOfPointsPerSegment;
	int m_maxNumberOfPointsPerSegment;
	int m_numberOfIterations;
	int m_timeFrameSizeInDays;
	int m_TminFactor;

	int m_ancas;
	int m_sboAncas;
	int m_catch;


};









#endif //SHIELD_AppConfigurationManager_H