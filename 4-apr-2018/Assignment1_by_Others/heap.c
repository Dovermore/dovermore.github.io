#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "heap.h"


//A function to return an empty Heap with capacity for max_size items.
Heap*
new_heap(int max_size){
  Heap *heap;
  int i;
	heap=((Heap*)malloc(max_size*sizeof(*heap)));
	assert(heap);
  heap->item=((item_t*)malloc(max_size*sizeof(*(heap->item))));
  assert(heap->item);
  heap->max_item_num=max_size;
	heap->actual_item_num=0;
  for (i=0;i<max_size;i++){
    heap->item[i].data=0;
    heap->item[i].key=0.0;
  }
  return heap;
}

//A function to swap item i up the heap into its rightful position.
void
siftup(Heap *h, int i){
  int indexing;


  while (i!=0){
    indexing=(i-1)/2;
    if (h->item[i].key<h->item[indexing].key){
      swap_items(h->item+i, h->item+indexing);
      i=indexing;
    }
    else{
			break;
		}
  }
}

//A function to swaptwo items with item_t types
void
swap_items(item_t *a,item_t *b){
	item_t temp;
	temp=*a;
	*a=*b;
	*b=temp;
}



//A functiion to swap item i down the heap into tis rightful position.
void
siftdown(Heap *h,int i){
  int child;

	while ((child=min_child(h,i))>0){
		if (h->item[child].key<h->item[i].key){
			swap_items(&(h->item[child]),&(h->item[i]));
			i=child;
		}
		else{
			break;
		}
	}
}

//A function to find the index of the minimun-key child of item i.
int
min_child(Heap *h,int i){

  int l_child=i*2+1,r_child=i*2+2;
	int mchild=-1;
	if (l_child<h->actual_item_num){
		mchild=l_child;
	}
	if (r_child<h->actual_item_num&&
							h->item[r_child].key<h->item[l_child].key){
		mchild=r_child;
	}
	return mchild;
}

//A function to insert data into h with proiority key.
void
heap_insert(Heap *h, float key, int data){
  assert(h->actual_item_num<h->max_item_num);
  h->item[h->actual_item_num].key=key;
  h->item[h->actual_item_num].data=data;
	h->actual_item_num++;
  siftup(h,h->actual_item_num-1);
}

//A function to remove and return the item with the smallest key in h.
int heap_remove_min(Heap *h){
  assert(h!=NULL);
  assert(h->actual_item_num>0);
	assert(h->max_item_num>=h->actual_item_num);

	int minitem=heap_peek_min(h);
	swap_items(&h->item[h->actual_item_num-1],&h->item[0]);
	h->actual_item_num--;
	siftdown(h,0);
	return minitem;
}

//A function to return, but not remove, the item with the smallest key in h.
int heap_peek_min(Heap *h){
  assert(h!=NULL);
  assert(h->actual_item_num>0);
	assert(h->max_item_num>=h->actual_item_num);
	return h->item[0].data;
}

//A function to return the key of the item with the smallest key in h.
float heap_peek_key(Heap *h){
  assert(h!=NULL);
  assert(h->actual_item_num>0);
	assert(h->max_item_num>=h->actual_item_num);
  float minkey=h->item[0].key;
  return minkey;
}

//A function to de-allocate all memory used by h.
void free_heap(Heap *h){
  assert(h!=NULL);

  item_t *item=h->item;

  free(item);
  free(h);
}
