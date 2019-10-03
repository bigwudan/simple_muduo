#ifndef _TIMERQUEUE_H
#define _TIMERQUEUE_H

#include <iostream>
#include <cstdint>
#include <set>

class Timer;
class EventLoop;
class Channel;

class TimerQueue
{
    public:
        explicit TimerQueue(EventLoop* loop);
        ~TimerQueue();

        TimerId addTimer(TimerCallback cb, int when, double interval);

        void cancel(TimerId timerId);
    private:

        typedef std::pair<int, Timer*>  Entry;
        typedef std::set<Entry> TimerList;
        typedef std::pair<Timer*, int64_t> ActiveTimer;
        typedef std::set<ActiveTimer> ActiveTimerSet;

        void addTimerInLoop(Timer* timer);
        void cancelInLoop(TimerId timerId);
        void handleRead();

        std::vector<Entry> getExpired(int now);
        bool insert(Timer* timer);

        EventLoop* loop_;
        const int timerfd_;
        Channel timerfdChannel_;
        TimerList timers_;

        ActiveTimerSet activeTimers_;
        bool callingExpiredTimers_;
        ActiveTimerSet cancelingTimers_;


};


#endif

