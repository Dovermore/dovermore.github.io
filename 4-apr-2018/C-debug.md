# debugging with dynamic segmentation fault
## When spotted bug with some dynamic structure (array, struct, etc).
## And the segmentation behaves in a way that every time the code is executed, the **stoppings point(eg:index) are different**.
## In most cases this means there might be bugs related to **allocation of the memory**.

Here is a example when I'm helping one of my peers to debug.
As shown in the sample code[Assignment1_by_Others/query.c](https://github.com/Dovermore/dovermore.github.io/blob/master/4-apr-2018/Assignment1_by_Others/query.c):


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
            // printf("next id %d and data %f\n", ((Document*)nodearray[20]->next->data)->id, ((Document*)nodearray[20]->next->data)->score);
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
    ... ...



The bug appears to be constantly occurring when code hits in between `printf("5\n");` and `printf("6\n");`.
However, after asserting, it appears the memory location is a valid location. And we are stuck.
Some more wired behaviors happens when
`
//printf("next id %d and data %f\n", ((Document*)nodearray[20]->next->data)->id, ((Document*)nodearray[20]->next->data)->score);
// printf("asdfasdfa\n");
// printf("next id %d and data %f\n", ((Document*)nodearray[22]->next->data)->id, ((Document*)nodearray[22]->next->data)->score);
`
was not commented (or commented partially), the spot where segfault happens in different indexes. This is frustrating, and can't lead to any certain conclusion.
The first though I came up with was that during some points the array is corrupted by some assignment statements. But there weren't any.

After more than half hour of struggle, I finally starts to think: if the memory wasn't correctly allocated, then the state of program will change for every different line or even different time I ran it.
Then I went to check the declaration of the array. It appears the declaration of array `Node* nodearray[n_results];` should be of `n_queries` length than `n_results`.

This bug took me surprisingly long time to conquer. On one hand, I'm dealing with code that wasn't done by myself. On the other hand, the bug, though simple, behaves nasty.
