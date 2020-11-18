#pragma once
#include "TaskQueue.h"
#include <thread>

class TasksPerformer
{
private:
	TaskQueue queue;
public:
	explicit TasksPerformer(TaskQueue _queue);
	void startPerform(int _maxThreadsCount);
};

