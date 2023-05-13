#pragma once
#include <math.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::ublas;

template <class T>

class Function
{
public:
	virtual T getValue(double x) = 0;
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
class VectorFunction : public Function<vector<double>>
{
public:
	VectorFunction(Function<double> *x, Function<double> *y, Function<double> *z)
	{
		X = x; Y = y;Z = z;
	}
	vector<double> getValue(double x)
	{
		vector<double> v(3);
		v(0) = X->getValue(x);
		v(1) = Y->getValue(x);
		v(2) = Z->getValue(x);
		return v;
	};
	Function<double>* X;
	Function<double>* Y;
	Function<double>* Z;
};

class RelativeDistanceFunction : public Function<double>
{
public:
	RelativeDistanceFunction(VectorFunction* g1, VectorFunction* g2) : f(g1,g2)
	{

	}
	double getValue(double x)
	{
		return norm_2(f.getValue(x));
	}
private:
	RelativeFunction<vector<double>> f;
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