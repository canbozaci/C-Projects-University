#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define NO_PATH         -1
#define NUM_OF_CITY     7
#define FALSE           0
#define  TRUE           1

typedef struct sGraph
{
    int *dist;
    char *name;
    int city_num;
    struct sGraph **conn;
}Graph;

typedef struct nodestack
{
    Graph *graph;
    struct nodestack *prev_node;
}NodeStack;

NodeStack *init_node = NULL, *last_node = NULL;

/*
   Prague : 0 | Helsinki - 1845 | london 1264
   Helsinki :1 | Tokyo 7815
   Beijing : 2 | Tokyo 1303 | London 8132 | New york 11550 
   Tokyo : 3 | Jakarta 5782 | New York 10838
   Jakarta : 4 | Beijing 4616
   London : 5 | Tokyo 9566
   New York : 6 | London  5567
*/

int distances[NUM_OF_CITY][NUM_OF_CITY] = { {0, 1845, NO_PATH, NO_PATH, NO_PATH, 1264, NO_PATH},
                        {NO_PATH, 0, NO_PATH, 7815, NO_PATH, NO_PATH, NO_PATH},
                        {NO_PATH, NO_PATH, 0, 1303, NO_PATH, 8132, 11550},
                        {NO_PATH, NO_PATH, NO_PATH, 0, 5782, NO_PATH, 10838},
                        {NO_PATH, NO_PATH, 4616, NO_PATH, 0, NO_PATH, NO_PATH},
                        {NO_PATH, NO_PATH, NO_PATH, 9566, NO_PATH, 0, NO_PATH},
                        {NO_PATH, NO_PATH, NO_PATH, NO_PATH, NO_PATH, 5567, 0}

};


char *name[NUM_OF_CITY] = {"Prague","Helsinki","Beijing","Tokyo","Jakarta","London","New York"};

void prevent_inf_loop()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int init_stack(NodeStack **init_node, Graph *graph)
{
    if(init_node != NULL)
    {
        *init_node = (NodeStack *)malloc(sizeof(NodeStack));
        (*init_node)->graph = graph;
        (*init_node)->prev_node = NULL;
    }
    else
    {
        return -1;
    }

    return 0;
}

int push_to_stack(NodeStack **last_node, Graph *graph)
{
    NodeStack *new_node;
    if(last_node != NULL && *last_node != NULL)
    {
        new_node = (NodeStack *)malloc(sizeof(NodeStack));
        new_node->graph = graph;
        new_node->prev_node = *last_node;
        *last_node = new_node;
    }
    else
    {
        return -1;
    }

    return 0;
}

int pop_from_stack(NodeStack **last_node, Graph **graph)
{
    NodeStack *temp;
    if(last_node != NULL && *last_node != NULL)
    {
        *graph = (*last_node)->graph;
        (*last_node)->graph = NULL;
        temp = *last_node;
        *last_node = (*last_node)->prev_node;
        temp->prev_node = NULL;
        free(temp);
    }
    else
    {
        return -1;
    }

    return 0;
}

int find_path(int init, int dest, Graph *graph, int *visited)
{
    int i;
    static int result = 0;
    Graph *temp = NULL, *prev = NULL;
    //printf("Func init %s %d\n",graph[init].name,visited[init]);
    visited[init] = TRUE;
    if(init_node == NULL)
    {
        if(init_stack(&init_node,&graph[init]) == -1)
        {
            exit(1);
        }
        last_node = init_node;
    }
    else
    {
        if(last_node == NULL)
        {
            return result;
        }

        if(push_to_stack(&last_node,&graph[init]) == -1)
        {
            exit(1);
        }
    }

    if(init == dest)
    {
        printf("Path Found:\n");
        while(1)
        {
                pop_from_stack(&last_node,&temp);
                if(temp != NULL)
                {
                    printf("%s ",temp->name);
                    if(prev != NULL)
                    {
                        result += temp->dist[prev->city_num];
                    }
                    prev = temp;
                    temp = NULL;
                    fflush(stdout);
                }
                else
                {
                    printf("Initial City\n");
                    break;
                }
                printf(" <-- ");
                fflush(stdout);
        }
    }
    else
    {
        for(i = 0; graph[init].conn[i] != NULL; ++i)
        {
                if(visited[graph[init].conn[i]->city_num] == FALSE)
                {
                    //printf("City : %s Index : %d\n",graph[init].conn[i]->name, i);
                    find_path(graph[init].conn[i]->city_num,dest,graph,visited);
                }
        }
    }

    pop_from_stack(&last_node,&temp);
    visited[init] = FALSE;

    return result;
}

void print_city_names(Graph *graph)
{
    int i,j;
    for(i = 0; i < NUM_OF_CITY; ++i)
    {
        printf("Number of city : %d City Name: %s\n", i,graph[i].name);
    }
}

void get_city_from_user(int *init, int *dest, Graph *graph)
{
    int valid_input = FALSE;
    /* GET INITIAL CITY */
    while(valid_input == FALSE)
    {
        print_city_names(graph);
        printf("Please enter the index of initial city : ");
        scanf("%d",init);
        prevent_inf_loop();
        if(*init >= 0 && *init < NUM_OF_CITY)
        {
            valid_input = TRUE;
        }
        else
        {
            printf("Please enter number in from range 0 to 7\n");
        }
    }
    /* GET DESTINATION CITY */
    valid_input = FALSE;

    while(valid_input == FALSE)
    {
        print_city_names(graph);
        printf("Please enter the index of destination city : ");
        scanf("%d",dest);
        //prevent_inf_loop();
        if(*dest >= 0 && *dest < NUM_OF_CITY)
        {
            valid_input = TRUE;
        }
        else
        {
            printf("Please enter number in from range 0 to 7\n");
        }
    }
}

int find_connections(int *dist, int *city_numbers)
{
    int i, number = 0;

    for(i = 0; i < NUM_OF_CITY; ++i)
    {
        if(dist[i] != NO_PATH && dist[i] != 0)
        {
            city_numbers[number] = i;
            number++;
        }
    }

    return number;
}

int init_graph(Graph *graph)
{
    int i, j, numbers = 0;
    int city_numbers[7];

    for(i = 0; i < NUM_OF_CITY; ++i)
    {
        graph[i].dist = distances[i];
        graph[i].name = name[i];
        graph[i].city_num = i;
        numbers = find_connections(graph[i].dist,city_numbers);
        graph[i].conn = (Graph **)malloc((numbers + 1) * sizeof(Graph *));
        for(j = 0; j < numbers; ++j)
        {
            graph[i].conn[j] = &graph[city_numbers[j]];
        }
        graph[i].conn[j] = NULL;
    }

    return 0;
}

int main(void)
{
    int initial_city;
    int destination_city;
    int distance;
    int i;
    int visited[7] = {0};
    Graph graph[NUM_OF_CITY];
    init_graph(&graph[0]);
    get_city_from_user(&initial_city, &destination_city, &graph[0]);
    if(initial_city == destination_city)
    {
        printf("Path found : %s to %s\n",graph[initial_city].name, graph[destination_city].name);
        printf("Total distance : 0 km\n");
        exit(0);
    }
    
    distance = find_path(initial_city, destination_city, &graph[0], &visited[0]);

    if(distance == 0)
    {
        printf("No path found from %s to %s\n", graph[initial_city].name, graph[destination_city].name);
    }
    else
    {
        printf("Distance : %d km\n",distance);
    }
    
    getch();

    return 0;
}
