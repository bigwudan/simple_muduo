#include <iostream>
#include <unistd.h>
#include "eventloop.h"
#include "thread.h"
#include "channel.h"
#include "poller.h"

#include <cstring>
#include <sys/timerfd.h>

EventLoop* g_loop;

void timeout()
{
    std::cout<< "time out" << std::endl;
    //g_loop->quit();
}


int main()
{
    
    EventLoop loop;
    g_loop = &loop;
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
    Channel channel(&loop, timerfd);


    channel.setReadCallback(timeout);
    channel.enableReading();

    struct itimerspec howlong;
    ::bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;

    ::timerfd_settime(timerfd, 0, &howlong, NULL);
    loop.loop();
    ::close(timerfd);

}
