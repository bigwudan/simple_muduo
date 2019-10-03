#include <iostream>
#include <unistd.h>
#include "eventloop.h"
#include "thread.h"
#include "channel.h"
#include "poller.h"
#include "timer.h"
#include "timerid.h"
#include "timerqueue.h"

#include <cstring>
#include <sys/timerfd.h>

EventLoop* g_loop;

void out_time()
{
    std::cout << "outtime" << std::endl;
}

int main()
{
    
    EventLoop loop;
    g_loop = &loop;
    
    TimerQueue m_queue(g_loop);
    m_queue.addTimer(out_time, 5, 0);

    loop.loop();

}
