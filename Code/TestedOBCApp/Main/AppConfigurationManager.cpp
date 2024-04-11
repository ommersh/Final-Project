#include "AppConfigurationManager.h"
#include <iostream>



void AppConfigurationManager::init(const std::string& filename)
{
	INIReader reader(filename);
    std::string stringValue;

    setDefaults();
    if (reader.ParseError() < 0) {
        std::cerr << "Error parsing INI file!" << std::endl;
        return;
    }
    //Get the values
    stringValue = reader.Get("General", "CommChannelType", "LocalSimulation");
    if (stringValue == "LocalSimulation")
    {
        m_commChannelType = AppConfiguration::CommChannelType::LocalSimulation;
    }

    stringValue = reader.Get("General", "TimerType", "ChronoTimer");
    if (stringValue == "ChronoTimer")
    {
        m_timerType = AppConfiguration::TimerType::ChronoTimer;
    }




    stringValue = reader.Get("DefaultAlgParameters", "FullCatalogTestDataVariation", "OneWithAll");
    if (stringValue == "AllWithAll")
    {
        m_fullCatalogTestDataVariation = AppConfiguration::FullCatalogTestDataVariation::eAllWithAll;
    }
    else
    {
        m_fullCatalogTestDataVariation = AppConfiguration::FullCatalogTestDataVariation::eOneWithAll;
    }
    
    stringValue = reader.Get("DefaultAlgParameters", "FullCatalogTestTypeVariation", "PointsInInterval");
    if (stringValue == "TimeInterval")
    {
        m_fullCatalogTestTypeVariation = AppConfiguration::FullCatalogTestTypeVariation::eTimeInterval;
    }
    else if(stringValue == "CatchDegree")
    {
        m_fullCatalogTestTypeVariation = AppConfiguration::FullCatalogTestTypeVariation::eCatchDegree;
    }
    else
    {
        m_fullCatalogTestTypeVariation = AppConfiguration::FullCatalogTestTypeVariation::ePointsInInterval;
    }

    m_TOLd = reader.GetReal("DefaultAlgParameters", "SboAncasDefaultToldKm", AppConfiguration::SBO_ANCAS_TOL_D_KM);
    m_TOLt = reader.GetReal("DefaultAlgParameters", "SboAncasDefaultToltSec", AppConfiguration::SBO_ANCAS_TOL_T_SEC);
    m_minNumberOfPointsPerSegment = reader.GetInteger("DefaultAlgParameters", "minNumberOfPointsPerSegment", AppConfiguration::MIN_NUMBER_OF_POINTS_PER_SEGMENT);
    m_maxNumberOfPointsPerSegment = reader.GetInteger("DefaultAlgParameters", "maxNumberOfPointsPerSegment", AppConfiguration::MAX_NUMBER_OF_POINTS_PER_SEGMENT);
    m_numberOfIterations = reader.GetInteger("DefaultAlgParameters", "numberOfIterations", AppConfiguration::NUMBER_OF_ITERATIONS);
    m_timeFrameSizeInDays = reader.GetInteger("DefaultAlgParameters", "timeFrameSizeInDays", AppConfiguration::TIME_FRAME_SIZE_IN_DAYS);
    m_TminFactor = reader.GetInteger("DefaultAlgParameters", "TMinFactor", AppConfiguration::T_MIN_FACTOR);

    m_ancas = reader.GetInteger("DefaultAlgParameters", "ANCAS", 1);
    m_sboAncas = reader.GetInteger("DefaultAlgParameters", "SBOANCAS", 1);
    m_catch = reader.GetInteger("DefaultAlgParameters", "CATCH",  1);
}


void AppConfigurationManager::setDefaults()
{
    m_timerType = AppConfiguration::TimerType::ChronoTimer;
    m_fullCatalogTestDataVariation = AppConfiguration::FullCatalogTestDataVariation::eOneWithAll;
    m_TOLd = AppConfiguration::SBO_ANCAS_TOL_D_KM;
    m_TOLt = AppConfiguration::SBO_ANCAS_TOL_T_SEC;
    m_minNumberOfPointsPerSegment = AppConfiguration::MIN_NUMBER_OF_POINTS_PER_SEGMENT;
    m_maxNumberOfPointsPerSegment = AppConfiguration::MAX_NUMBER_OF_POINTS_PER_SEGMENT;
    m_numberOfIterations = AppConfiguration::NUMBER_OF_ITERATIONS;
    m_timeFrameSizeInDays = AppConfiguration::TIME_FRAME_SIZE_IN_DAYS;
    m_TminFactor = AppConfiguration::TIME_FRAME_SIZE_IN_DAYS;


    m_ancas = 1;
    m_sboAncas = 1;
    m_catch = 1;
}
