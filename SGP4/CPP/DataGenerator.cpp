#include <string>
#include "SGP4.h"

#include <fstream>
#include <vector>
#include "DataGenerator.h"
#include <TcaCalculation.h>

static const double PI = 3.14159265358979323846;

/// <summary>
/// this method take 2 orbiting elements, and save in CSV file the following data:
/// Point of time, Distance and Velocity vectors of the elements on each point of time, Vector F - square of the Euclidean distance between two points, df, 
/// </summary>
/// <param name="timeInMinutes"></param>
/// <param name="timePointsArrLength"></param>
/// <param name="elsetrec1"></param>
/// <param name="elsetrec2"></param>
/// <param name=""></param>
/// <param name="fileName"></param>
/// <param name=""></param>
/// <param name=""></param>
/// <param name=""></param>
void DataGenerator::GenearateDiffVectorFor2OrbitalElementsCSV(int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, std::string& fileName, TcaCalculation::sPointData elementsVectors[], double& startTime1, double& startTime2)
{
    CalculateRelativeVectorsForTwoElements(timePointsArrLength, elsetrec1, elsetrec2, elementsVectors, startTime1, startTime2);
    saveDataInCSVFile(timePointsArrLength, fileName, elementsVectors);
}

void DataGenerator::CalculateRelativeVectorsForTwoElements(int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, TcaCalculation::sPointData elementsVectors[], double& startTime1, double& startTime2)
{
    double startTimeElem1;
    double startTimeElem2;

    GetStartTimeOfOrbElem(elsetrec1, elsetrec2, startTimeElem1, startTimeElem2);
    //todo add starttime to recipe

    double r1[3], v1[3];
    double r2[3], v2[3];

    // Compute position and velocity for each time point
    for (int i = 0; i < timePointsArrLength; ++i) {
        SGP4Funcs::sgp4(elsetrec1, startTimeElem1 + elementsVectors[i].time, r1, v1);
        SGP4Funcs::sgp4(elsetrec2, startTimeElem2 + elementsVectors[i].time, r2, v2);

        elementsVectors[i].r1x = r1[0];
        elementsVectors[i].r1y = r1[1];
        elementsVectors[i].r1z = r1[2];
        elementsVectors[i].r2x = r2[0];
        elementsVectors[i].r2y = r2[1];
        elementsVectors[i].r2z = r2[2];
        elementsVectors[i].v1x = r1[0];
        elementsVectors[i].v1y = v1[1];
        elementsVectors[i].v1z = v1[2];
        elementsVectors[i].v2x = v2[0];
        elementsVectors[i].v2y = v2[1];
        elementsVectors[i].v2z = v2[2];
    }
}

/// <summary>
/// this function converts the Omm (Orbit Mean-Element Message) xml file data to
/// variables and initializes the sgp4 variables.
/// </summary>
/// <param name="xmlFile"></param>
/// <param name="satrec"></param>
void DataGenerator::InitOrbitalElementsFromXml(std::string& xmlFile, elsetrec& satrec)
{
    const double deg2rad = PI / 180.0;
    const double minToRadian = 720.0 / PI;
    const double ndotUnits = 1036800.0 / PI;
    const double nddotUnits = 2985984000.0 / 2.0 / PI;


    // Read XML file
    std::ifstream file(xmlFile);
    std::string xmlContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Find relevant data from XML
    std::string objectID, noradCatID, epoch, meanMotion, eccentricity, inclination, raOfAscNode, argOfPerigee, meanAnomaly, revAtEpoch, bstar, classificationType, meanMotionDot, meanMotionDDot, setNum, ephtype;

    // EPHEMERIS_TYPE
    ephtype = GetDataFromXmlNode(xmlContent, "EPHEMERIS_TYPE");
    satrec.ephtype = std::stoi(ephtype);

    // Object ID
    objectID = GetDataFromXmlNode(xmlContent, "OBJECT_ID");
    strncpy_s(satrec.intldesg, objectID.c_str(), 10);
    satrec.intldesg[10] = '\0';

    // NORAD Catalog ID
    noradCatID = GetDataFromXmlNode(xmlContent, "NORAD_CAT_ID");
    strncpy_s(satrec.satnum, noradCatID.c_str(), 5);
    satrec.satnum[5] = '\0';

    // Mean Motion
    meanMotion = GetDataFromXmlNode(xmlContent, "MEAN_MOTION");
    satrec.no_kozai = std::stod(meanMotion);

    // Eccentricity
    eccentricity = GetDataFromXmlNode(xmlContent, "ECCENTRICITY");
    satrec.ecco = std::stod(eccentricity);

    // Inclination
    inclination = GetDataFromXmlNode(xmlContent, "INCLINATION");
    satrec.inclo = std::stod(inclination);

    // RA of Ascending Node
    raOfAscNode = GetDataFromXmlNode(xmlContent, "RA_OF_ASC_NODE");
    satrec.nodeo = std::stod(raOfAscNode);

    // Argument of Perigee
    argOfPerigee = GetDataFromXmlNode(xmlContent, "ARG_OF_PERICENTER");
    satrec.argpo = std::stod(argOfPerigee);

    // Mean Anomaly
    meanAnomaly = GetDataFromXmlNode(xmlContent, "MEAN_ANOMALY");
    satrec.mo = std::stod(meanAnomaly);

    // Revolution at Epoch
    revAtEpoch = GetDataFromXmlNode(xmlContent, "REV_AT_EPOCH");
    satrec.revnum = std::stod(revAtEpoch);

    // B* (Bstar)
    bstar = GetDataFromXmlNode(xmlContent, "BSTAR");
    satrec.bstar = std::stod(bstar);

    // Classification Type
    classificationType = GetDataFromXmlNode(xmlContent, "CLASSIFICATION_TYPE");
    satrec.classification = classificationType[0];

    // Mean motion dot Type
    meanMotionDot = GetDataFromXmlNode(xmlContent, "MEAN_MOTION_DOT");
    satrec.ndot = std::stod(meanMotionDot);

    // Mean motion double dot Type
    meanMotionDDot = GetDataFromXmlNode(xmlContent, "MEAN_MOTION_DDOT");
    satrec.nddot = std::stod(meanMotionDDot);

    // Set number
    setNum = GetDataFromXmlNode(xmlContent, "ELEMENT_SET_NO");
    satrec.elnum = std::stol(setNum);

    // Epoch
    epoch = GetDataFromXmlNode(xmlContent, "EPOCH");
    int year = std::stoi(epoch.substr(0, 4));
    int month = std::stoi(epoch.substr(5, 2));
    int day = std::stoi(epoch.substr(8, 2));
    int hour = std::stoi(epoch.substr(11, 2));
    int minute = std::stoi(epoch.substr(14, 2));
    int second = std::stoi(epoch.substr(17, 2));
    double fraction = std::stod(epoch.substr(20));
    if (satrec.epochyr > 2000)
        satrec.epochyr -= 2000;
    else
        satrec.epochyr -= 1900;

    satrec.epochdays = GetEpochDayInYear(year, month, day, hour, minute, second, fraction);
    SGP4Funcs::jday_SGP4(year, month, day, hour, minute, second + fraction, satrec.jdsatepoch, satrec.jdsatepochF);

    // ---- find no, ndot, nddot ----
    satrec.no_kozai = satrec.no_kozai / minToRadian; //* rad/min

    // ---- convert to sgp4 units ----
    // satrec.a    = pow( satrec.no_kozai*tumin , (-2.0/3.0) );
    satrec.ndot = satrec.ndot / ndotUnits;  //* ? * minperday
    satrec.nddot = satrec.nddot / nddotUnits;

    // ---- find standard orbital elements ----
    satrec.inclo = satrec.inclo * deg2rad;
    satrec.nodeo = satrec.nodeo * deg2rad;
    satrec.argpo = satrec.argpo * deg2rad;
    satrec.mo = satrec.mo * deg2rad;

    SGP4Funcs::sgp4init(wgs72, 'i', satrec.satnum, (satrec.jdsatepoch + satrec.jdsatepochF) - 2433281.5, satrec.bstar, satrec.ndot, satrec.nddot, satrec.ecco,
        satrec.argpo, satrec.inclo, satrec.mo, satrec.no_kozai, satrec.nodeo, satrec);
}

/// <summary>
///  Wrapper method for the original SGP4 initializing orbiting element values from tle
/// </summary>
/// <param name="longstr1"></param>
/// <param name="longstr2"></param>
/// <param name="startmfe"></param>
/// <param name="stopmfe"></param>
/// <param name="deltamin"></param>
/// <param name="satrec"></param>
void DataGenerator::InitOrbitalElementsFromTLE(char longstr1[130], char longstr2[130], elsetrec& satrec)
{
    double startmfe, stopmfe, deltamin;
    char opsMode = 'i';
    char typeRun = 'c';
    char typeInput = 'm';
    SGP4Funcs::twoline2rv(longstr1, longstr2, typeRun, typeInput, opsMode, wgs72, startmfe, stopmfe, deltamin, satrec);
}

/// <summary>
///  This method gets the content of the XML as string, and name of tag, and returns the data from this tag.
/// </summary>
/// <param name="xmlContent"></param>
/// <param name="nodeName"></param>
/// <returns></returns>
std::string DataGenerator::GetDataFromXmlNode(std::string xmlContent, std::string nodeName)
{
    std::string nodeStartTag = "<" + nodeName + ">";
    std::string nodeEndTag = "</" + nodeName + ">";
    int dataStartPos = xmlContent.find(nodeStartTag) + nodeStartTag.length();
    int dataEndPos = xmlContent.find(nodeEndTag, dataStartPos);
    return xmlContent.substr(dataStartPos, dataEndPos);
}

/// <summary>
/// This method gets time and return the day in the year of that time
/// </summary>
/// <param name="year"></param>
/// <param name="month"></param>
/// <param name="day"></param>
/// <param name="hour"></param>
/// <param name="minute"></param>
/// <param name="second"></param>
/// <param name="fraction"></param>
/// <returns></returns>
double DataGenerator::GetEpochDayInYear(int year, int month, int day, int hour, int minute, int second, double fraction) {
    // Calculate the day of the year
    std::tm timeinfo = {};
    timeinfo.tm_year = year - 1900;  // years since 1900
    timeinfo.tm_mon = month - 1;      // months since January [0-11]
    timeinfo.tm_mday = day;           // day of the month [1-31]
    timeinfo.tm_hour = hour;          // hours since midnight [0-23]
    timeinfo.tm_min = minute;         // minutes after the hour [0-59]
    timeinfo.tm_sec = second;         // seconds after the minute [0-60]
    std::time_t time = std::mktime(&timeinfo);
    int dayOfYear = timeinfo.tm_yday + 1; // tm_yday is 0-365, so we add 1 to get 1-366

    // Calculate the fraction of the day
    double FractionOfDay = (hour * 3600 + minute * 60 + second + fraction) / (24 * 60 * 60);

    // Combine the day of the year and the fraction of the day
    double dayOfYearWithFraction = dayOfYear + FractionOfDay;
    return dayOfYearWithFraction;
}

/// <summary>
/// Method to save all the location and velocity vectors of the orbiting elements, and the values of F, dF, and also the distance between them
/// 
/// </summary>
/// <param name="length"></param>
/// <param name="name"></param>
/// <param name="elementsVectors"></param>
void DataGenerator::saveDataInCSVFile(int length, std::string& name, TcaCalculation::sPointData elementsVectors[]) {
    std::ofstream file("data/" + name + ".csv");
    file << "t,r1x,r1y,r1z,v1x,v1y,v1z,r2x,r2y,r2z,v2x,v2y,v2z\n";

    for (int timePoint = 0; timePoint < length; ++timePoint) {
        file << elementsVectors[timePoint].time << ","
            << elementsVectors[timePoint].r1x << "," << elementsVectors[timePoint].r1y << "," << elementsVectors[timePoint].r1z << ","
            << elementsVectors[timePoint].v1x << "," << elementsVectors[timePoint].v1y << "," << elementsVectors[timePoint].v1z << ","
            << elementsVectors[timePoint].r2x << "," << elementsVectors[timePoint].r2y << "," << elementsVectors[timePoint].r2z << ","
            << elementsVectors[timePoint].v2x << "," << elementsVectors[timePoint].v2y << "," << elementsVectors[timePoint].v2z << ","
            << "\n";
    }
}


void DataGenerator::GenerateTimePointForAncas(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[])
{
    m_ancasDataGenerator.GenerateTimePoint(n, tEnd, gamma, elementsVectors);
}

void DataGenerator::GenerateTimePointForCatch(int n, double tEnd, double gamma, TcaCalculation::sPointData elementsVectors[])
{
    m_catchDataGenerator.GenerateTimePoint(n, tEnd, gamma, elementsVectors);
}

double  DataGenerator::GetGamma(elsetrec elsetrec1, elsetrec elsetrec2, double factor) {
    // Compute time for half revolution in seconds for each satellite
    double t_sec1 = 60 * 2 / factor * 2 * PI / elsetrec1.no_kozai;
    double t_sec2 = 60 * 2 / factor * 2 * PI / elsetrec2.no_kozai;

    // Find the minimum time of the two half revolutions
    double Gamma = std::min(t_sec1, t_sec2);
    return Gamma;
}

void DataGenerator::GetStartTimeOfOrbElem(elsetrec elsetrec1, elsetrec elsetrec2, double& elemOneTime, double& ElemTwoTime)
{
    double time1 = elsetrec1.jdsatepoch + elsetrec1.jdsatepochF;
    double time2 = elsetrec2.jdsatepoch + elsetrec2.jdsatepochF;
    double minutesTimeDiff = (time1 - time2) * 1440;

    elemOneTime = std::max(0.0, -minutesTimeDiff);
    ElemTwoTime = std::max(0.0, minutesTimeDiff);
}