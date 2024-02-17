#include "TestUtils.h"
#include <math.h>

/// <summary>
/// Check if 2 values are equal with the given error
/// </summary>
/// <param name="a">first value</param>
/// <param name="b">second value</param>
/// <param name="maxError">the max error</param>
/// <returns>true if the 2 values are equal</returns>
bool TestUtils::CompareValues(double a, double b, double maxError)
{
    if (fabs(a - b) <= maxError)
    {
        return true;
    }
    return false;
}

bool TestUtils::CheckDoubleInArray(double* array, double value, int arraySize, double maxError)
{
    bool result = false;
    if (nullptr != array)
    {
        for (int i = 0; i < arraySize; i++)
        {
            if (CompareValues(array[i], value, maxError))
            {
                result = true;
                break;
            }
        }
    }
    return result;
}