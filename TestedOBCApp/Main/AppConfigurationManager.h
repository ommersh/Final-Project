#ifndef SHIELD_AppConfigurationManager_H    // Check if the symbol SHIELD_AppConfigurationManager_H is not defined
#define SHIELD_AppConfigurationManager_H    // Define the symbol SHIELD_AppConfigurationManager_H


namespace AppConfiguration
{
	enum TimerType
	{
		WinChrono
	};
}




class AppConfigurationManager
{
public:
	static const AppConfiguration::TimerType timerType = AppConfiguration::TimerType::WinChrono;

};









#endif //SHIELD_AppConfigurationManager_H