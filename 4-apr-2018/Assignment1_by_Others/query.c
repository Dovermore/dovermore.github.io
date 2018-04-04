/* ... */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "query.h"
#include "list.h"
#include "heap.h"

typedef struct{
	float key;
	int data;
} scoreitem_t;

// see Assignment spec: 'Task 1: Array-based accumulation approach'
void print_array_results(Index *index, int n_results, int n_documents) {

	if (n_results==0){
		return;
	}

	if (n_results>n_documents){
		n_results=n_documents;
	}

  float score_record[n_documents],mini_key;
  Heap *scoreheap=new_heap(n_results);
	scoreitem_t finalheap[n_results];
	Document *document=NULL;
  Node *anode;
  int i,mini_data;

	//Initialize score_record
  for (i=0;i<n_documents;i++){
    score_record[i]=0.0;
  }

	//Add num_terms of scores into score_record
  for (i=0;i<index->num_terms;i++){
    anode=index->doclists[i]->head;
    while (anode){
      document = (Document*)anode->data;
      score_record[document->id]+=document->score;
      anode=anode->next;
    }
  }

	//Initialize scoreheap
	for (i=0;i<n_results;i++){
		heap_insert(scoreheap,score_record[i],i);
	}


	//Compare the scores after n_results with the smallest score in scoreheap
	//and replace the smallest one with the new score
	for (i=n_results;i<n_documents;i++){
		if (heap_peek_key(scoreheap)<score_record[i]){
			heap_remove_min(scoreheap);
			heap_insert(scoreheap,score_record[i],i);
		}
	}

	//Print the maxmum k scores and corresponding id
	for (i=n_results-1;i>=0;i--){
			mini_key=heap_peek_key(scoreheap);
			mini_data=heap_remove_min(scoreheap);
			finalheap[i].data=mini_data;
			finalheap[i].key=mini_key;
	}

	for (i=0;i<n_results;i++){
		if (finalheap[i].key>0){
			printf("%6d %.6f\n",finalheap[i].data,finalheap[i].key);
		}
	}
}





// see Assignment spec: 'Task 2: Priority queue-based multi-way merge approach'
void print_merge_results(Index *index, int n_results) {
	if (n_results==0){
		return;
	}

	if (n_results>131563){
		n_results=131563;
	}

	Node* nodearray[n_results];
	Heap* nodeheap=new_heap(index->num_terms);
	scoreitem_t curr;
	Heap* scoreheap=new_heap(n_results);
	int i,minindex,endloop=0;
	Document *document=NULL;
	scoreitem_t finalheap[n_results];
	float mini_key;
	int mini_data;


	for (i=0;i<index->num_terms;i++){
		nodearray[i]=index->doclists[i]->head;
		if (nodearray[i]==NULL){
			endloop++;
			continue;
		}
		document = (Document*)nodearray[i]->data;
		heap_insert(nodeheap,document->id,i);
	}

	minindex=heap_peek_min(nodeheap);
	document = (Document*)nodearray[minindex]->data;
	printf("%s\n", index->terms[20]);
	printf("next id %d and data %f\n", ((Document*)nodearray[20]->next->data)->id, ((Document*)nodearray[20]->next->data)->score);
	printf("document->id = %d\n", document->id);
	printf("document->score = %f\n", document->score);

	curr.key=0.0;
	curr.data=document->id;

	while (endloop<index->num_terms){
		///compare curr.key and score of the smallset id
		printf("\n1\n");
		printf("curr.data is %d,heap_peek_key=%d",curr.data,(int)heap_peek_key(nodeheap));
		assert(document!=NULL);

		printf("doc exists,\n");
		assert(nodearray!=NULL);
		printf("nodearray exists,\n");
		if ((int)heap_peek_key(nodeheap)==curr.data){
			printf("currdata %d\n", curr.data);
			printf("key %d\n", (int)heap_peek_key(nodeheap));
			printf("documentscore=%f\n", document->score);
			printf("2\n");
			curr.key+=document->score;
			printf("3\n");
		}

		else{
			printf("3-\n");
			//scoreheap is not full
			if (scoreheap->actual_item_num<n_results){
				heap_insert(scoreheap,curr.key,curr.data);
			}
			//if scoreheap is full

			else if (curr.key>heap_peek_key(scoreheap)){
					heap_remove_min(scoreheap);
					heap_insert(scoreheap,curr.key,curr.data);
			}
			curr.data=heap_peek_key(nodeheap);
			curr.key=document->score;
		}
		printf("next id %d and data %f\n", ((Document*)nodearray[20]->next->data)->id, ((Document*)nodearray[20]->next->data)->score);
		// printf("asdfasdfa\n");
		// printf("next id %d and data %f\n", ((Document*)nodearray[22]->next->data)->id, ((Document*)nodearray[22]->next->data)->score);
		if (nodearray[minindex]->next!=NULL){
			printf("4\n");
			heap_remove_min(nodeheap);
			printf("pre minindex is %d\n",minindex);
			assert(nodearray[minindex]!=NULL);
			nodearray[minindex]=nodearray[minindex]->next;
			printf("next id %d and data %f\n", ((Document*)nodearray[20]->next->data)->id, ((Document*)nodearray[20]->next->data)->score);
			assert(nodearray[minindex]!=NULL);
			document=(Document*)nodearray[minindex]->data;
			printf("document->id = %d\n", document->id);
			printf("document->score = %f\n", document->score);
			heap_insert(nodeheap,document->id,minindex);
			printf("5\n");
			minindex=heap_peek_min(nodeheap);
			printf("after minindex is %d,\n",minindex);
			printf("%s\n", index->terms[minindex]);
			assert(nodearray[minindex]!=NULL);
			printf("after minindex is %d,\n",minindex);
			assert(nodearray[minindex]->data!=NULL);
			printf("after minindex is %d,\n",minindex);
			document=(Document*)nodearray[minindex]->data;

			printf("6\n");
		}

		else{
			printf("else after 6\n");
			endloop++;
			if (endloop==index->num_terms){
				break;
			}
			heap_remove_min(nodeheap);
			minindex=heap_peek_min(nodeheap);
			if (nodearray[minindex]->next!=NULL){
				printf("other else if\n");
				nodearray[minindex]=nodearray[minindex]->next;
				document=(Document*)nodearray[minindex]->data;
				heap_insert(nodeheap,document->id,minindex);
				minindex=heap_peek_min(nodeheap);
				printf("7\n");
				endloop++;
			}

		}
		printf("8\n");
	}

	//print the result

	for (i=n_results-1;i>=0;i--){
		printf("print");
		if (scoreheap->actual_item_num!=1){
			mini_key=heap_peek_key(scoreheap);
			mini_data=heap_remove_min(scoreheap);
			finalheap[i].data=mini_data;
			finalheap[i].key=mini_key;
		}
		else{
			mini_key=heap_peek_key(scoreheap);
			mini_data=heap_peek_min(scoreheap);
			finalheap[i].data=mini_data;
			finalheap[i].key=mini_key;
			break;
		}
	}

	for (i=0;i<n_results;i++){
		if (finalheap[i].key){
			printf("%6d %.6f\n",finalheap[i].data,finalheap[i].key);
		}
	}
}
