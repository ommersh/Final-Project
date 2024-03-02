#ifndef SHIELD_AppConfigurationManager_H    // Check if the symbol SHIELD_AppConfigurationManager_H is not defined
#define SHIELD_AppConfigurationManager_H    // Define the symbol SHIELD_AppConfigurationManager_H


namespace AppConfiguration
{
	enum TimerType
	{
		ChronoTimer
	};
}



/// <summary>
/// Class to manage app configureations(like type of timer)
/// </summary>
class AppConfigurationManager
{
public:
	static const AppConfiguration::TimerType timerType = AppConfiguration::TimerType::ChronoTimer;

};









#endif //SHIELD_AppConfigurationManager_H