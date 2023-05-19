// Implementations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CATCH.h"
class TestFunction : public Function<double>
{
public:
    double getValue(double x)
    {
        return sin(x);
    }
};
class TestFunctiontag : public Function<double>
{
public:
    double getValue(double y)
    {
        return cos(y);
    }
};
class TestFunctionConst : public Function<double>
{
public:
    TestFunctionConst(double num)
    {
        mConst = num;
    }
    double getValue(double x)
    {
        return sin(x);
    }
private:
    double mConst;
};
int main()
{
    CATCH c;
    TestFunction t;
    TestFunctiontag ttag;
    TestFunctionConst tconst1(1), tconst0(0), tconst100(100);
    VectorFunction f1(&t,&tconst1,&tconst1), f2(&ttag, &tconst0, &tconst0);
    VectorFunction g1(&tconst100, &tconst100, &tconst100), g2(&tconst0, &tconst0, &tconst0);
    double gamma = 100, tmax = 100;
    TCA tca = c.CatchAlgorithm(&f1, &g1, &f2, &g2, gamma, tmax);
    std::cout << "result:\nTime: "<< tca.time <<"\nDistance:"<< tca.distance << "\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
