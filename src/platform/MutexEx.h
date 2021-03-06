/**
 * Copyright 2017, PaPa.  All rights reserved.
 * 递归锁扩展, 支持条件变量使用
 * (HANDLE)一旦线程拥有者销毁,就会释放锁
 */

#ifndef ETOOL_PLATFORM_MUTEXEX
#define ETOOL_PLATFORM_MUTEXEX

#include <stdlib.h>
#if defined(_windows)
#include <windows.h>
#endif
#if defined(_linux) || defined(_mac) || defined(_android) || defined(_ios)
#include <pthread.h>
#endif

typedef struct _etool_mutexEx {
#if defined(_windows)
	HANDLE mutex;
#endif
#if defined(_linux) || defined(_mac) || defined(_android) || defined(_ios)
	pthread_mutex_t mutex;
#endif
} etool_mutexEx;


/**
 * 创建
 * @return       [实体]
 */
etool_mutexEx* etool_mutexEx_create();

/**
 * 装载
 * @param mutex [not null]
 * @return
 */
int etool_mutexEx_load(etool_mutexEx *mutex);

/**
 * 卸载
 * @param mutex [not null]
 */
void etool_mutexEx_unload(etool_mutexEx *mutex);

/**
 * 销毁
 * @param mutex [not null]
 */
void etool_mutexEx_destroy(etool_mutexEx *mutex);

/**
 * 锁
 * @param mutex [not null]
 */
void etool_mutexEx_lock(etool_mutexEx *mutex);

/**
 * 尝试锁
 * @param  mutex [not null]
 * @return       [error code]
 */
int etool_mutexEx_trylock(etool_mutexEx *mutex);

/**
 * 解锁
 * @param mutex [not null]
 */
void etool_mutexEx_unlock(etool_mutexEx *mutex);

#endif //ETOOL_PLATFORM_MUTEXEX
