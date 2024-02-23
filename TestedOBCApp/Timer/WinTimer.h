#ifndef SHIELD_WinTimer_H    // Check if the symbol SHIELD_WinTimer_H is not defined
#define SHIELD_WinTimer_H    // Define the symbol SHIELD_WinTimer_H


#include "ITimer.h"


class WinTimer : public ITimer
{
public:
	WinTimer();
	void startTimer() override;
	void stopTimer() override;
	double getTimeInMicroSec() override;
	double getTimeInMilliSec() override;
	double getTimeInSec() override;


private:
	double m_startTime;
	double m_endTime;
	double m_timeInMicro;
};

#endif //SHIELD_WinTimer_H