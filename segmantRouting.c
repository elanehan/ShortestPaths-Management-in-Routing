#include<stdio.h>
#include<stdlib.h>
int num_links;
int each_shortest_path[1000][1000], record_each_pos[1000][1000];
struct Node
{
    int data;
    float capacity_data;
    float load_data;
    float weight_data;
    struct Node* next;
};
struct Node* createNode(int, int);//build a node with these structure
struct Graph
{
    int num_nodes;
    struct Node** adjLists;
    int* visited;//saving nodes having been to
    int* distance;//every distance of node from the starting node
    int* predecessor;//to record this node's previous node
    int segment[1000][1000];//to save the segment we need
};//organize those nodes
struct Graph* createGraph(int num);//build a graph
struct Queue
{
    int items[1000];
    int front;
    int rear;
};
struct Queue* createQueue();
void enqueue(struct Queue* queue, int, int);
int dequeue(struct Queue* queue);
int isEmpty(struct Queue* queue);
void bfs_shortestPath(struct Graph* graph, int start, int num);
void addEdge(struct Graph* graph, int src, int dest, int capacity);//add the edge to adjacent nodes
void trace_and_save_path(int index, int num, int* predecessor, int* distance);

struct Node* createNode(int vertex, int data1)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = vertex;
    newNode->capacity_data = data1;
    newNode->load_data = 0;
    newNode->weight_data = 0;
    newNode->next = NULL;
    return newNode;
}
struct Graph* createGraph(int num)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->num_nodes = num;
    graph->adjLists = malloc(num*sizeof(struct Node));
    graph->visited = malloc(num*sizeof(int));
    graph->distance = malloc(num*sizeof(int));
    graph->predecessor = malloc(num*sizeof(int));
    for(int i=0; i<num; i++)//initialization
    {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
        graph->distance[i] = num + 1;
        graph->predecessor[i] = -1;
    }
    return graph;
}
void initialize_segment(struct Graph *graph, int num1, int num2)
{
    for(int j=0; j<num2; j++)
        graph->segment[num1][j] = -1;

}
struct Queue* createQueue()
{
    struct Queue* queue = malloc(sizeof(struct Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}
int isEmpty(struct Queue* queue)
{
    if(queue->rear == -1)
        return 1;
    else
        return 0;
}
void enqueue(struct Queue* queue, int value, int num)
{
    if(queue->rear == num - 1)
        return;
    else
    {
        if(queue->front == -1)
            queue->front = 0;
        queue->rear++;
        queue->items[queue->rear] = value;
    }
}
int dequeue(struct Queue* queue)
{
    int item;
    if(isEmpty(queue))
        item = -1;
    else
    {
        item = queue->items[queue->front];
        queue->front++;
        if(queue->front > queue->rear)//queue should be reset
            queue->front = queue->rear = -1;
    }
    return item;
}
void addEdge(struct Graph* graph, int src, int dest, int capacity)
{
    //add a edge from src to dest
    struct Node* newNode = createNode(dest, capacity);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
    //add a edge from dest to src
    newNode = createNode(src, capacity);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}
void bfs_shortestPath(struct Graph* graph, int start, int num)
{
    for(int i=0; i<num; i++)//initialization
    {
        graph->visited[i] = 0;
        graph->distance[i] = num + 1;
        graph->predecessor[i] = -1;
    }
    struct Queue* queue = createQueue();
    graph->visited[start] = 1;//regard the start node as visited
    graph->distance[start] = 0;
    graph->predecessor[start] = -1;
    enqueue(queue, start, graph->num_nodes);
    while(!isEmpty(queue))
    {
        int currentVertex = dequeue(queue);//take out the node which has been searched
        struct Node* temp = graph->adjLists[currentVertex];
        //search the take-out node's adjacent nodes
        while(temp)
        {
            int adjNode = temp->data;
            if(graph->visited[adjNode] == 0)
            {
                graph->visited[adjNode] = 1;
                graph->distance[adjNode] = graph->distance[currentVertex] + 1;
                graph->predecessor[adjNode] = currentVertex;
                enqueue(queue, adjNode, graph->num_nodes);
            }
            temp = temp->next;
        }
    }
}
void trace_and_save_path(int index, int num, int* predecessor, int* distance)
{
   // printf("iiii%d\n", index);
    record_each_pos[index][0] = distance[0];
    record_each_pos[index][1] = record_each_pos[index][0] + distance[1] + 1;
    each_shortest_path[index][0] = index;
    //printf("%d %d %d\n", record_each_pos[index][0], record_each_pos[index][1], each_shortest_path[index][0]);
    int i;
    for(i=0; i<num; i++)
    {
        int current = i, k;
        if(i == 0)
            k = 0;
        else
            k=record_each_pos[index][i-1];
        for(int j=record_each_pos[index][i]; j>k; j--)//loop for path from index to i
        {
            each_shortest_path[index][j] = current;
            //printf("%d %d \n", j, current);
            current = predecessor[current];

        }
        record_each_pos[index][i+1] = record_each_pos[index][i] + distance[i+1] + 1;
    }
}
int save_into_segment(struct Graph* graph, int index, int src, int s, int second_start, int dest)//wrong
{
    //printf("index:%d src:%d s:%d second:%d dest: %d\n", index, src, s, second_start, dest);
    int seg=0, p1;
    if(s == 0)
        p1 = 0;
    else
        p1 = record_each_pos[src][s-1] + 1;
    for(p1; p1<=record_each_pos[src][s]; p1++)//save the first segment
    {
        //printf("eachp1:%d\n", each_shortest_path[src][p1]);
        graph->segment[index][seg] = each_shortest_path[src][p1];
        seg++;
    }
    for(int p2=record_each_pos[second_start][dest-1]+2; p2<=record_each_pos[second_start][dest]; p2++)//save the second segment
    {
        //printf("eachp2:%d\n", each_shortest_path[second_start][p2]);
        graph->segment[index][seg] = each_shortest_path[second_start][p2];
        seg++;
    }
    //printf("sav%d\n", seg);
    return seg - 1;
}
int update_load_andWeight(struct Graph* graph, int total, int index, int flow)
{
    int flag=0;
    for(int i=0; i<total; i++)
    {
        //printf("a%d %d %d\n", index, i, graph->segment[index][i]);
        struct Node* cur = graph->adjLists[graph->segment[index][i]];
        while(cur->data != graph->segment[index][i+1])
        {
            //printf("bbbb %d %d\n", cur->data, graph->segment[index][i+1]);
            cur = cur->next;
        }
        //printf("dddddddd\n");
        cur->load_data += flow;
        if((cur->capacity_data - cur->load_data) < 0)
        {
            flag = 1;
            cur->load_data -= flow;
        }
        else if((cur->capacity_data - cur->load_data) == 0)
            cur->weight_data = 10000;
        else
            cur->weight_data = cur->load_data / (cur->capacity_data - cur->load_data);
        cur = graph->adjLists[graph->segment[index][i+1]];
        //printf("fffff\n");
        while(cur->data != graph->segment[index][i])
        {
            //printf("ccccccc\n");
            cur = cur->next;
        }
        cur->load_data = flow;
        if((cur->capacity_data - cur->load_data) < 0)
        {
            flag = 1;
            cur->load_data -= flow;
        }
        else if((cur->capacity_data - cur->load_data) == 0)
            cur->weight_data = 10000;
        else
            cur->weight_data = cur->load_data / (cur->capacity_data - cur->load_data);
    }
    return flag;
}

int main()
{
    int num_nodes, index;
    scanf("%d %d", &num_nodes, &num_links);
    struct Graph* graph = createGraph(num_nodes);

    int preNode[num_links], nextNode[num_links], capacity[num_links], length[100];
    for(int i=0; i<num_links; i++)
        scanf("%d %d %d %d", &index, &preNode[i], &nextNode[i], &capacity[i]);
    for(int i=num_links-1; i>=0; i--)
        addEdge(graph, preNode[i], nextNode[i], capacity[i]);

    for(int i=0; i<num_nodes; i++)//saving shortest path of every node to every node
    {
        bfs_shortestPath(graph, i, num_nodes);
        trace_and_save_path(i, num_nodes, graph->predecessor, graph->distance);
    }

    int num_requests, num_reply=0, num_flow=0;
    scanf("%d", &num_requests);
    int index_request, src, dest, demand_flow;
    for(int i=0; i<num_requests; i++)
    {
        scanf("%d %d %d %d", &index_request, &src, &dest, &demand_flow);
        //recording and counting segment
        float min_weight=1000000;
        int record_len=0, flag=0, secflag=0;
        int for_hop;
        for(int s=0; s<num_nodes; s++)//represent ending node of first segment and index2 of record_each_pos
        {
            //printf("aaaa\n");
            int w1;
            float weight=0;
            //printf("ww%d\n", record_each_pos[src][s+1]);
            if(s==0)
                w1 = 0;
            else
                w1 = record_each_pos[src][s-1] + 1;
            for(w1; w1<record_each_pos[src][s]; w1++)
            {
                //printf("bbb\n");
                struct Node* cur = graph->adjLists[each_shortest_path[src][w1]];
                //printf("ee%d %d %d\n", src, w1, each_shortest_path[src][w1+1]);
                while(cur->data != each_shortest_path[src][w1+1])
                {
                    //printf("%d %d\n", cur->data, each_shortest_path[src][w1+1]);
                    cur = cur->next;
                }
                //printf("w1:%d %d weigh:%f\n", w1, cur->data, cur->weight_data);
                if(cur->weight_data == 10000)
                {
                    //printf("dddd\n");
                    flag = 1;
                    break;
                }
                else
                    flag = 0;
                weight += cur->weight_data;
            }
            //printf("eee\n");
            int second_start=each_shortest_path[src][w1];
            //printf("sec%d %d %d\n", w2, second_start, record_each_pos[second_start][dest]);
            int w2;
            if(dest == 0)
                w2 = 0;
            else
                w2 = record_each_pos[second_start][dest-1]+1;
            for(w2; w2<record_each_pos[second_start][dest]; w2++)
            {
                printf("dddd\n");
                struct Node* cur = graph->adjLists[each_shortest_path[second_start][w2]];
                while(cur->data != each_shortest_path[second_start][w2+1])
                {
                    cur = cur->next;
                }
                printf("iiiiii%d\n", i);
                printf("w2:%d %d weigh:%f\n", w2, cur->data, cur->weight_data);
                if(cur->weight_data == 10000)
                {
                    flag = 1;
                    break;
                }

                else
                    flag = 0;
                weight += cur->weight_data;
                printf("eeeee\n");
                printf("www%d\n", weight);
            }
            if(flag == 1)
                continue;

            printf("ffff\n");
            if(weight < min_weight)
            {
                initialize_segment(graph, num_reply, num_nodes);
                printf("big:%f\n", weight);
                min_weight = weight;
                record_len = for_hop = save_into_segment(graph, num_reply, src, s, second_start, dest);
                printf("ggggg\n");
            }
            else if(weight == min_weight)//if weights are the same
            {
                printf("sss i:%d w:%f\n", i, weight);
                int len1, len2;
                if(s == 0)
                    len1 = record_each_pos[src][s]+1;
                else
                    len1 = record_each_pos[src][s] - record_each_pos[src][s-1];
                if(dest == 0)
                    len2 = record_each_pos[second_start][dest]+1;
                else
                    len2 = record_each_pos[second_start][dest] - record_each_pos[second_start][dest-1];
                printf("len:%d hop:%d\n", len1+len2, for_hop);
                if(len1 + len2 < for_hop)
                {
                    initialize_segment(graph, num_reply+1, num_nodes);
                    record_len = save_into_segment(graph, num_reply, src, s, second_start, dest);
                    for_hop = len1 + len2;
                    //printf("hhhh\n");
                }
                else if(len1 + len2 == for_hop)//find the smallest-index path
                {
                    int ss=0, w1, v;
                    if(s == 0)
                        v = 0;
                    else
                        v = record_each_pos[src][s-1] + 1;
                    for(v; v<record_each_pos[src][s]; v++)
                    {
                        //printf("jjjj\n");
                        if(each_shortest_path[src][v] < graph->segment[num_reply][ss])
                        {
                            initialize_segment(graph, num_reply+1, num_nodes);
                            record_len = save_into_segment(graph, num_reply, src, s, second_start, dest);
                            break;
                        }
                        ss++;
                    }
                }
            }
        }
        if(flag == 1)//if the selected path isn't available then rejected
        {
            initialize_segment(graph, num_reply, num_nodes);
            printf("break\n");
            continue;
        }
        secflag = update_load_andWeight(graph, record_len, num_reply, demand_flow);
        if(secflag == 0)
        {
            length[num_reply] = record_len;
            num_reply++;
            num_flow += demand_flow;
        }
    }
    printf("%d %d\n", num_reply, num_flow);
    for(int i=0; i<num_reply; i++)
    {
        printf("%d ", i);
        for(int j=0; j<=length[i]; j++)
            printf("%d ",  graph->segment[i][j]);
        printf("\n");
    }


    return 0;
}
