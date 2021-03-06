#include "Log.h"


COLORS_DEFINE();
void etool_log_threadProc(void *this)
{
	etool_log *log = (etool_log*)this;
	char *msg = 0;
	while (etool_thread_loop(&(log->thread))) {
		etool_mutexEx_lock(&(log->mutex));
		if (etool_circQueue_empty(log->queue)) {
			etool_condition_wait(&(log->condition), &(log->mutex));
			if (etool_circQueue_empty(log->queue)) { continue; }
		}
		etool_circQueue_exit(log->queue, msg, char*);
#ifdef USE_LOG_STDOUT
		ETOOL_LOG_STDOUT(msg);
#endif
#ifdef USE_LOG_COLOR_STDOUT
		ETOOL_LOG_COLOR_STDOUT(log->level, msg);
#endif
		fwrite(msg, sizeof(char), strlen(msg), log->file);
		etool_mutexEx_unlock(&(log->mutex));
		free(msg);
	}
}

etool_log* etool_log_create(const char *path, const etool_logLevel level)
{
	etool_log *log = (etool_log*)malloc(sizeof(etool_log));
	if (log == 0) { return 0; }
	log->file = fopen(path, "w");
	if (log->file == 0) {
		free(log);
		return 0;
	}
	strcpy(log->path, path);
	log->level = level;
	etool_circQueue_init(log->queue, ETOOL_LOG_QUEUE_SIZE, char*);
	if (log->queue == 0) {
		fclose(log->file);
		free(log);
		return 0;		
	}
	if (etool_mutexEx_load(&(log->mutex)) != 0) {
		fclose(log->file);
		etool_circQueue_free(log->queue);
		free(log);
		return 0;
	}
	if (etool_condition_load(&(log->condition)) != 0) {
		fclose(log->file);
		etool_circQueue_free(log->queue);
		etool_mutexEx_unload(&(log->mutex));
		free(log);
		return 0;
	}
	if (etool_thread_load(&(log->thread)) != 0) {
		fclose(log->file);
		etool_circQueue_free(log->queue);
		etool_mutexEx_unload(&(log->mutex));
		etool_condition_unload(&(log->condition));
		free(log);
		return 0;		
	}
	etool_thread_start(&(log->thread), etool_log_threadProc, log);
	return log;
}

void etool_log_destroy(etool_log *log)
{
	etool_thread_reset(&(log->thread));
	etool_condition_signal(&(log->condition));
	etool_thread_unload(&(log->thread));
	fclose(log->file);
	etool_circQueue_free(log->queue);
	etool_mutexEx_unload(&(log->mutex));
	etool_condition_unload(&(log->condition));
	etool_thread_unload(&(log->thread));
	free(log);
}

int etool_log_path(etool_log *log, const char *path)
{	
	FILE *file = fopen(path, "rw+");
	if (file == 0) {
		return -1;
	}
	strcpy(log->path, path);
	FILE *tmpFile = log->file;
	log->file = file;
	fclose(tmpFile);
	return 0;
}

void etool_log_level(etool_log *log, const etool_logLevel level)
{
	log->level = level;
}

void etool_log_flush(etool_log *log)
{
	fflush(log->file);
}

//const char *fileName, const int lineNum, const char *functionName
void etool_log_printf(etool_log *log, const etool_logLevel level, const char *fmt, ...)
{
	if (level < log->level) {
		return;		
	}
	char msg[ETOOL_LOG_MSG_SIZE];
	va_list argList;
	va_start(argList, fmt);
	vsprintf(msg, fmt, argList);
	va_end(argList);
#ifdef USE_LOG_STDOUT
	ETOOL_LOG_STDOUT(msg);
#endif
#ifdef USE_LOG_COLOR_STDOUT
	ETOOL_LOG_COLOR_STDOUT(level, msg);
#endif
	fwrite(msg, sizeof(char), strlen(msg), log->file);
}

void etool_log_async_printf(etool_log *log, const etool_logLevel level, const char *fmt, ...)
{
	if (level < log->level) {
		return;		
	}
	char *msg = (char*)malloc(sizeof(char) * ETOOL_LOG_MSG_SIZE);
	if (msg == 0) { return; }
	va_list argList;
	va_start(argList, fmt);
	vsprintf(msg, fmt, argList);
	va_end(argList);
	etool_mutexEx_lock(&(log->mutex));
	etool_circQueue_enter(log->queue, msg, char*);
	if (etool_circQueue_empty(log->queue)) {
		etool_condition_signal(&(log->condition));
	}
	etool_mutexEx_unlock(&(log->mutex));
}