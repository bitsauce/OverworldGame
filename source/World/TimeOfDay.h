#ifndef TIME_OF_DAY_H
#define TIME_OF_DAY_H

#include "Config.h"

class TimeOfDay
{
public:
	TimeOfDay();

	float getTime()
	{
		return time;
	}
	
	int getHour()
	{
		return int(time/60.0f);
	}
	
	int getMinute()
	{
		return int(time-getHour()*60.0f);
	}
	
	bool isDay()
	{
		int hour = getHour();
		return hour >= 6 && hour < 18;
	}
	
	bool isNight()
	{
		return !isDay();
	}

	void update(const float delta)
	{
		// Apply time
		time += delta;

		/*// Debug: Time speedup (0 forwards, 9 backwards)
		if(xd::Input::getKeyState(xd::XD_KEY_0))
		{
			time += 10.0f;
		}
		else if(xd::Input::getKeyState(xd::XD_KEY_9))
		{
			time -= 10.0f;
		}*/

		// Make sure time loops around to 00:00
		if(time >= 1440.0f)
		{
			time = 0.0f;
		}

		// Make sure time loops back to 24:59
		if(time < 0.0f)
		{
			time = 1440.0f;
		}
		
		// Draw time
		//World::getDebug()->setVariable("Time", formatInt(getHour(), "0", 2) + ":" + formatInt(getMinute(), "0", 2));
	}

private:
	float time;
};

#endif // TIME_OF_DAY_H