#ifndef SHIELD_ITimer_H    // Check if the symbol SHIELD_ITimer_H is not defined
#define SHIELD_ITimer_H    // Define the symbol SHIELD_ITimer_H

class ITimer
{
public:
	virtual ~ITimer()
	{

	};
	virtual void startTimer() = 0;
	virtual void stopTimer() = 0;
	virtual double getTimeInMicroSec() = 0;
	virtual double getTimeInMilliSec() = 0;
	virtual double getTimeInSec() = 0;

};



#endif //SHIELD_ITimer_H