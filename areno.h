#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef  ARENO_MALLOC
#define  ARENO_MALLOC malloc
#endif //ARENO_MALLOC

#ifndef  ARENO_FREE
#define  ARENO_FREE free
#endif //ARENO_FREE

#define ARENO_CAPACITY 1024*1024 // 1M

typedef struct Areno Areno;
typedef struct Areno {
	void*  start;
	Areno* next;
	size_t count;
} Areno;

void *areno_alloc(Areno* areno, size_t size_in_byte)
{
	assert(size_in_byte < ARENO_CAPACITY && "Requested to much memory");

	if (areno->start == NULL) { // initial alloc
		areno->start = ARENO_MALLOC(ARENO_CAPACITY);
		assert(areno->start != NULL);
	}

	if (areno->count + size_in_byte >= ARENO_CAPACITY) { // not enough place in this areno
		if (areno->next == NULL) {
			areno->next = ARENO_MALLOC(sizeof(Areno));
			assert(areno->next != NULL);

			*areno->next = (Areno) {0};
		}
		return areno_alloc(areno->next, size_in_byte);
	}

	areno->count += size_in_byte;
	return areno->start + areno->count;
}

void *areno_free(Areno* areno)
{
	Areno *current = areno;
	ARENO_FREE(areno->start);
	while (current->next != NULL) {
		Areno *next = current->next;

		if (current != areno) {
			ARENO_FREE(current->start);
			ARENO_FREE(current);
		}

		current = next;
	}
}
