#include "TaskQueue.h"
#include <mutex>

std::mutex mutex;

TaskQueue::TaskQueue()
{
}

int TaskQueue::addTask(const TTask& task)
{
	mutex.lock();
	tasksQueue->push(task);
	mutex.unlock();

	return 1;
}

TTask TaskQueue::popTask()
{
	TTask result;

	mutex.lock();
	if (tasksQueue->empty()) {
		result = NULL;
	}
	else {
		result = tasksQueue->front();
		tasksQueue->pop();
	}
	mutex.unlock();

	return result;
}
