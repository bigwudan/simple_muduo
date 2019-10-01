#include "eventloop.h"



EventLoop* t_loopInThisThread = 0;


EventLoop::EventLoop()
    :looping_(false),threadId_(static_cast<pid_t>(::syscall(SYS_gettid)))
{
    if(t_loopInThisThread){
        std::cout << "exists " << threadId_ << std::endl;
    
    }else{
        t_loopInThisThread = this;
    }

}     

EventLoop::~EventLoop(){

    t_loopInThisThread = NULL;

};


EventLoop* getEventLoopOfCurrentThread()
{

    return t_loopInThisThread;

}

void EventLoop::abortNotInLoopThread()
{
    std::cout << "EventLoop::abortNotInLoopThread - EventLoop " << this
        << " was created in threadId_ = " << threadId_
        << ", current thread id = " <<  static_cast<pid_t>(::syscall(SYS_gettid))<<std::endl;
}



void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;

    ::poll(NULL, 0, 5*1000);
    std::cout << "eventloop " << this << "stop looping";
    looping_ = false;


}

pid_t 
getCurrPid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));

}

