#ifndef _TIMER_H
#define _TIMER_H


#include <cstdint>
#include <functional>

typedef std::function<void()> TimerCallback;
class Timer{

public:
    Timer(TimerCallback cb, int when, double interval)
        : callback_(std::move(cb)),
        expiration_(when),
        interval_(interval),
        repeat_(interval > 0.0),
        sequence_(0)
    {}

    void run() const
    {
        callback_();
    }

    int expiration() const { return expiration_;}   

    bool repeat() const {return repeat_;}

    int64_t sequence() const {return sequence_;}

    void restart(int now);
    static int64_t numCreated(){return s_numCreated_;}

private:
    const TimerCallback callback_;
    int expiration_;
    const double interval_;
    const bool repeat_;
    const int64_t sequence_;
    static int64_t s_numCreated_;


};

#endif

