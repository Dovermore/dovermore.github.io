#ifndef HEAP_H
#define HEAP_H
#include <stdbool.h>

#include "heap.h"


typedef struct{
	float key;
	int data;
} item_t;


typedef struct{
  item_t *item;
  int actual_item_num;
  int max_item_num;
} Heap;

Heap *new_heap(int max_size);

void heap_insert(Heap *h,float key,int data);

int heap_remove_min(Heap *h);

int heap_peek_min(Heap *h);

float heap_peek_key(Heap *h);

void free_heap(Heap *h);

void siftup(Heap *h, int i);

void siftdown(Heap *h,int i);

int min_child(Heap *h,int i);

void swap_items(item_t *a,item_t *b); 
#endif
