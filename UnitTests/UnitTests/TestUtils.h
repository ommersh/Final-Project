#ifndef SHIELD_TestUtils_H    // Check if the symbol SHIELD_TestUtils_H is not defined
#define SHIELD_TestUtils_H    // Define the symbol SHIELD_TestUtils_H



class TestUtils
{
public:
    static bool CompareValues(double a, double b, double maxError);
    static bool CheckDoubleInArray(double* array, double value, int arraySize, double maxError);
};

#endif //SHIELD_TestUtils_H



