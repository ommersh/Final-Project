#ifndef SHIELD_UTSboAncasTests_H    // Check if the symbol SHIELD_UTSboAncasTests_H is not defined
#define SHIELD_UTSboAncasTests_H    // Define the symbol SHIELD_UTSboAncasTests_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SboAncas.h"
#include "SimpleDataGeneration.h"
#include "SGP4SinglePointGenerator.h"




class SboAncasTestCase : public ::testing::Test {
protected:
    SboAncas sboAncas;
    elsetrec elsetrec1;
    elsetrec elsetrec2;
    double startTime1Min;
    double startTime2Min;
    SimpleDataGeneration m_SimpleDataGeneration;
    SGP4SinglePointGenerator m_SGP4SinglePointGenerator;
    TcaCalculation::sPointData* data;
    void getSboAncasData();
    void calculateWithSmallTimestep(double timePoint);

    TCA resultsTca;

    void SetUp() override {
        // Setup code
        data = nullptr;
    }

    void TearDown() override {
        // Teardown code
        if (nullptr != data)
        {
            delete[] data;
        }
    }
};



class WorstCasePropogator : public ISinglePointPropogator
{
public:

    TcaCalculation::sPointData getSinglePoint(double timePointSec)override
    {
        TcaCalculation::sPointData pointData = { 0 };
        pointData.time = timePointSec;
        return pointData;
    }
};

class WorstCaseSboAncas : public SboAncas
{
public:
    double middlePoint;
    double epsilon;
    unsigned long long iterations;
protected:

    TCA ANCASIteration() override
    {
        TCA tca = { 0 };
        if (iterations % 2 == 0)
        {
            tca.time = m_dataPoints[0].time + epsilon;

        }
        else
        {
            tca.time = m_dataPoints[3].time - epsilon;
        }
        iterations++;
        return tca;
    }

    
};

class SboAncasWorstCaseTest : public ::testing::Test {
protected:
    
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code

    }
};

#endif //SHIELD_ANCASTests_H