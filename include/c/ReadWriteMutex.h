/**
 * Copyright 2017, PaPa.  All rights reserved.
 * 读写锁(可递归)
 */

#ifndef ETOOL_READWRITEMUTEX
#define ETOOL_READWRITEMUTEX

typedef struct _etool_readWriteMutex etool_readWriteMutex;

/**
 * 创建
 * @return       [实体]
 */
etool_readWriteMutex* etool_readWriteMutex_create();

/**
 * 装载
 * @param mutex [not null]
 * @return
 */
int etool_readWriteMutex_load(etool_readWriteMutex *mutex);

/**
 * 卸载
 * @param mutex [not null]
 */
void etool_readWriteMutex_unload(etool_readWriteMutex *mutex);

/**
 * 销毁
 * @param mutex [not null]
 */
void etool_readWriteMutex_destroy(etool_readWriteMutex *mutex);

/**
 * 锁读
 * @param mutex [not null]
 */
void etool_readWriteMutex_lockRead(etool_readWriteMutex *mutex);

/**
 * 尝试锁读
 * @param mutex [not null]
 * @return       [error code]
 */
int etool_readWriteMutex_trylockRead(etool_readWriteMutex *mutex);

/**
 * 解锁读
 * @param mutex [not null]
 */
void etool_readWriteMutex_unlockRead(etool_readWriteMutex *mutex);

/**
 * 锁写
 * @param mutex [not null]
 */
void etool_readWriteMutex_lockWrite(etool_readWriteMutex *mutex);

/**
 * 尝试锁写
 * @param mutex [not null]
 * @return       [error code]
 */
int etool_readWriteMutex_trylockWrite(etool_readWriteMutex *mutex);

/**
 * 解锁写
 * @param mutex [not null]
 */
void etool_readWriteMutex_unlockWrite(etool_readWriteMutex *mutex);

#endif //ETOOL_READWRITEMUTEX
