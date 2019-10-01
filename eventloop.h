#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <assert.h>
#include <signal.h>
#include <poll.h>
#include <vector>

class Channel;
class Poller;

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

	void quit()
	{
		quit_ = true;


	}

	void updateChannel(Channel* channel);


private:
    void abortNotInLoopThread();
	typedef std::vector<Channel *> ChannelList;


    bool looping_;
	const pid_t threadId_;
	bool quit_;
	Poller * poller_;
	ChannelList activeChannels_;
            


};
#endif
