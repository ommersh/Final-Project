#include "AppConfigurationManager.h"
#include <iostream>



void AppConfigurationManager::init(const std::string& filename)
{
	INIReader reader(filename);
    std::string stringValue;
    double doubleValue;

    if (reader.ParseError() < 0) {
        std::cerr << "Error parsing INI file!" << std::endl;
        setDefaults();
        return;
    }
    //Get the values
    stringValue = reader.Get("General", "TimerType", "ChronoTimer");

    if (stringValue == "ChronoTimer")
    {
        m_timerType = AppConfiguration::TimerType::ChronoTimer;
    }

    stringValue = reader.Get("DefaultAlgParameters", "fullCatalogTestVariation", "OneWithAll");
    if (stringValue == "AllWithAll")
    {
        m_fullCatalogTestVariation = AppConfiguration::FullCatalogTestVariation::eAllWithAll;
    }
    else
    {
        m_fullCatalogTestVariation = AppConfiguration::FullCatalogTestVariation::eOneWithAll;
    }


    m_TOLd = reader.GetReal("DefaultAlgParameters", "SboAncasDefaultToldKm", AppConfiguration::SBO_ANCAS_TOL_D_KM);
    m_TOLt = reader.GetReal("DefaultAlgParameters", "SboAncasDefaultToltSec", AppConfiguration::SBO_ANCAS_TOL_T_SEC);
    m_minNumberOfPointsPerSegment = reader.GetInteger("DefaultAlgParameters", "minNumberOfPointsPerSegment", AppConfiguration::MIN_NUMBER_OF_POINTS_PER_SEGMENT);
    m_maxNumberOfPointsPerSegment = reader.GetInteger("DefaultAlgParameters", "maxNumberOfPointsPerSegment", AppConfiguration::MAX_NUMBER_OF_POINTS_PER_SEGMENT);
    m_numberOfIterations = reader.GetInteger("DefaultAlgParameters", "numberOfIterations", AppConfiguration::NUMBER_OF_ITERATIONS);
    m_timeFrameSizeInDays = reader.GetInteger("DefaultAlgParameters", "timeFrameSizeInDays", AppConfiguration::TIME_FRAME_SIZE_IN_DAYS);


}


void AppConfigurationManager::setDefaults()
{
    m_timerType = AppConfiguration::TimerType::ChronoTimer;
    m_fullCatalogTestVariation = AppConfiguration::FullCatalogTestVariation::eOneWithAll;
    m_TOLd = AppConfiguration::SBO_ANCAS_TOL_D_KM;
    m_TOLt = AppConfiguration::SBO_ANCAS_TOL_T_SEC;
    m_minNumberOfPointsPerSegment = AppConfiguration::MIN_NUMBER_OF_POINTS_PER_SEGMENT;
    m_maxNumberOfPointsPerSegment = AppConfiguration::MAX_NUMBER_OF_POINTS_PER_SEGMENT;
    m_numberOfIterations = AppConfiguration::NUMBER_OF_ITERATIONS;
    m_timeFrameSizeInDays = AppConfiguration::TIME_FRAME_SIZE_IN_DAYS;


}
