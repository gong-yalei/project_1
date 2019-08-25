#include "TaskQueue.h"
#include <iostream>
using namespace std;

TaskQueue::TaskQueue(int maxNode) : 
	m_maxNode(maxNode)
{
	pthread_mutex_init(&m_mutex, NULL);
}


TaskQueue::~TaskQueue()
{
	pthread_mutex_destroy(&m_mutex);
}

bool TaskQueue::addTask(Task & task)
{
	if (m_queue.size() < m_maxNode)
	{
		pthread_mutex_lock(&m_mutex);
		m_queue.push(task);
		pthread_mutex_unlock(&m_mutex);
		return true;
	}
	printLog("队列已满, 无法添加新任务");
	return false;
}

bool TaskQueue::addTask(callback func, void * arg)
{
	if (m_queue.size() < m_maxNode)
	{
		pthread_mutex_lock(&m_mutex);
		Task t;
		t.arg = arg;
		t.function = func;
		m_queue.push(t);
		pthread_mutex_unlock(&m_mutex);
		return true;
	}
	return false;
}

Task TaskQueue::takeTask()
{
	if (m_queue.size() > 0)
	{
		pthread_mutex_lock(&m_mutex);
		Task t = m_queue.front();
		m_queue.pop();
		pthread_mutex_unlock(&m_mutex);
		return t;
	}
	return Task();
}
