#include "AppConfigurationManager.h"
#include <iostream>
#include "EventLogger.h"


void AppConfigurationManager::Init(const std::string& filename)
{
	INIReader reader(filename);
    std::string stringValue;
    //Log the app configuration
    std::string logString = "";

    setDefaults();
    if (reader.ParseError() < 0) {
        std::cerr << "Error parsing Config INI file!" << std::endl;
        logString = "Error parsing Config INI file!";
        EventLogger::getInstance().log(logString, "AppConfigurationManager");
        logString = "Working with default configurations!";
        EventLogger::getInstance().log(logString, "AppConfigurationManager");
        return;
    }
    //Get the values
    //CommChannelType
    stringValue = reader.Get("General", "CommChannelType", "LocalSimulation");
    if (stringValue == "LocalSimulation")
    {
        m_commChannelType = AppConfiguration::CommChannelType::LocalSimulation;
    }
    else if (stringValue == "Tcp")
    {
        m_commChannelType = AppConfiguration::CommChannelType::Tcp;
    }
    else if (stringValue == "WinUdp")
    {
        m_commChannelType = AppConfiguration::CommChannelType::WinUdp;
    }

    logString = "CommChannelType: " + stringValue;
    EventLogger::getInstance().log(logString, "AppConfigurationManager");

    //TimerType
    stringValue = reader.Get("General", "TimerType", "ChronoTimer");
    if (stringValue == "ChronoTimer")
    {
        m_timerType = AppConfiguration::TimerType::ChronoTimer;
    }

    logString = "TimerType: " + stringValue;
    EventLogger::getInstance().log(logString, "AppConfigurationManager");

    //IP and Ports
    m_localPort = reader.GetInteger("General", "SourcePort", AppConfiguration::LOCAL_PORT);
    m_destPort = reader.GetInteger("General", "DestinationPort", AppConfiguration::DEST_PORT);
    m_localIpAddress = reader.Get("General", "LocalIpAddress", AppConfiguration::IP_ADDRESS);
    m_destIpAddress = reader.Get("General", "DestIpAddress", AppConfiguration::IP_ADDRESS);

    logString = "SourcePort: " + std::to_string(m_localPort);
    EventLogger::getInstance().log(logString, "AppConfigurationManager");
    logString = "DestinationPort: " + std::to_string(m_destPort);
    EventLogger::getInstance().log(logString, "AppConfigurationManager");
    logString = "LocalIpAddress: " + m_localIpAddress;
    EventLogger::getInstance().log(logString, "AppConfigurationManager");
    logString = "DestIpAddress: " + m_destIpAddress;
    EventLogger::getInstance().log(logString, "AppConfigurationManager");

    //Defaults local sim parameters
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




    EventLogger::getInstance().log("App Started", "AppConfigurationManager");
}


void AppConfigurationManager::setDefaults()
{
    m_localPort = AppConfiguration::LOCAL_PORT;
    m_destPort = AppConfiguration::DEST_PORT;
    m_localIpAddress = AppConfiguration::IP_ADDRESS;
    m_destIpAddress = AppConfiguration::IP_ADDRESS;


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

AppConfiguration::TimerType AppConfigurationManager::GetTimerType()
{
    return m_timerType;
}

AppConfiguration::CommChannelType AppConfigurationManager::GetCommChannelType()
{
    return m_commChannelType;
}

int AppConfigurationManager::GetSourcePort()
{
    return m_localPort;
}

int AppConfigurationManager::GetDestPort()
{
    return m_destPort;
}

std::string AppConfigurationManager::GetLocalIpAddress()
{
    return m_localIpAddress;
}

std::string AppConfigurationManager::GetDestIpAddress()
{
    return m_destIpAddress;
}

double AppConfigurationManager::GetTOLd()
{
    return m_TOLd;
}

double AppConfigurationManager::GetTOLt()
{
    return m_TOLt;
}

AppConfiguration::FullCatalogTestDataVariation AppConfigurationManager::GetFullCatalogTestDataVariation()
{
    return m_fullCatalogTestDataVariation;
}

AppConfiguration::FullCatalogTestTypeVariation AppConfigurationManager::GetFullCatalogTestTypeVariation()
{
    return m_fullCatalogTestTypeVariation;
}

int AppConfigurationManager::GetMinNumberOfPointsPerSegment()
{
    return m_minNumberOfPointsPerSegment;
}

int AppConfigurationManager::GetMaxNumberOfPointsPerSegment()
{
    return m_maxNumberOfPointsPerSegment;
}

int AppConfigurationManager::GetNumberOfIterations()
{
    return m_numberOfIterations;
}

int AppConfigurationManager::GetTimeFrameSizeInDays()
{
    return m_timeFrameSizeInDays;
}

int AppConfigurationManager::GetTminFactor()
{
    return m_TminFactor;
}

bool AppConfigurationManager::GetRunAncas()
{
    return m_ancas == 1;
}

bool AppConfigurationManager::GetRunSboAncas()
{
    return m_sboAncas == 1;
}

bool AppConfigurationManager::GetRunCatch()
{
    return m_catch == 1;
}