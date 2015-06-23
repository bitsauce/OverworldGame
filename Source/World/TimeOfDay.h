#ifndef TIME_OF_DAY_H
#define TIME_OF_DAY_H

#include "Config.h"

class TimeOfDay
{
public:
	TimeOfDay();

	void setTime(const float time)
	{
		this->time = time;
	}

	float getTime() const
	{
		return time;
	}
	
	int getHour() const
	{
		return int(time/60.0f);
	}
	
	int getMinute() const
	{
		return int(time-getHour()*60.0f);
	}
	
	bool isDay() const
	{
		int hour = getHour();
		return hour >= 6 && hour < 18;
	}
	
	bool isNight() const
	{
		return !isDay();
	}

	void update(const float delta)
	{
		// Apply time
		time += delta;

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
	}

private:
	float time;
};

#endif // TIME_OF_DAY_H