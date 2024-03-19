#ifndef SHIELD_SimpleDataGeneration_H    // Check if the symbol SHIELD_SimpleDataGeneration_H is not defined
#define SHIELD_SimpleDataGeneration_H    // Define the symbol SHIELD_SimpleDataGeneration_H

#include "TcaCalculation.h"
#include "SGP4.h"
#include "DataGenerator.h"

class SimpleDataGeneration
{
private:
    DataGenerator m_dataGenerator;
public:
    TcaCalculation::sPointData* m_pointsDataANCAS;
    TcaCalculation::sPointData* m_pointsDataCATCH;
    int m_numberOfPoints;

    const double PI = 3.14159265358979323846;

    void GenearateDataFromTle(char* longstr1Obj1, char* longstr2bj1, char* longstr1Obj2, char* longstr2bj2, int numberOfDays, int numberOfPointsInSegment, elsetrec &elsetrec1, elsetrec &elsetrec2, double &startDataElem1, double &startDataElem2)
    {
        double startmfe, stopmfe, deltamin;
        char opsMode = 'i';
        char typeRun = 'c';
        char typeInput = 'm';
        char meanMotionString1[10];
        char meanMotionString2[10];

        memcpy(meanMotionString1, longstr2bj1 + 52, 10);
        memcpy(meanMotionString2, longstr2bj2 + 52, 10);

        // Convert the strings to double values
       

        SGP4Funcs::twoline2rv(longstr1Obj1, longstr2bj1, typeRun, typeInput, opsMode, wgs72, startmfe, stopmfe, deltamin, elsetrec1);
        SGP4Funcs::twoline2rv(longstr1Obj2, longstr2bj2, typeRun, typeInput, opsMode, wgs72, startmfe, stopmfe, deltamin, elsetrec2);

        double time1 = elsetrec1.jdsatepoch + elsetrec1.jdsatepochF;
        double time2 = elsetrec2.jdsatepoch + elsetrec2.jdsatepochF;
        double minutesTimeDiff = (time1 - time2) * 1440;

        startDataElem1 = std::max(0.0, -minutesTimeDiff);
        startDataElem2 = std::max(0.0, minutesTimeDiff);

        //mean motion is the number of revolutions per day, 1 / mean motion is the time in days for each revolution
        //we change the time units to seconds and time by 1 / 2 to get the time of half revolution in seconds
        const double seconds_in_day = 86400.0;
        double testTime = seconds_in_day * numberOfDays;

        // Compute time for half revolution in seconds for each satellite
        double t_sec1 = 60 * 0.5 * 2 * PI / elsetrec1.no_kozai;
        double t_sec2 = 60 * 0.5 * 2 * PI / elsetrec2.no_kozai;

        // Find the minimum time of the two half revolutions
        double Gamma = std::min(t_sec1, t_sec2);

        //Generate timePoints
        m_numberOfPoints = numberOfPointsInSegment * testTime / Gamma;

        m_pointsDataANCAS = new TcaCalculation::sPointData[m_numberOfPoints];
        m_pointsDataCATCH = new TcaCalculation::sPointData[m_numberOfPoints];
        GenerateTimePointsForANCAS(numberOfPointsInSegment, testTime, Gamma, m_numberOfPoints);
        GenerateTimePointsForCatch(numberOfPointsInSegment, testTime, Gamma, m_numberOfPoints);

        //generate test data
        double r1[3], v1[3];
        double r2[3], v2[3];

        //for ancas
        for (int i = 0; i < m_numberOfPoints; i++) {
            // Compute position and velocity
            double timeInMinutes = m_pointsDataANCAS[i].time / 60;
            SGP4Funcs::sgp4(elsetrec1, startDataElem1 + timeInMinutes, r1, v1);
            SGP4Funcs::sgp4(elsetrec2, startDataElem2 + timeInMinutes, r2, v2);

            m_pointsDataANCAS[i].r1x = r1[0];
            m_pointsDataANCAS[i].r1y = r1[1];
            m_pointsDataANCAS[i].r1z = r1[2];

            m_pointsDataANCAS[i].r2x = r2[0];
            m_pointsDataANCAS[i].r2y = r2[1];
            m_pointsDataANCAS[i].r2z = r2[2];

            m_pointsDataANCAS[i].v1x = v1[0];
            m_pointsDataANCAS[i].v1y = v1[1];
            m_pointsDataANCAS[i].v1z = v1[2];

            m_pointsDataANCAS[i].v2x = v2[0];
            m_pointsDataANCAS[i].v2y = v2[1];
            m_pointsDataANCAS[i].v2z = v2[2];
            /*std::cout << m_pointsDataANCAS[i].time << "," <<
                m_pointsDataANCAS[i].r1x << "," <<
                m_pointsDataANCAS[i].r1y << "," <<
                m_pointsDataANCAS[i].r1z << "," <<

                m_pointsDataANCAS[i].v1x << "," <<
                m_pointsDataANCAS[i].v1y << "," <<
                m_pointsDataANCAS[i].v1z << "," <<

                m_pointsDataANCAS[i].r2x << "," <<
                m_pointsDataANCAS[i].r2y << "," <<
                m_pointsDataANCAS[i].r2z << "," <<

                m_pointsDataANCAS[i].v2x << "," <<
                m_pointsDataANCAS[i].v2y << "," <<
                m_pointsDataANCAS[i].v2z << "," << std::endl;
                */

        }

        //for catch
        for (int i = 0; i < m_numberOfPoints; i++) {
            // Compute position and velocity
            SGP4Funcs::sgp4(elsetrec1, startDataElem1 + m_pointsDataCATCH[i].time / 60, r1, v1);
            SGP4Funcs::sgp4(elsetrec2, startDataElem2 + m_pointsDataCATCH[i].time / 60, r2, v2);

            m_pointsDataCATCH[i].r1x = r1[0];
            m_pointsDataCATCH[i].r1y = r1[1];
            m_pointsDataCATCH[i].r1z = r1[2];

            m_pointsDataCATCH[i].r2x = r2[0];
            m_pointsDataCATCH[i].r2y = r2[1];
            m_pointsDataCATCH[i].r2z = r2[2];

            m_pointsDataCATCH[i].v1x = v1[0];
            m_pointsDataCATCH[i].v1y = v1[1];
            m_pointsDataCATCH[i].v1z = v1[2];

            m_pointsDataCATCH[i].v2x = v2[0];
            m_pointsDataCATCH[i].v2y = v2[1];
            m_pointsDataCATCH[i].v2z = v2[2];
        }
    }

    void GenerateTimePointsForANCAS(int numberOfPointsInSegment, double tEnd, double gamma, int numberOfPoints)
    {
        double Ancas_jump = gamma / (numberOfPointsInSegment - 1);
        double a = 0;
        double b = gamma;
        int index = 1;
        m_pointsDataANCAS[0].time = 0;

        while (b < tEnd) {
            for (int i = 1; i < numberOfPointsInSegment; i++) {
                if (index < numberOfPoints)
                {
                    m_pointsDataANCAS[index++].time = a + i * Ancas_jump;
                }
            }
            a = b;
            b = b + gamma;
        }
    }


    void GenerateTimePointsForCatch(int numberOfPointsInSegment, double tEnd, double gamma, int numberOfPoints)
    {
        double* catchPoints = calculateXforN(numberOfPointsInSegment, gamma);
        double a = 0;
        double b = gamma;
        int index = 1;
        m_pointsDataCATCH[0].time = 0;

        while (b < tEnd) {
            for (int i = 1; i < numberOfPointsInSegment; i++) {
                if (index < numberOfPoints)
                {
                    m_pointsDataCATCH[index++].time = a + catchPoints[i];
                }
            }
            a = b;
            b = b + gamma;
        }

        delete[] catchPoints;
    }

    double* calculateXforN(int n, double Gamma)
    {
        double* times = new double[n];
        for (int i = 0; i < n; ++i) {
            int j = n - 1 - i;
            times[i] = calculateXj(0, Gamma, j, n);
        }
        return times;
    }

    double calculateXj(double a, double b, int j, int n)
    {
        double x = ((b - a) / 2) * cos(PI * j / (n - 1)) + (b + a) / 2;
        return x;
    }
};



#endif //SHIELD_SimpleDataGeneration_H