#include "timeofday.h"

#include "constants.h"

TimeOfDay::TimeOfDay() :
	GameObject(DRAW_ORDER_TIME_OF_DAY),
	time(60*10)
{
}