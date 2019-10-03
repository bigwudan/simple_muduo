#include "eventloop.h"
#include "timer.h"
#include "timerid.h"

#include "timerqueue.h"
#include <sys/timerfd.h>
#include <unistd.h>


int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
            TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        std::cout << "fail timerfd" << std::endl;
        return -1;
    }
    return timerfd;
}

struct timespec howMuchTimeFromNow(int when)
{
  struct timespec ts;
  ts.tv_sec = when;
  ts.tv_nsec = 0;
  return ts;
}

void readTimerfd(int timerfd, int now)
{
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  if (n != sizeof howmany)
  {
  }
}

void resetTimerfd(int timerfd, int expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  memset(&newValue, 0, sizeof newValue);
  memset(&oldValue, 0, sizeof oldValue);
  newValue.it_value = howMuchTimeFromNow(expiration);
  


  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret == -1)
  {
      std::cout << "set time err" << std::endl;
  }
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(int now)
{
	std::vector<Entry> expired;
	Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
	TimerList::iterator end = timers_.lower_bound(sentry);
	std::copy(timers_.begin(), end, back_inserter(expired));
	timers_.erase(timers_.begin(), end);

	for (const Entry& it : expired)
	{
		ActiveTimer timer(it.second, it.second->sequence());
		size_t n = activeTimers_.erase(timer);
		assert(n == 1); (void)n;
	}

	return expired;
}


void TimerQueue::reset(const std::vector<Entry>& expired, int now)
{
  int nextExpire;

  for (const Entry& it : expired)
  {
    ActiveTimer timer(it.second, it.second->sequence());
    if (it.second->repeat()
        && cancelingTimers_.find(timer) == cancelingTimers_.end())
    {
      it.second->restart(now);
      insert(it.second);
    }
    else
    {
      // FIXME move to a free list
      delete it.second; // FIXME: no delete please
    }
  }

  if (!timers_.empty())
  {
    nextExpire = timers_.begin()->second->expiration();
  }

  resetTimerfd(timerfd_, nextExpire);
}


void TimerQueue::handleRead()
{
	int now = time(NULL);
	readTimerfd(timerfd_, now);

	std::vector<Entry> expired = getExpired(now);

	callingExpiredTimers_ = true;
	cancelingTimers_.clear();
	// safe to callback outside critical section
	for (const Entry& it : expired)
	{
		it.second->run();
	}
	callingExpiredTimers_ = false;

	reset(expired, now);
}





TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop, timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
	timerfdChannel_.setReadCallback(
			std::bind(&TimerQueue::handleRead, this));
	timerfdChannel_.enableReading();
}


TimerQueue::~TimerQueue(){};

bool TimerQueue::insert(Timer* timer)
{
	bool earliestChanged = false;
	int when = timer->expiration();
	TimerList::iterator it = timers_.begin();
	if (it == timers_.end() || when < it->first)
	{
		earliestChanged = true;
	}
	{
		timers_.insert(Entry(when, timer));
	}
	{
		activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
	}


	return earliestChanged;
}




TimerId TimerQueue::addTimer(TimerCallback cb,
        int when,
        double interval)
{
    Timer* timer = new Timer(std::move(cb), when, interval);
    addTimerInLoop(timer);
    return TimerId(timer, timer->sequence());
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
    bool earliestChanged = insert(timer);
    if (earliestChanged)
    {
        resetTimerfd(timerfd_, timer->expiration());
    }
}








