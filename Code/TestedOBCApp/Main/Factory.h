#ifndef SHIELD_Factory_H    // Check if the symbol SHIELD_Factory_H is not defined
#define SHIELD_Factory_H    // Define the symbol SHIELD_Factory_H

#include "TcpCommChannel.h"
#ifdef WIN32
#include "WinUdpCommChannel.h"
#endif // WIN32

#include "ITimer.h"
#include "ITcaAlgorithm.h"
#include "IRootsFindAlg.h"
#include "ISinglePointPropogator.h"

#include "ChronoBasedTimer.h"
#include "CATCH.h"
#include "ANCAS.h"
#include "SboAncas.h"
#include "SboAncasEquallySpacedPoints.h"

#include "CompanionMatrixRootsFinderEigen.h"
//#include "CompanionMatrixRootsFinderArmadillo.h"
#include "SGP4SinglePointGenerator.h"
#include "ResultsLogger.h"

#include "CommonStructures.h"
#include "AlgorithmEnum.h"
#include "AppConfigurationManager.h"
#include "TestManager.h"

#include "ICommChannel.h"

#include "TestedOBCLocalSimulation.h"



/// <summary>
/// TestedOBCApp Factory.
/// Create and manage app opbjects.
/// Singleton.
/// </summary>
class Factory
{
public:
	/// <summary>
	/// Get a reference to the single instance of the factory
	/// </summary>
	/// <returns></returns>
	static Factory* GetReference();
	~Factory();
	/// <summary>
	/// Get the timer, for timing tests.
	/// </summary>
	/// <returns></returns>
	ITimer*			GetTimer();
	/// <summary>
	/// Get appropriate IRootsFindAlg object based on the requested type;
	/// </summary>
	/// <param name="algType"></param>
	/// <param name="degree"></param>
	/// <returns></returns>
	IRootsFindAlg*	GetRootsFindAlg(AlgorithmsEnums::CatchRootsAlg algType, int degree);
	/// <summary>
	/// Get Catch object initialize to the requested degree and root finding algorithm.
	/// </summary>
	/// <param name="algType"></param>
	/// <param name="degree"></param>
	/// <returns></returns>
	CATCH*			GetCATCH(AlgorithmsEnums::CatchRootsAlg algType, int degree);
	/// <summary>
	/// Get Ancas object, should be initialized.
	/// </summary>
	/// <returns></returns>
	ANCAS*			GetANCAS();

	SboAncas*		GetSboAncas(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2, double TOLd, double TOLt);

	ISinglePointPropogator* GetSinglePointPropogator(elsetrec elsetrec1, elsetrec elsetrec2, double startTime1, double startTime2);
	ResultsLogger* GetResultsLogger();
	AppConfigurationManager* GetConfigurationManager();

	ICommChannel* GetCommChannel();
private:
	static Factory* m_reference;
	Factory();
	ITimer* m_timer;
	ICommChannel* m_commChannel;
	CATCH m_catch;
	ANCAS m_ancas;
	SboAncas m_sboAncas;
	CompanionMatrixRootsFinderEigen m_companionMatrixRootsFinderEigen;
	//CompanionMatrixRootsFinderArmadillo m_companionMatrixRootsFinderAArmadillo;
	SGP4SinglePointGenerator m_SGP4SinglePointGenerator;
	ResultsLogger m_resultsLogger;

	AppConfigurationManager m_configManager;
};









#endif //SHIELD_Factory_H