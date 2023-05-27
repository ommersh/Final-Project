#pragma once
#include <math.h>
#include <Eigen/Eigenvalues>
#include <iostream>

using namespace Eigen;

template <class T>

class Function
{
public:
	virtual T getValue(int i) = 0;
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
	U getValue(int x)
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
	//VectorFunction(Vector3d* arr, int size)
	//{
		//VectorFunction::arr = arr;
		//VectorFunction::size = size;
	//};
	Vector3d getValue(int x) = 0;

private:
	//int size;
	//Vector3d *arr;
};

class RelativeDistanceFunction : public Function<Vector3d>
{
public:
	RelativeDistanceFunction(VectorFunction* g1, VectorFunction* g2)// : f(g1,g2)
	{
		RelativeDistanceFunction::g1 = g1;
		RelativeDistanceFunction::g2 = g2;

	}
	Vector3d getValue(int x)
	{
		//std::cout << "x = " << x << " g1 = " << g1->getValue(x) << " g2 = " << g2->getValue(x) << " \n";
		return g1->getValue(x) - g2->getValue(x);
	}
private:
	//RelativeFunction<Vector3d> f;
	VectorFunction* g1; VectorFunction* g2;
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
	double getValue(int x)
	{
		//std::cout << "x = " << x << "index = " << index << " \n" << f1->getValue(x)(index) - f2->getValue(x)(index) << " \n";
		return f1->getValue(x)(index) - f2->getValue(x)(index);
	};
private:
	VectorFunction* f1;
	VectorFunction* f2;
	int index;
};

class Fd : public Function<double>
{
public:
	Fd(RelativeDistanceFunction* Rd, RelativeDistanceFunction* Rd_dot)
	{
		Fd::Rd = Rd;
		Fd::Rd_dot = Rd_dot;
	}
	double getValue(int x)
	{
		//std::cout << x << " rd " << Rd->getValue(x) << " rddot " << Rd_dot->getValue(x) << "\n";
		//std::cout << "\n" << 2 * (Rd->getValue(x).dot(Rd_dot->getValue(x))) << "\n";
		return 2 * (Rd->getValue(x).dot(Rd_dot->getValue(x)));
	}
private:
	RelativeDistanceFunction* Rd;
	RelativeDistanceFunction* Rd_dot;
};