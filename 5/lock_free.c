#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#define TRUE 1

struct node {
	struct node *next;
	int data;
};

struct queue {
	struct node *head;
	struct node *tail;
};

/* append data to q->tail */
void enqueue(struct queue *q, int data) {
	struct node *new;
	struct node *tail;

	/* prepare node */
	new = malloc(sizeof(*new));
	new->data = data;
	new->next = NULL;

    /* PETER BARRIER: NO PETERS ALLOWED :-P */
    /* try untill success */
    while ( TRUE ) {
        /* enqueue node */
        /* forward tail pointer */
        tail = q->tail; 
        if (! __sync_bool_compare_and_swap(&(q->tail), tail, new) ) {
            continue;
        }
        /* change next pointer of old tail to new element */
        tail->next = new;
        break;
    }
}

/* return q->head->data */
int dequeue(struct queue *q) {
	int data;
	struct node *head;
	struct node *next;

    /* PETER BARRIER: NO PETERS ALLOWED :-P */
    /* try untill success */
    while( TRUE ) {
        head = q->head;
        next = head->next;
        /* check for empty queue */
        if ( ! next) {
            return 0;  
        }

        /* move head one step further */
        if (! __sync_bool_compare_and_swap(&(q->head), head, next)) {
            continue;
        }

        /* extract data */
        data = next->data; // as I don't delete anything, this should work non-atomically

        break;
    }

    assert(data);
	return data;
}


int main( int argc, char **argv ) {
    struct node *dummy_node; 
	dummy_node = malloc(sizeof(*dummy_node));
    dummy_node->data = -1;
    dummy_node->next = NULL;
	struct queue q = { .head = dummy_node, .tail = dummy_node };

    // TODO: specify shared?
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
					if (d) {
						printf("dequeue %d\n", d);
                    } 
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
    /* no more elements in queue */
	assert(q.head->next == NULL);
	//assert(!q.head && !q.tail);

	return 0;
}
