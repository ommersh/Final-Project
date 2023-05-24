#pragma once
#include <math.h>
#include <Eigen/Eigenvalues>

using namespace Eigen;

template <class T>

class Function
{
public:
	virtual T getValue(double i) = 0;
};
template <class U>
class RelativeFunction : public Function<U>
{
public:
	RelativeFunction(Function<U>* g1, Function<U>* g2)
	{
		f1 = g1;
		f2 = g2;
	}
	U getValue(double x)
	{
		return f1->getValue(x) - f2->getValue(x);
	};
private:
	Function<U>* f1;
	Function<U>* f2;
};
class VectorFunction : public Function<Vector3d>
{
public:
	Vector3d getValue(int x)
	{
		Vector3d v(3);
		v(0) = 0;
		v(1) = 0;
		v(2) = 0;
		return v;
	};
};

class RelativeDistanceFunction : public Function<double>
{
public:
	RelativeDistanceFunction(VectorFunction* g1, VectorFunction* g2) : f(g1,g2)
	{

	}
	double getValue(double x)
	{
		return f.getValue(x).dot(f.getValue(x));
	}
private:
	RelativeFunction<Vector3d> f;
};
class RelativeFunctionInIndex : public Function<double>
{
public:
	RelativeFunctionInIndex(VectorFunction* g1, VectorFunction* g2, int i)
	{
		f1 = g1;
		f2 = g2;
		index = i;
	};
	double getValue(double x)
	{
		return f1->getValue(x)(index) - f2->getValue(x)(index);
	};
private:
	VectorFunction* f1;
	VectorFunction* f2;
	int index;
};