#include <stdio.h>
extern "C" {
#include "../src/type/CircList.h"
#include "../src/type/CircQueue.h"
}

int type_circList_test()
{
	etool_circList *list = etool_circList_create(sizeof(int), 10);
	for (int i = 0; i < 31; i++) {
		printf("add %d\n", i);
		if (etool_circList_insert(list, i, &i) == -1) {
			printf("-1");
			return -1;
		}
		printf("add %d : %p , %p\n", i, list->memory->freeAddr[i], ((struct _etool_circNode*)list->memory->freeAddr[i])->next);
	}
	etool_circList_clear(list);
	for (int i = 0; i < 21; i++) {
		printf("add %d\n", i);
		int a = i*2;
		if (etool_circList_insert(list, i, &i) == -1) {
			printf("-1");
			return -1;
		}
		printf("add %d, %p : %d  , %p\n", i, list->memory->freeAddr[i], list->memory->length, ((struct _etool_circNode*)list->memory->freeAddr[i])->next);
	}
	// printf("addr : %p, %p\n", list->memory->freeAddr, list);
	// for (int i = 0; i < 44; i++) {
	// 	printf("add %d : %p\n", i, list->memory->freeAddr[i]);
	// }
	int i;
	etool_circList_erase(list, 5, &i);
	printf("add %d : %p\n", list->memory->length, list->memory->freeAddr[list->memory->length]);
	etool_circList_insert(list, 5, &i);
	printf("add %d, %p : %d  , %p\n", 5, list->memory->freeAddr[5], list->memory->length, ((struct _etool_circNode*)list->memory->freeAddr[5])->next);

	etool_circList_insert(list, 8, &i);
	etool_circList_insert(list, 11, &i);
	etool_circList_insert(list, 12, &i);
	etool_circList_insert(list, 16, &i);
	etool_circList_insert(list, 14, &i);
	etool_circList_insert(list, 20, &i);

	etool_circList_erase(list, 8, &i);
	etool_circList_erase(list, 11, &i);
	etool_circList_erase(list, 12, &i);
	etool_circList_erase(list, 16, &i);
	etool_circList_erase(list, 14, &i);
	etool_circList_erase(list, 20, &i);

	// int d = 0;
	// printf("ceshi 1 %c \n", i[d]);
	// printf("ceshi 2 %c :\n", i[d++]);
	// printf("ceshi 3 %d\n", d);
	// printf("ceshi 4 %c \n", i[d]);
	// printf("ceshi 5 %c :\n", i[++d]);
	// printf("ceshi 6 %d\n", d);

	// etool_circList_clear(list);
	// for (int i = 0; i < 31; i++) {
	// 	printf("add %d\n", i);
	// 	int a = i*2;
	// 	if (etool_circList_insert(list, i, &a) == -1) {
	// 		printf("-1");
	// 		return -1;
	// 	}
	// 	printf("add %d : %p , %p\n", i, list->memory->freeAddr[i], ((struct _etool_circNode*)list->memory->freeAddr[i])->next);
	// }

	printf("List 5 erase node : %d\n", i);
	i+=2;
	int n = etool_circList_locate(list, &i);
	if (n == -1) {
		printf("List %d locate node\n", n);
	} else {
		printf("List %d locate node\n", n);
	}
	printf("List length : %d\n", etool_circList_length(list));
	printf("List size : %d\n", list->memory->size
		);
	printf("List 5 node : %d\n", *(int*)etool_circList_find(list, 8));
	etool_circListIterator *iterator = etool_circListIterator_init(list);
	do {
		printf("List node : %d\n", **(int**)(iterator));
	} while (etool_circListIterator_next(iterator));

	etool_circList_destroy(list);

	return 0;
}

int type_circQueue_test()
{
	etool_circQueue *queue = etool_circQueue_create(sizeof(int), 10);
	for (int i = 0; i < 31; i++) {
		printf("add %d\n", i);
		if (etool_circQueue_enter(queue, &i) == -1) {
			printf("-1");
			return -1;
		}
	}
	etool_circQueue_clear(queue);
	for (int i = 0; i < 21; i++) {
		printf("add %d\n", i);
		int a = i*2;
		if (etool_circQueue_enter(queue, &i) == -1) {
			printf("-1");
			return -1;
		}
	}
	// printf("addr : %p, %p\n", list->memory->freeAddr, list);
	// for (int i = 0; i < 44; i++) {
	// 	printf("add %d : %p\n", i, list->memory->freeAddr[i]);
	// }
	int i;
	etool_circQueue_get(queue, &i);
	printf("exit %d \n", i);
	etool_circQueue_exit(queue, &i);
	printf("exit %d \n", i);
	etool_circQueue_exit(queue, &i);
	printf("exit %d\n", i);
	etool_circQueue_exit(queue, &i);
	printf("exit %d\n", i);

	// etool_circList_clear(queue);
	// for (int i = 0; i < 31; i++) {
	// 	printf("add %d\n", i);
	// 	int a = i*2;
	// 	if (etool_circList_insert(queue, i, &a) == -1) {
	// 		printf("-1");
	// 		return -1;
	// 	}
	// 	printf("add %d : %p , %p\n", i, queue->memory->freeAddr[i], ((struct _etool_circNode*)queue->memory->freeAddr[i])->next);
	// }


	printf("List length : %d\n", etool_circQueue_length(queue));
	printf("List size : %d\n", queue->size);
	etool_circQueueIterator *iterator = etool_circQueueIterator_init(queue);
	do {
		printf("List node : %d\n", **(int**)(iterator));
	} while (etool_circQueueIterator_next(iterator));
	return 0;	
}

int main(int argc, char *argv[])
{
	printf("hello, world\n");
	type_circList_test();
	return 0;
}

