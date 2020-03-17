#include <stdio.h>
#include <stdlib.h>

typedef struct queue *queuePointer;
typedef struct queue
{
    int data;
    queuePointer link;
};
queuePointer front[8192], rear[8192];

int color[8192], distance[8192], predecessor[8192];
int num_nodes;

void addq(int index_queue, int data)
{
    queuePointer temp;
    temp = (temp*) malloc(sizeof(temp));
    temp->data = data;
    temp->link = NULL;
    if(front[index_queue])
        rear[index_queue]->link = temp;
    else
        front[index_queue] = temp;
    rear[index_queue] = temp;
}
void deleteq(int index_queue)
{
     queuePointer temp = front[index_queue];
     if(!temp)
        return;
     front[index_queue] = temp->link;
     free(temp);
     return;
}

void add_edge(int from, int to)
{

}









