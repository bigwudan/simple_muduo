#ifndef _POLLER_H
#define _POLLER_H

#include <iostream>
#include <vector>
#include <map>
#include "eventloop.h"

struct pollfd;

class Channel;

class Poller {
public:
    typedef std::vector<Channel *> ChannelList;
    Poller(EventLoop* loop);
    ~Poller();
    
    int poll(int timeoutMs, ChannelList* activeChannels);
    
    void updateChannel(Channel* channel);
    
    void assertInLoopThread(){
        
        ownerLoop_->assertInLoopThread();
    }
public:
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    
    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int, Channel *> ChannelMap;
    EventLoop* ownerLoop_;
    PollFdList pollfds_;
    ChannelMap channels_;
    
};

#endif
