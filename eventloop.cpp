#include "eventloop.h"
#include "poller.h"
#include "channel.h"



EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

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
    exit(1);
}



void EventLoop::loop()
{
	looping_ = true;
	quit_ = false;  // FIXME: what if someone calls quit() before loop() ?


	while (!quit_)
	{
		activeChannels_.clear();
		poller_->poll(kPollTimeMs, &activeChannels_);
		for(ChannelList::iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it)
		{

			(*it)->handleEvent();

		}	

	}
	looping_ = false;

}

pid_t 
getCurrPid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));

}

void EventLoop::updateChannel(Channel* channel)
{
	
	poller_->updateChannel(channel);
	
}




