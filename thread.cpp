#include "thread.h"


struct ThreadData
{
	typedef Thread::ThreadFunc ThreadFunc;
	ThreadFunc func_;
	string name_;
	pid_t* tid_;  

	ThreadData(ThreadFunc func,
			   const string& name,
			   pid_t* tid
			   )
		: func_(std::move(func)),
		name_(name),
		tid_(tid)
	{ }
	void runInThread()
	{
		func_();
	}
};


void* startThread(void* obj)
{
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}



Thread::Thread(ThreadFunc func, const string& n)
    : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(std::move(func)),
    name_(n)
{



}


Thread::~Thread()
{
}


void Thread::start()
{
	assert(!started_);
	started_ = true;
	ThreadData* data = new ThreadData(func_, name_, &tid_);
	pthread_create(&pthreadId_, NULL, &startThread, data);
}


