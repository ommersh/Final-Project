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
		LocalSimulation
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

}



/// <summary>
/// Class to manage app configureations(like type of timer)
/// </summary>
class AppConfigurationManager
{
public:
	void init(const std::string& filename);
	AppConfiguration::TimerType getTimerType()
	{
		return m_timerType;
	}

	AppConfiguration::CommChannelType getCommChannelType()
	{
		return m_commChannelType;
	}

	double getTOLd()
	{
		return m_TOLd;
	}

	double getTOLt()
	{
		return m_TOLt;
	}

	AppConfiguration::FullCatalogTestDataVariation getFullCatalogTestDataVariation()
	{
		return m_fullCatalogTestDataVariation;
	}

	AppConfiguration::FullCatalogTestTypeVariation getFullCatalogTestTypeVariation()
	{
		return m_fullCatalogTestTypeVariation;
	}

	int getMinNumberOfPointsPerSegment()
	{
		return m_minNumberOfPointsPerSegment;
	}

	int getMaxNumberOfPointsPerSegment()
	{
		return m_maxNumberOfPointsPerSegment;
	}

	int getNumberOfIterations()
	{
		return m_numberOfIterations;
	}

	int getTimeFrameSizeInDays()
	{
		return m_timeFrameSizeInDays;
	}

	int getTminFactor()
	{
		return m_TminFactor;
	}

	bool getRunAncas()
	{
		return m_ancas == 1;
	}

	bool getRunSboAncas()
	{
		return m_sboAncas == 1;
	}

	bool getRunCatch()
	{
		return m_catch == 1;
	}
	/*get()
	{
		return;
	}*/
private:
	void setDefaults();

	double m_TOLd;
	double m_TOLt;

	AppConfiguration::TimerType m_timerType;
	AppConfiguration::CommChannelType m_commChannelType;

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