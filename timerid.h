#ifndef _TIMERID_H
#define _TIMERID_H

#include <cstdint>

class Timer;


class TimerId
{
 public:
  TimerId()
    : timer_(NULL),
      sequence_(0)
  {
  }

  TimerId(Timer* timer, int64_t seq)
    : timer_(timer),
      sequence_(seq)
  {
  }

  // default copy-ctor, dtor and assignment are okay

  friend class TimerQueue;

 private:
  Timer* timer_;
  int64_t sequence_;
};



#endif  // MUDUO_NET_TIMERID_H
