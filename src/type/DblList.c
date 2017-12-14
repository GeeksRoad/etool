#include "DblList.h"


etool_dblList* etool_dblList_create(const unsigned int typeSize, const unsigned int size)
{
	etool_dblList *list = malloc(sizeof(etool_dblList));
	if (list == 0) { return 0; }
	list->memory = etool_memory_create(sizeof(struct _etool_dblNode) + typeSize, size);
	if (list->memory == 0) { free(list); return 0; }
	list->previous = list->next = (struct _etool_dblNode*)list;
	list->typeSize = typeSize;
	return list;
}

void etool_dblList_destroy(etool_dblList *list)
{
	etool_memory_destroy(list->memory);
	free(list);
}

int etool_dblList_size(const unsigned int typeSize, const unsigned int size)
{
	return sizeof(etool_dblList) + etool_memory_size(sizeof(struct _etool_dblNode) + typeSize, size);
}

etool_dblList* etool_dblList_init(void *block, const unsigned int typeSize, const unsigned int size)
{
	if (block == 0) { return 0; }
	etool_dblList *list = block;
	list->memory = etool_memory_init(block + sizeof(etool_dblList), typeSize, size);
	list->previous = list->next = (struct _etool_dblNode*)list;
	list->typeSize = typeSize;
	return list;
}

void etool_dblList_clear(etool_dblList *list)
{
	etool_memory_clear(list->memory);
	list->previous = list->next = (struct _etool_dblNode*)list;
}

int etool_dblList_length(etool_dblList *list)
{
	return list->memory->length;
}

int etool_dblList_empty(etool_dblList *list)
{
	return list->next == (struct _etool_dblNode*)list;
}

int etool_dblList_full(etool_dblList *list)
{
	return list->memory->length == list->memory->size;
}

void* etool_dblList_find(etool_dblList *list, unsigned int index, int direction)
{
	if (index >= list->memory->length) { return 0; }
	struct _etool_dblNode *node = (direction == -1) ? list->previous : list->next;
	while(index > 0) {
		index--;
		node = (direction == -1) ? node->previous : node->next;
	}
	return node->data;
}

int etool_dblList_locate(etool_dblList *list, const void *value)
{
	struct _etool_dblNode *node = list->next;
	int n, index = 0, isFind = 0;
	while(node != (struct _etool_dblNode*)list) {
		if (node->data[0] == ((unsigned char*)value)[0]) {
			isFind = 1;
			for (n = 1; n < list->typeSize; n++) {
				if (node->data[n] != ((unsigned char*)value)[n]) {
					isFind = 0;
					break;
				}
			}
		}
		if (isFind) {
			return index;
		}
		index++;
		node = node->next;
	}
	return -1;
}

int etool_dblList_insert(etool_dblList *list, unsigned int index, const void *value, int direction)
{
	if (index > list->memory->length) {
		return -1;
	}
	struct _etool_dblNode *newNode = etool_memory_malloc(list->memory);
	if (newNode == 0) { return -1; }
	int n;
	newNode->data = (void*)newNode + sizeof(struct _etool_dblNode);
	for (n = 0; n < list->typeSize; n++) {
		newNode->data[n] = ((unsigned char*)value)[n];
	}

	//建立在list可以和node互转
	struct _etool_dblNode *node = (struct _etool_dblNode*)list;
	while(index > 0) {
		index--;
		node = (direction == -1) ? node->previous : node->next;
	}
	if (direction == -1) {
		newNode->previous = node->previous;
		newNode->next = node;
		node->previous = newNode;
		newNode->previous->next = newNode;
	} else {
		newNode->next = node->next;
		newNode->previous = node;
		node->next = newNode;
		newNode->next->previous = newNode;
	}
	return 0;
}

int etool_dblList_erase(etool_dblList *list, unsigned int index, void *value, int direction)
{
	if (index >= list->memory->length) {
		return -1;
	}
	//建立在list可以和node互转
	int n;
	struct _etool_dblNode *node = (direction == -1) ? list->previous : list->next;
	while(index > 0) {
		index--;
		node = (direction == -1) ? node->previous : node->next;
	}
	node->previous->next = node->next;
	node->next->previous = node->previous;
	for (n = 0; n < list->typeSize; n++) {
		((unsigned char*)value)[n] = node->data[n];
	}
	etool_memory_free(list->memory, node);
	return 0;
}

int etool_dblList_copy(etool_dblList *srcList, etool_dblList *dstList)
{
	if (srcList->typeSize != dstList->typeSize) {
		return -1;
	}
	int n;
	struct _etool_dblNode *srcNode = (struct _etool_dblNode*)srcList;
	struct _etool_dblNode *dstNode = (struct _etool_dblNode*)dstList;
	etool_memory_clear(dstList->memory);
	//开始拷贝
	struct _etool_dblNode *newNode = 0;
	while (srcNode->next != 0) {
		newNode = etool_memory_malloc(dstList->memory);
		if (newNode == 0) { return -1; }
		newNode->data = (void*)newNode + sizeof(struct _etool_dblNode);
		for (n = 0; n < srcList->typeSize; n++) {
			newNode->data[n] = srcNode->data[n];
		}
		newNode->previous = dstNode;
		dstNode->next = newNode;
		dstNode = newNode;
		srcNode = srcNode->next;
	}
	dstList->previous = dstNode;
	dstNode->next = (struct _etool_dblNode*)dstList;
	return 0;
}

etool_dblListIterator* etool_dblListIterator_init(etool_dblList *list)
{
	if (list->next == (struct _etool_dblNode*)list) { return 0; }
	etool_dblListIterator *iterator = malloc(sizeof(etool_dblListIterator));
	if (iterator == 0) { return 0; }
	iterator->data = list->next->data;
	iterator->list = (struct _etool_dblNode*)list;
	iterator->next = list->next->next;
	return iterator;
}

int etool_dblListIterator_next(etool_dblListIterator *iterator)
{
	if (iterator->next == iterator->list) {
		free(iterator);
		return 0;
	}
	iterator->data = iterator->next->data;
	iterator->next = iterator->next->next;
	return 1;
}