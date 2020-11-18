#include "TasksPerformer.h"

TasksPerformer::TasksPerformer(TaskQueue _queue)
{
	queue = _queue;
}

void threadFunction(TaskQueue _queue, int _count)
{
	std::vector<std::thread> arr;
	while (_count) {
		TTask task = _queue.popTask();
		if (task != NULL) {
			std::thread thr(task);
			arr.push_back(move(thr));
			_count--;
		}
	}

	for (auto &task : arr) {
		task.join();
	}
}

void TasksPerformer::startPerform(int _maxThreadsCount) {
	std::thread thr(threadFunction, queue, _maxThreadsCount);
	thr.join();
}




