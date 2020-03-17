#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define INT_MAX 1010
int graph[INT_MAX][INT_MAX];
int dijkstra_dist[INT_MAX][INT_MAX];
int shortest_dist[INT_MAX];
int smallest_radius[INT_MAX];
int sampled_dist[INT_MAX][INT_MAX];

struct Node
{
    int node1;
    int node2;
    int distance;
    struct Node* next;
};
//build a node with these structure
struct Node* createNode(int from, int to, int dist)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->node1 = from;
    newNode->node2 = to;
    newNode->distance = dist;
    newNode->next = NULL;
    return newNode;
};
struct Node** hash_table;
//build an empty hash table
void createHash(int n)
{
    hash_table = malloc(n*sizeof(struct Node));
    for(int i=0; i<n; i++)
        hash_table[i] = NULL;
}
//to find the vertex with minimum distance value, which hasn't discovered
int minDistance(int discovered[], int n)
{
    int min, min_index;
    min = INT_MAX;
    for(int v=0; v<n; v++)
    {
        if(discovered[v] == -1 && shortest_dist[v] <= min)
        {
            min = shortest_dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstra(int src, int n)
{
    int discovered[n]; //if node i has been discovered, then discovered[i]=1; otherwise, discovered[i]=-1
    //initialize
    for(int i=0; i<n; i++)
    {
        shortest_dist[i] = INT_MAX;
        discovered[i] = -1;
    }
    //distance of itself is 0
    shortest_dist[src] = 0;
    //find shortest path
    for(int count=0; count<n-1; count++)
    {
        //Pick the minimum distance vertex from the discovered array
        //u will be equal to src in the first iteration
        int u = minDistance(discovered, n);
        //marked the picked vertex as discovered
        discovered[u] = 1;
        //Update dist value of the adjacent vertices of the picked vertex
        for(int v=0; v<n; v++)
        {
            //Update shortest_dist[v] only if its discovered[v]=-1, there is an edge from u to v,
            //and total weight of path from src to v through u is smaller than current value of shortest_dist[v]
            if(discovered[v] != 1 && graph[u][v] && shortest_dist[u] != INT_MAX
               && (shortest_dist[u] + graph[u][v] < shortest_dist[v]))
               shortest_dist[v] = shortest_dist[u] + graph[u][v];
        }
    }
}
//Add a node to hash table of the index
void addNode(int index, int from, int to, int dist)
{
    struct Node* newNode = createNode(from, to, dist);
    newNode->next = hash_table[index];
    hash_table[index] = newNode;
}
//Find the distance between node1 and node2, if it wasn't save in hash table, then return -1
int search(int index, int node1, int node2)
{
    struct Node* current = hash_table[index];
    while(current)
    {
        //printf("%d %d\n", current->node1, current->node2);
        if((current->node1 == node1 && current->node2 == node2) || (current->node1 == node2 && current->node2 == node1))
            return current->distance;
        current = current->next;
    }
    return -1;
}

int main()
{
    int num_nodes, num_lines, P;
    scanf("%d %d %d", &num_nodes, &num_lines, &P);
    int index;
    for(int i=0; i<num_lines; i++)
    {
        int src, goal, dist;
        scanf("%d %d %d %d", &index, &src, &goal, &dist);
        //build the graph
        graph[src][goal] = dist;
        graph[goal][src] = dist;
    }
    //Count the shortest distance from every nodes to each node
    for(int i=0; i<num_nodes; i++)
    {
        dijkstra(i, num_nodes);
        for(int v=0; v<num_nodes; v++)
        {
            dijkstra_dist[i][v] = shortest_dist[v];
            //printf("%d ", dijkstra_dist[i][v]);
        }
        //printf("\n");
    }
    //count the sampled number
    double sampled_num;
    sampled_num = sqrt(num_nodes);
    sampled_num = ceil(sampled_num);
    //if the vertex i has been pick, chosen[i]=1
    int pick=0, min, chosen[INT_MAX]={0}, select[INT_MAX]={0}, small_num[INT_MAX]={0}, done2[num_nodes][num_nodes];
    for(int i=0; i<num_nodes; i++)
    {
        for(int j=0; j<num_nodes; j++)
            done2[i][j] = 0;
    }
    for(int i=0; i<sampled_num; i++)
    {
        select[i] = pick;
        dijkstra(pick, num_nodes);
        for(int v=0; v<num_nodes; v++)
        {
            done2[pick][v] = 1;
            done2[v][pick] = 1;
            //Save and find the smallest radius
            if(pick == 0)
            {
                smallest_radius[v] = shortest_dist[v];
                //printf("count:%d %d small:%d\n", i, v, smallest_radius[v]);
                small_num[v] = i;
            }
            else if(shortest_dist[v] < smallest_radius[v])
            {
                //printf("count:%d %d small:%d %d\n", i, v, smallest_radius[v], shortest_dist[v]);
                smallest_radius[v] = shortest_dist[v];
                small_num[v] = i;
            }
            sampled_dist[i][v] = shortest_dist[v];
        }
        chosen[pick] = 1;
        //THe beneath part only execute sampled-number - 1 times
        if(i == sampled_num -1)
            break;
        //Save and find the farthest vertex with smallest index and not-picked
        int max=-1;
        for(int v=0; v<num_nodes; v++)
        {
            if(smallest_radius[v] > max && chosen[v] != 1)
            {
                pick = v;
                max = smallest_radius[v];
            }
        }
    }
    /*for(int i=0; i<num_nodes; i++)
        printf("ss%d ", smallest_radius[i]);
    printf("\n");*/
    //build hash table
    double x;
    int size, num_dist=0, done1[INT_MAX]={0};
    x = pow(num_nodes, 1.5);
    size = ceil(x);
    createHash(size);
    for(int count=0; count<sampled_num; count++)
    {
        for(int v=0; v<num_nodes; v++)
        {
            if(v == select[count] || done1[v] == 1)
                continue;
            //hash function
            int index = ((select[count]+1) * (v+1)) % P % size;
            addNode(index, select[count], v, sampled_dist[count][v]);
            done1[select[count]] = 1;
            num_dist++;
        }
    }
    //printf("%d\n", num_dist);
    //Save it into the hash table, if distance between nodes is shorter than nodes' smallest radius
    for(int i=1; i<num_nodes; i++)
    {
        for(int j=0; j<num_nodes; j++)
        {
            //if the nodes aren't connected, then keep it out of the consideration
            if(done2[j][i] == 1 || i == j)
                continue;
            if(dijkstra_dist[i][j] <= smallest_radius[i] || dijkstra_dist[i][j] <= smallest_radius[j])
            {
                int index = ((i+1) * (j+1)) % P % size;
                //printf("rr%d %d\n", i, j);
                addNode(index, i, j, dijkstra_dist[i][j]);
                done2[i][j] = 1;
                num_dist++;
            }
        }
    }
    printf("%d\n", num_dist);
    int num_query;
    scanf("%d", &num_query);
    for(int i=0; i<num_query; i++)
    {
        int from, to;
        scanf("%d %d %d", &index, &from, &to);
        int hash_f = ((from+1) * (to+1)) % P % size;
        int dist = search(hash_f, from, to);
        //if nodes aren't in the hash table, then check the from's smallest radius and to's corresponding distance; change the order and redo it;
        //otherwise, simply search in the hash table
        if(dist != -1)
            printf("%d %d %d %d\n", i, from, to, dist);
        else
        {
            int dist1 = smallest_radius[from];
            dist1 = dist1 + sampled_dist[small_num[from]][to];
            int dist2 = smallest_radius[to];
            dist2 = dist2 + sampled_dist[small_num[to]][from];
            if(dist1 < dist2)
                printf("%d %d %d %d\n", i, from, to, dist1);
            else
                printf("%d %d %d %d\n", i, from, to, dist2);
        }
    }

    return 0;
}
