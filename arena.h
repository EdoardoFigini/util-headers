#ifndef ARENA_H
#define ARENA_H


#define DEFAULT_BLOCK_SIZE 8*1024

#define BLOCK_CAN_ALLOC(block_ptr, bytes) (block_ptr != NULL && block_ptr->size + bytes < block_ptr->capacity)

#ifdef _WIN32
#include <heapapi.h>

#define platform_alloc(size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size)
#define platform_free(ptr) HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, ptr)
#define platform_memcopy(dest, src, size) CopyMemory(dest, src, size)
#else
#include <stdlib.h>

#define platform_alloc(size) malloc(size)
#define platform_free(ptr) free(ptr)
#define platform_memcopy(dest, src, size) memcpy(dest, src, size)
#endif

typedef struct _block {
  unsigned char data[DEFAULT_BLOCK_SIZE];
  size_t size;
  size_t capacity;
  struct _block *next;
} Block;

typedef struct {
  Block *head;
  Block *current;
  size_t block_count;
} Arena;

void* arena_alloc(Arena*, size_t);
void arena_free(Arena*);
char* arena_strdup(Arena*, const char*);

#ifdef ARENA_IMPLEMENTATION

void* arena_alloc(Arena* a, size_t bytes) {
  if (!a) return NULL;
  if (bytes > DEFAULT_BLOCK_SIZE) return NULL; 

  // new block
  if (a->current == NULL || !BLOCK_CAN_ALLOC(a->current, bytes)) {
    Block *prev = a->current; 
    a->current = platform_alloc(sizeof(Block));

#if defined(ZERO_MEM) && !defined(_WIN32) // windows defaults to HEAP_ZERO_MEMORY
    memset(a->current, 0, sizeof(Block));
#endif

    a->current->size = 0;
    a->current->capacity = DEFAULT_BLOCK_SIZE;
    a->current->next = NULL;
    if (a->head == NULL) 
      a->head = a->current;
    else
      prev->next = a->current;

    a->block_count++;
  }

  size_t free_idx = a->current->size;

  a->current->size += bytes;
  
  return (void*)(a->current->data + free_idx);
}

void arena_free(Arena *a) {
  if (!a) return;
  if (a->head == NULL) return;

  Block* tmp;
  Block* next;

  for (tmp = a->head; tmp != NULL; tmp = next) {
    next = tmp->next;
    platform_free(tmp);
  }

  a->head = NULL;
  a->current = NULL;
  a->block_count = 0;
}

char* arena_strdup(Arena* a, const char* str) {
  size_t len = strlen(str) + 1;
  char* copy = (char*)arena_alloc(a, len);
  if (!copy) return NULL;

  platform_memcopy(copy, str, len);
  
  return copy;
}


#endif // !ARENA_IMPLEMENTATION
#endif // !ARENA_H
