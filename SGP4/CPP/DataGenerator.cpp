#include <string>
#include "SGP4.h"

#include <fstream>
#include <vector>
#include "DataGenerator.h"

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
void DataGenerator::GenearateDiffVectorFor2OrbitalElementsCSV(double* timeInMinutes, int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, std::string& fileName)
{
    double* f = new double[timePointsArrLength];
    double* df = new double[timePointsArrLength];
    double** r1Arr = new double* [timePointsArrLength];
    double** r2Arr = new double* [timePointsArrLength];
    double** v1Arr = new double* [timePointsArrLength];
    double** v2Arr = new double* [timePointsArrLength];

    for (int i = 0; i < timePointsArrLength; ++i) {
        r1Arr[i] = new double[3];
        r2Arr[i] = new double[3];
        v1Arr[i] = new double[3];
        v2Arr[i] = new double[3];
    }

    CalculateRelativeVectorsForTwoElements(timeInMinutes, timePointsArrLength, elsetrec1, elsetrec2, r1Arr, r2Arr, v1Arr, v1Arr, f, df);
    saveDataInCSVFile(timeInMinutes, r1Arr, v1Arr, r2Arr, v2Arr, f, df, timePointsArrLength, fileName);

    delete f, df, r1Arr, r2Arr, v1Arr, v1Arr;
}

void DataGenerator::CalculateRelativeVectorsForTwoElements(double* timeInMinutes, int timePointsArrLength, elsetrec elsetrec1, elsetrec elsetrec2, double** r1Arr, double** r2Arr,
    double** v1Arr, double** v2Arr, double* f, double* df) 
{
    double time1 = elsetrec1.jdsatepoch + elsetrec1.jdsatepochF;
    double time2 = elsetrec2.jdsatepoch + elsetrec2.jdsatepochF;
    double minutesTimeDiff = (time1 - time2) * 1440;

    double startDataElem1 = std::max(0.0, -minutesTimeDiff);
    double startDataElem2 = std::max(0.0, minutesTimeDiff);

    double r1[3], v1[3];
    double r2[3], v2[3];

    for (int i = 0; i < timePointsArrLength; ++i) {
        // Compute position and velocity
        SGP4Funcs::sgp4(elsetrec1, startDataElem1 + timeInMinutes[i], r1, v1);
        SGP4Funcs::sgp4(elsetrec2, startDataElem2 + timeInMinutes[i], r2, v2);

        r1Arr[i] = r1;
        r2Arr[i] = r2;
        v1Arr[i] = v1;
        v2Arr[i] = v2;

        std::cout << "Position Diff (km): " << r1[0] - r2[0] << ", " << r1[1] - r2[1] << ", " << r1[2] - r2[2] << std::endl;
        f[i] = pow(r2[0] - r1[0], 2) + pow(r2[1] - r1[1], 2) + pow(r2[2] - r1[2], 2);
        df[i] = (r2[0] - r1[0]) * (v2[0] - v1[0]) + (r2[1] - r1[1]) * (v2[1] - v1[1]) + (r2[2] - r1[2]) * (v2[2] - v1[2]);
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
    const double minToRadian = 720.0 / PI; //todo: write hardships
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
    strncpy(satrec.intldesg, objectID.c_str(), 10);
    satrec.intldesg[10] = '\0';

    // NORAD Catalog ID
    noradCatID = GetDataFromXmlNode(xmlContent, "NORAD_CAT_ID");
    strncpy(satrec.satnum, noradCatID.c_str(), 5);
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
    tm timeinfo = {};
    timeinfo.tm_year = year - 1900;  // years since 1900
    timeinfo.tm_mon = month - 1;      // months since January [0-11]
    timeinfo.tm_mday = day;           // day of the month [1-31]
    timeinfo.tm_hour = hour;          // hours since midnight [0-23]
    timeinfo.tm_min = minute;         // minutes after the hour [0-59]
    timeinfo.tm_sec = second;         // seconds after the minute [0-60]
    time_t time = mktime(&timeinfo);
    int dayOfYear = timeinfo.tm_yday + 1; // tm_yday is 0-365, so we add 1 to get 1-366

    // Calculate the fraction of the day
    double FractionOfDay = (hour * 3600 + minute * 60 + second + fraction) / (24 * 60 * 60);

    // Combine the day of the year and the fraction of the day
    double dayOfYearWithFraction = dayOfYear + FractionOfDay;
    return dayOfYearWithFraction;
}

/// <summary>
/// Method to save all the location and velocity vectors of the orbiting elements, and the values of F, dF, and also the distance between them
/// </summary>
/// <param name="time_points"></param>
/// <param name="r1"></param>
/// <param name="v1"></param>
/// <param name="r2"></param>
/// <param name="v2"></param>
/// <param name="f"></param>
/// <param name="df"></param>
/// <param name="length"></param>
/// <param name="name"></param>
void DataGenerator::saveDataInCSVFile(double* time_points,
    double** r1,
    double** v1,
    double** r2,
    double** v2,
    double* f,
    double* df,
    int length,
    std::string& name) {
    std::ofstream file("data/" + name + ".csv");
    file << "t,r1x,r1y,r1z,v1x,v1y,v1z,r2x,r2y,r2z,v2x,v2y,v2z,f,df,distance\n";

    for (int row = 0; row < length; ++row) {
        double distance = std::sqrt(f[row]);
        file << time_points[row] << ","
            << r1[row][0] << "," << r1[row][1] << "," << r1[row][2] << ","
            << v1[row][0] << "," << v1[row][1] << "," << v1[row][2] << ","
            << r2[row][0] << "," << r2[row][1] << "," << r2[row][2] << ","
            << v2[row][0] << "," << v2[row][1] << "," << v2[row][2] << ","
            << f[row] << "," << df[row] << ","
            << distance << "\n";
    }
}


double* DataGenerator::GenerateTimePointForAncas(int n, double tEnd, double gamma)
{
    return m_ancasDataGenerator.GenerateTimePoint(n, tEnd, gamma);
}

double* DataGenerator::GenerateTimePointForCatch(int n, double tEnd, double gamma)
{
    return m_catchDataGenerator.GenerateTimePoint(n, tEnd, gamma);
}