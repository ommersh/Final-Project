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


	//Default values
	//Tolt 10-5 Told 10-9
	static const double SBO_ANCAS_TOL_D_KM = 10e-9;
	static const double SBO_ANCAS_TOL_T_SEC = 10e-5;

	//FullCatalogTest parameters
	typedef enum
	{
		eAllWithAll,
		eOneWithAll
	}FullCatalogTestVariation;
	static const int MIN_NUMBER_OF_POINTS_PER_SEGMENT = 16;
	static const int MAX_NUMBER_OF_POINTS_PER_SEGMENT = 16;
	static const int NUMBER_OF_ITERATIONS = 1;
	static const int TIME_FRAME_SIZE_IN_DAYS = 7;
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
	double getTOLd()
	{
		return m_TOLd;
	}
	double getTOLt()
	{
		return m_TOLt;
	}
	AppConfiguration::FullCatalogTestVariation getFullCatalogTestVariation()
	{
		return m_fullCatalogTestVariation;
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

	/*get()
	{
		return;
	}*/
private:
	void setDefaults();

	double m_TOLd;
	double m_TOLt;

	AppConfiguration::TimerType m_timerType;


	//FullCatalogTest parameters
	AppConfiguration::FullCatalogTestVariation m_fullCatalogTestVariation;
	int m_minNumberOfPointsPerSegment;
	int m_maxNumberOfPointsPerSegment;
	int m_numberOfIterations;
	int m_timeFrameSizeInDays;
};









#endif //SHIELD_AppConfigurationManager_H