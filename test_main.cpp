#include <iostream>
#include <unistd.h>
#include "thread.h"

void threadFunc()
{
	
	std::cout << "finish" << std::endl;
	
}


int main()
{
	Thread thread(threadFunc);
	thread.start();
	pthread_exit(NULL);
	sleep(10);
	
}


