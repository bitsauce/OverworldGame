#include "timeofday.h"

#include "constants.h"

TimeOfDay::TimeOfDay(World *world) :
	Entity(world, ENTITY_TIME_OF_DAY),
	time(60*10)
{
}