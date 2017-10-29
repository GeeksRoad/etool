/**
 * Copyright 2017, Geeks Road.  All rights reserved.
 * 信号量 POSIX 无名
 */

#ifndef ETOOL_PLATFORM_SEMAPHORE
#define ETOOL_PLATFORM_SEMAPHORE

#include <stdlib.h>
#if defined(_windows)
#include <windows.h>			
#endif
#if defined(_linux) || defined(_android)
#include <sys/time.h>
#include <semaphore.h>
#endif
#if defined(_mac) || defined(_ios)
#include <mach/semaphore.h>
#include <mach/task.h>
#include <mach/mach.h>
#endif

#define SEMAPHORE_NULL 0

typedef struct _etool_semaphore {
#if defined(_windows)
	HANDLE semaphore;
#endif
#if defined(_linux) || defined(_android)
	sem_t semaphore;
#endif
#if defined(_mac) || defined(_ios)
	semaphore_t semaphore;
#endif
} etool_semaphore;

/**
 * 创建
 * @param  initNum   [not null]
 * @return           [实体]
 */
etool_semaphore* etool_semaphore_create(int initNum);

/**
 * 装载
 * @param semaphore [not null]
 * @param  initNum   [not null]
 */
void etool_semaphore_load(etool_semaphore *semaphore, int initNum);

/**
 * 卸载
 * @param semaphore [not null]
 */
void etool_semaphore_unload(etool_semaphore *semaphore);

/**
 * 销毁
 * @param semaphore [not null]
 */
void etool_semaphore_destroy(etool_semaphore *semaphore);

/**
 * 阻塞
 * @param semaphore [not null]
 */
void etool_semaphore_pend(etool_semaphore *semaphore);

/**
 * 尝试阻塞
 * @param  semaphore [not null]
 * @param  timeOut   [not null]
 * @return           [error code]
 */
int etool_semaphore_trypend(etool_semaphore *semaphore, long timeOut);

/**
 * 通知
 * @param semaphore [not null]
 */
void etool_semaphore_post(etool_semaphore *semaphore);

#endif //ETOOL_PLATFORM_SEMAPHOR
