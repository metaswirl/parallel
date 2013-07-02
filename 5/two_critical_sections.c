#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#define DUMMY -1

struct node {
	struct node *next;
	int data;
};

struct queue {
	omp_lock_t head_lock;
	omp_lock_t tail_lock;
	struct node *head;
	struct node *tail;
};

/* append data to q->tail */
void enqueue(struct queue *q, int data) {
	struct node *n;

	/* prepare node */
	n = malloc(sizeof(*n));
	n->data = data;
	n->next = NULL;

	//omp_set_lock(&q->tail_lock);
    #pragma omp critical  // TAIL
    {
        /* enqueue node */
        q->tail->next = n;
        q->tail = n;
    }
	//omp_unset_lock(&q->tail_lock);
}

/* return q->head->data */
int dequeue(struct queue *q) {
	int data;
	struct node *n;

	//omp_set_lock(&q->head_lock);
    #pragma omp critical // HEAD
    {
        #ifdef DEBUG
            printf("start dequeuing\n");
        #endif

        /* check for empty queue */
        if (! q->head->next) {
            omp_unset_lock(&q->head_lock);
            #ifdef DEBUG
                printf("empty queue\n");
            #endif
        } else {
            /* dequeue head node */
            #ifdef DEBUG
                printf("not empty queue\n");
                printf("cur data %d\n", q->head->data);
                printf("next data %d\n", q->head->next->data);
            #endif
            n = q->head;
            q->head = q->head->next;
            data = q->head->data;
            //assert(data);
            q->head->data = DUMMY;
            free(n);
        }
    }
	//omp_unset_lock(&q->head_lock);

	return data;
}


int main( int argc, char **argv ) {
    struct node *dummy_node; 
	dummy_node = malloc(sizeof(*dummy_node));
    dummy_node->data = DUMMY;
    dummy_node->next = NULL;
	struct queue q = { .head = dummy_node, .tail = dummy_node };
	omp_init_lock(&q.tail_lock);
	omp_init_lock(&q.head_lock);

	#pragma omp parallel
	#pragma omp single nowait
	{
		int i;
		for(i=1;i<5;++i) {
			#pragma omp task
			{
				int j;
				for(j = 0; j < 1000; ++j) {
					enqueue(&q, i*1000+j);
					printf("enqueue %d\n", i*1000+j);
				}
			}
			#pragma omp task
			{
				int d, j;
				for(j = 0; j < 1000; ++j) {
					d = dequeue(&q);
					if (d)
						printf("dequeue %d\n", d);
				}
			}
		}
	}

	int d;
	while(1) {
		d = dequeue(&q);
		if (!d)
			break;
		printf("dequeue %d\n", d);
	}
	assert(q.head->data == DUMMY && q.tail->data == DUMMY);
	//assert(!q.head && !q.tail);

	omp_destroy_lock(&q.tail_lock);
	omp_destroy_lock(&q.head_lock);
	return 0;
}
