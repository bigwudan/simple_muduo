#include <iostream>
#include <poll.h>
#include <sys/timerfd.h>
#include <cstring>



int main(){
    

    int m_timefd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
    int ret =0;
    struct itimerspec howlong;
    ::bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(m_timefd, 0, &howlong, NULL);

    struct pollfd fds[1];
    fds[0].fd = m_timefd;
    fds[0].events = POLLIN;

    std::cout << "ret=" << ret << " ,fd=" << fds[0].fd << ",revents=" << fds[0].revents << std::endl;
    ret = poll(fds,1, 60000);
    std::cout << "ret=" << ret << " ,fd=" << fds[0].fd << ",revents=" << fds[0].revents << std::endl;

}
