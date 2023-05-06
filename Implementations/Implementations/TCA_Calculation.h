#pragma once
namespace TCA_Calculation
{
	struct TCA {
		long double time;
		long double distance;
	};
	class definedFunctionInInterval
	{
	public:
		definedFunctionInInterval(double start, double end)
		{
			Start = start;
			End = end;
		};
		double getValue(double x)
		{
			return 0;
		};
		void setValues(double x_values[], double y_values[], int size)
		{

		}
	private:
		double Start, End;

	};

};