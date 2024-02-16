#ifndef SHIELD_IRootsFindAlg_H    // Check if the symbol SHIELD_IRootsFindAlg_H is not defined
#define SHIELD_IRootsFindAlg_H    // Define the symbol SHIELD_IRootsFindAlg_H


class IRootsFindAlg
{
public:
	/// <summary>
	/// find the roots for a given polynomial
	/// </summary>
	/// <param name="coefficients">the polynomial coefficients</param>
	/// <param name="degree">the polynomial degree</param>
	/// <param name="roots">array for the results, with size = degree</param>
	/// <returns>the number of real roots found</returns>
	virtual int findRoots(double* coefficients, int degree, double* roots) = 0;


};
#endif //SHIELD_IRootsFindAlg_H