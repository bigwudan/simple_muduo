#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <poll.h>

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    
    void assertInLoopThread(){
        if(!isInLoopThread()){
            abortNotInLoopThread();
        }
    
    
    }
        
    static EventLoop* getEventLoopOfCurrentThread();
    static pid_t getCurrPid();


    bool isInLoopThread() const{
    
        return threadId_ == static_cast<pid_t>(::syscall(SYS_gettid));
    }
private:
    void abortNotInLoopThread();
    bool looping_;
    const pid_t threadId_;
            


};
#endif
