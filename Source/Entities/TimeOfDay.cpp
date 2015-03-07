#include "timeofday.h"

#include "constants.h"

TimeOfDay::TimeOfDay() :
	GameObject(PRIORITY_TIME_OF_DAY),
	time(60*10)
{
}