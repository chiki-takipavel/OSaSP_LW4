#pragma once
#include <queue>
#include <functional>

typedef std::function<void()> TTask;

class TaskQueue
{
public:
	TaskQueue();
	int addTask(const TTask& task);
	TTask popTask();
private:
	std::queue<TTask>* tasksQueue = new std::queue<TTask>;
};