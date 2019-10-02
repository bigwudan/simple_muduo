#include "timer.h"

int64_t Timer::s_numCreated_;

void Timer::restart(int now)
{
    expiration_ = now;
}
