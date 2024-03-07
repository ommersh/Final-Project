#ifndef SHIELD_WinTimer_H    // Check if the symbol SHIELD_WinTimer_H is not defined
#define SHIELD_WinTimer_H    // Define the symbol SHIELD_WinTimer_H


#include "ITimer.h"


class ChronoBasedTimer : public ITimer
{
public:
	ChronoBasedTimer();
	void startTimer() override;
	void stopTimer() override;
	double getTimeInMicroSec() override;
	double getTimeInMilliSec() override;
	double getTimeInSec() override;


private:
	long int m_startTime;
	long int m_endTime;
	double m_timeInMicro;
};

#endif //SHIELD_WinTimer_H