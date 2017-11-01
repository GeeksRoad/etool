#include "SeqList.h"


etool_seqList* etool_seqList_create(const unsigned int typeSize, const unsigned int size)
{
	etool_seqList *list = malloc(sizeof(etool_seqList));
	if (list == 0) { return 0; }
	list->data = malloc(typeSize * size);
	if (list->data == 0) { return 0; }
	list->typeSize = typeSize;
	list->size = size;
	list->length = 0;
	list->mode = ETOOL_MODE_CREATE;
	return list;
}

void etool_seqList_destroy(etool_seqList *list)
{
	free(list->data);
	free(list);
}

int etool_seqList_size(const unsigned int typeSize, const unsigned int size)
{
	return sizeof(etool_seqList) + typeSize * size;
}

etool_seqList* etool_seqList_init(void *block, const unsigned int typeSize, const unsigned int size)
{
	if (block == 0) { return 0; }
	etool_seqList *list = block;
	list->data = block + sizeof(etool_seqList);
	list->typeSize = typeSize;
	list->size = size;
	list->length = 0;
	list->mode = ETOOL_MODE_INIT;
	return list;
}

void etool_seqList_clear(etool_seqList *list)
{
	list->length = 0;
}

int etool_seqList_length(etool_seqList *list)
{
	return list->length;
}

int etool_seqList_empty(etool_seqList *list)
{
	//1为真, 0为假
	return (list->length == 0) ? 1 : 0;
}

int etool_seqList_full(etool_seqList *list)
{
	//1为真, 0为假
	return (list->length == list->size) ? 1 : 0;
}

void* etool_seqList_find(etool_seqList *list, const unsigned int index)
{
	int offset = index * list->typeSize;
	return list->data + offset;
}

int etool_seqList_locate(etool_seqList *list, void *value)
{
	int offset = 0, isFind = 0;
	while (offset < list->length * list->typeSize) {
		if (list->data[offset] == ((unsigned char*)value)[0]) {
			isFind = 1;
			for (int n = 1; n < list->typeSize; n++) {
				offset++;
				if (list->data[offset] != ((unsigned char*)value)[n]) {
					isFind = 0;
					offset = offset - n;
					break;
				}
			}
		}
		if (isFind) {
			return offset / list->typeSize - 1;
		}
		offset += list->typeSize;
	}
	return -1;
}

int etool_seqList_insert(etool_seqList *list, const unsigned int index, void *value)
{
	if (index > list->length) {
		return -1;
	}
	if (list->length == list->size) {
		switch (list->mode) {
		case ETOOL_MODE_CREATE :
			{ ETOOL_SEQLIST_EXTEND(list); }
			break;
		case ETOOL_MODE_INIT :
			return -1;
		default :
			return -1;
		}	
	}
	//依次后移,空出第index位置
	int offset = list->length * list->typeSize - 1;
	while (offset >= index * list->typeSize) {
		list->data[offset + list->typeSize] = list->data[offset];
		offset--;
	}
	offset++;
	for (int n = 0; n < list->typeSize; n++) {
		list->data[offset + n] = ((unsigned char*)value)[n];
	}
	list->length++;
	return 0;
}

int etool_seqList_insertEx(etool_seqList *list, void *value)
{
	if (list->length == list->size) {
		switch (list->mode) {
		case ETOOL_MODE_CREATE :
			{ ETOOL_SEQLIST_EXTEND(list); }
			break;
		case ETOOL_MODE_INIT :
			return -1;
		default :
			return -1;
		}
	}
	int offset = (list->length - 1) * list->typeSize;
	for (int n = 0; n < list->typeSize; n++) {
		list->data[offset + n] = ((unsigned char*)value)[n];
	}
	list->length++;
	return 0;
}

int etool_seqList_erase(etool_seqList *list, const unsigned int index, void *value)
{
	if (index >= list->length) {
		return -1;
	}
	int offset = index * list->typeSize;
	if (value != 0) {
		for (int n = 0; n < list->typeSize; n++) {
			((unsigned char*)value)[n] = list->data[offset + n];
		}
	}
	while (offset <= list->length * list->typeSize - 1) {
		list->data[offset] = list->data[offset + list->typeSize];
		offset++;
	}
	list->length--;
	return 0;
}

int etool_seqList_eraseEx(etool_seqList *list, const unsigned int index, void *value)
{
	if (index >= list->length) {
		return -1;
	}
	int offset = index * list->typeSize;
	int _offset = (list->length - 1) * list->typeSize;
	if (value != 0) {
		for (int n = 0; n < list->typeSize; n++) {
			((unsigned char*)value)[n] = list->data[offset + n];
			list->data[offset + n] = list->data[_offset + n];
		}
	} else {
		for (int n = 0; n < list->typeSize; n++) {
			list->data[offset + n] = list->data[_offset + n];
		}
	}
	return 0;
}

