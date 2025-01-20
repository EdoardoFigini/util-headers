#ifndef ARENA_H
#define ARENA_H


#define DEFAULT_BLOCK_SIZE 8*1024

#define BLOCK_CAN_ALLOC(block_ptr, bytes) (block_ptr != NULL && block_ptr->size + bytes < block_ptr->capacity)

#ifdef _WIN32
#include <windows.h>

#define malloc(size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
#define free(ptr) HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, ptr);
#else
#include <stdlib.h>
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
} Arena;

void* arena_alloc(Arena* a, size_t bytes);

#ifdef ARENA_IMPLEMENTATION

void* arena_alloc(Arena* a, size_t bytes) {
  if (!a) return NULL;
  if (bytes > DEFAULT_BLOCK_SIZE) return NULL; 

  // new block
  if (a->current == NULL || !BLOCK_CAN_ALLOC(a->current, bytes)) {
    Block *prev = a->current; 
    a->current = malloc(sizeof(Block));

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
  }

  size_t begin = a->current->size;

  a->current->size += bytes;
  
  return (void*)(a->current + begin);
}

void arena_free(Arena *a) {
  if (!a) return;
  if (a->head == NULL) return;

  Block* tmp;
  Block* next;

  for (tmp = a->head; tmp != NULL; tmp = next) {
    next = tmp->next;
    free(tmp);
  }

  a->head = NULL;
  a->current = NULL;
}


#endif // !ARENA_IMPLEMENTATION
#endif // !ARENA_H
