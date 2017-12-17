#include "Executor.h"


void etool_worker_threadProc(void *this)
{
	struct _etool_worker *worker = (struct _etool_worker*)this;
	void *_work[2];
	while(etool_thread_loop(&(worker->thread))) {
		etool_mutexEx_lock(&(worker->mutex));
		if (etool_circQueue_exit(worker->queue, (void*)&_work) != 0) {
			worker->tick = 1;
			etool_condition_wait(&(worker->condition), &(worker->mutex));
			worker->tick = 0;
			etool_circQueue_exit(worker->queue, (void*)&_work);
		}
		etool_mutexEx_unlock(&(worker->mutex));
		((etool_workProc*)(_work[0]))(_work[1]);
	}
}

void etool_executor_threadProc(void *this)
{
	etool_executor *executor = (etool_executor*)this;
	int index = executor->maxCount;
	struct _etool_worker *worker;
	while(etool_thread_loop(&(executor->thread))) {
		while(index--) {
			worker = executor->workers[index];
			etool_mutexEx_lock(&(worker->mutex));
			if (worker->tick != 0) {
				if (worker->tick != ETOOL_MAX_TICK){
					worker->tick++;
				} else {
					etool_thread_cancel(&(worker->thread));
					etool_condition_signal(&(worker->condition));
				}
			}
			etool_mutexEx_unlock(&(worker->mutex));
		}
		etool_os_sleep(ETOOL_TICK_TIME);
	}
}

etool_executor* etool_executor_create(unsigned int minCount, unsigned int maxCount)
{
	etool_executor *executor = malloc(sizeof(etool_executor));
	if (executor == 0) { return 0; }
	executor->workers = malloc(sizeof(struct _etool_worker*) * maxCount);
	if (executor->workers == 0) { free(executor); return 0; }
	if (minCount != maxCount) {
		if (etool_thread_load(&(executor->thread)) != 0) {
			free(executor->workers);
			free(executor);
			return 0;
		}
		etool_thread_start(&(executor->thread), etool_executor_threadProc, executor);
	}
	executor->minCount = minCount;
	executor->maxCount = maxCount;
	struct _etool_worker *worker;
	while (maxCount--) {
		worker = malloc(sizeof(struct _etool_worker));
		if (worker == 0) {
			etool_executor_destroy(executor);
			return 0;
		}
		worker->queue = etool_circQueue_create(sizeof(etool_workProc*) + sizeof(void*), ETOOL_WORK_QUEUE_SIZE);
		if (worker->queue == 0) {
			free(worker);
			etool_executor_destroy(executor);
			return 0;
		}
		if (etool_mutexEx_load(&(worker->mutex)) != 0) {
			etool_circQueue_destroy(worker->queue);
			free(worker);
			etool_executor_destroy(executor);
			return 0;
		}
		if (etool_condition_load(&(worker->condition)) != 0) {
			etool_mutexEx_unload(&(worker->mutex));
			etool_circQueue_destroy(worker->queue);
			free(worker);
			etool_executor_destroy(executor);
			return 0;
		}
		if (etool_thread_load(&(worker->thread)) != 0) {
			etool_mutexEx_unload(&(worker->mutex));
			etool_condition_unload(&(worker->condition));
			etool_circQueue_destroy(worker->queue);
			free(worker);
			etool_executor_destroy(executor);
			return 0;
		}
		if (maxCount < minCount) {
			worker->tick = ETOOL_MAX_TICK;
			etool_thread_start(&(worker->thread), etool_worker_threadProc, worker);
		} else {
			worker->tick = 0;
		}
		executor->workers[maxCount] = worker;
	}
	return executor;
}

void etool_executor_destroy(etool_executor *executor)
{
	int index = executor->maxCount;
	struct _etool_worker *worker;
	while(index--) {
		worker = executor->workers[index];
		etool_thread_cancel(&(worker->thread));
		etool_condition_signal(&(worker->condition));
		etool_mutexEx_unload(&(worker->mutex));
		etool_condition_unload(&(worker->condition));
		etool_circQueue_destroy(worker->queue);
		etool_thread_unload(&(worker->thread));
		free(worker);
	}
	free(executor->workers);
	free(executor);
}

void etool_executor_work(etool_executor *executor, etool_workProc *work, void *param, const int index)
{
	if (index < 0 || index >= executor->maxCount) {
		int index = executor->maxCount - 1;
		int _weight, weight = etool_circQueue_length(executor->workers[index]->queue);
		while (index--) {
			if (weight == 0) {
				break;
			}
			_weight = etool_circQueue_length(executor->workers[index]->queue);
			if (weight > _weight) {
				weight = _weight;
			}
		}
	}
	struct _etool_worker *worker = executor->workers[index];
	etool_mutexEx_lock(&(worker->mutex));
	void *_work[2] = {work, param};
	etool_circQueue_enter(worker->queue, _work);
	if (worker->tick != 0) {
		if (worker->tick != ETOOL_MAX_TICK){
			etool_condition_signal(&(worker->condition));
		} else {
			etool_thread_start(&(worker->thread), etool_worker_threadProc, worker);
		}
	}
	etool_mutexEx_unlock(&(worker->mutex));
}