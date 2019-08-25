#pragma once
#include <pthread.h>
#include "ThreadPool.h"
#include "TaskQueue.h"
#define LOG
// 线程池类
class ThreadPool
{
public:
	// min: 最少线程个数, max: 最大线程个数
	ThreadPool(int min, int max, TaskQueue* queue);
	~ThreadPool();

	// 工作线程回调函数
	static void* threadWorking(void* arg);

	// 管理者线程回调函数
	static void* threadManager(void* arg);

	// 判断线程是否还存活着
	bool threadIsAlive(pthread_t tid);

	// 给线程池添加任务
	void addPoolTask(Task &task, void (*callback)());

	// 打印日志信息
	inline void printLog(std::string str)
	{
#ifdef LOG
		cout << "File[" << __FILE__ << "], Line[" << __LINE__
			<< "], Infomation[" << str << "]" << endl;
#endif
	}

private:
	pthread_mutex_t lock;               /* 用于锁住本结构体 */
	pthread_mutex_t thread_counter;     /* 记录忙状态线程个数de琐 -- busy_thr_num */
	pthread_cond_t queue_not_full;      /* 当任务队列满时，添加任务的线程阻塞，等待此条件变量 */
	pthread_cond_t queue_not_empty;     /* 任务队列里不为空时，通知等待任务的线程 */

	pthread_t *threads;                 /* 存放线程池中每个线程的tid。数组 */
	pthread_t adjust_tid;               /* 存管理线程tid */
	TaskQueue *task_queue;				/* 任务队列 */

	int min_thr_num;                    /* 线程池最小线程数 */
	int max_thr_num;                    /* 线程池最大线程数 */
	int live_thr_num;                   /* 当前存活线程个数 */
	int busy_thr_num;                   /* 忙状态线程个数 */
	int wait_exit_thr_num;              /* 要销毁的线程个数 */
};

