#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_ARENA_MEM_POOL 1024*1024 // 1M
typedef struct Arena Arena;
Arena   arena_create();
void*   arena_alloc(Arena *ar, size_t cap);
void*   arena_alloc(Arena *ar, size_t cap);

#ifdef    ARENA_IMPLEMENTATION

// arena 
#define INIT_ARENA_MEM_POOL 1024*1024 // 1M
typedef struct {
	void *pool;
	size_t count;
} Arena;


Arena arena_create()
{
	void *mem = malloc(INIT_ARENA_MEM_POOL);
	assert(mem != NULL && "No enough RAM");

	return (Arena) {
		.count = 0,
		.pool  = mem,
	};
}
void* arena_alloc(Arena *ar, eize_t cap)
{
	if (ar->count + cap >= INIT_ARENA_MEM_POOL) return NULL;
	void *addr = (char*)ar->pool + ar->count;
	ar->count += cap;
	return addr;
}
void arena_free(Arena *ar)
{
	free(ar->pool);
}

#endif // ARENA_IMPLEMENTATION
