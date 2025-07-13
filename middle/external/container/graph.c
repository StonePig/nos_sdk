/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  graph.c
 *         \unit  graph
 *        \brief  This is a C language graph
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* vertex data flags */
#define FLAG_VISITED                0x01

/* The macro definition method for traversal */
#define for_each(graph, i)          for (int i = 0, f = 0; i < (graph)->max && f < (graph)->cvertex; i++) if ((graph)->vertices[i] && ++f > 0)

typedef struct EDGE {
    struct EDGE* next;              /**< The pointer to the next neighbor */
    int index;                      /**< The indices of the adjacent vertices */
    int weight;                     /**< Edge weight */
} EDGE, *edge_t;

typedef struct {
    edge_t firstedge;               /**< The list that stores vertex edges */
    int flag;                       /**< Vertex data flags */
    void *data;                     /**< Address of data */
    int size;                       /**< Size of data */
} VERTEX, *vertex_t;

typedef struct GRAPH {
    vertex_t *vertices;
    int max;                        /**< Maximum number of vertices can be stored */
    int cvertex;                    /**< The count of vertices in the graph */
    int cedge;                      /**< The count of edges in the graph */
    int directed;                   /**< Mark whether it is a directed graph */
} GRAPH;

static int alloc_vertex_index(graph_t graph)
{
    int i;

    for (i = 0; i < graph->max; i++)
    {
        if (!graph->vertices[i])
        {
            return i;
        }
    }

    return -1;
}

static int vertex_set(vertex_t vectex, void* data, int size)
{
    void* d = NULL;

    /* Input value validity check */
    if (!vectex) return 0;
    if (size < 0) return 0;

    /* If the incoming data size is 0, set the air sensitive data directly */
    if (size == 0)
    {
        if (vectex->data) free(vectex->data);
        vectex->data = NULL;
        vectex->size = 0;
        return 1;
    }

    /* If the data size is inconsistent, update the data storage space */
    if (size != vectex->size)
    {
        d = realloc(vectex->data, size);
        if (!d) return 0;
        vectex->data = d;
    }

    /* Data assignment */
    if (data) memcpy(vectex->data, data, size);

    /* Update data size */
    vectex->size = size;

    return 1;
}

/** 
 *  \brief Creates and initializes a vertex
 *  \param[in] data: pointer to the data to be associated with the vertex
 *  \param[in] size: size of the data to be copied
 *  \return A pointer to the newly created vertex, or NULL if the creation fails
 */
static vertex_t vertex_create(void *data, int size)
{
    vertex_t vertex;

    // Allocate memory for the vertex structure
    vertex = (vertex_t)malloc(sizeof(VERTEX));
    if (!vertex) 
    {
        return NULL; // Return NULL if memory allocation fails
    }

    // Initialize the vertex memory to zero
    memset(vertex, 0, sizeof(VERTEX));

    // Set the data for the vertex, return NULL if it fails
    if (!vertex_set(vertex, data, size))
    {
        free(vertex); // Free the allocated memory if setting data fails
        return NULL;
    }

    return vertex; // Return the created vertex
}

/** 
 *  \brief Destroys a vertex and frees associated resources
 *  \param[in] vertex: pointer to the vertex to be destroyed
 *  \return none
 */
static void vertex_destroy(vertex_t vertex)
{
    // Check if the vertex has associated data and free it
    if (vertex->data)
    {
        free(vertex->data); // Free the allocated memory for the data
        vertex->data = NULL; // Set the pointer to NULL to avoid dangling reference
    }
    
    // Free the vertex structure itself
    free(vertex);
}

/** 
 *  \brief Creates and initializes an edge
 *  \param[in] index: index of the edge
 *  \param[in] weight: weight of the edge
 *  \return A pointer to the newly created edge, or NULL if the creation fails
 */
static edge_t edge_create(int index, int weight)
{
    edge_t edge;

    // Allocate memory for the edge structure
    edge = (edge_t)malloc(sizeof(EDGE));
    if (!edge) 
    {
        return NULL; // Return NULL if memory allocation fails
    }

    // Initialize the edge properties
    edge->index = index;   // Set the index of the edge
    edge->weight = weight; // Set the weight of the edge
    edge->next = NULL;     // Initialize the next pointer to NULL
    
    return edge; // Return the created edge
}

/** 
 *  \brief Destroys an edge and frees associated resources
 *  \param[in] edge: pointer to the edge to be destroyed
 *  \return none
 */
static void edge_destroy(edge_t edge)
{
    // Free the memory allocated for the edge structure
    free(edge);
}

graph_t graph_create(int max, int directed)
{
    graph_t graph;

    // Check if the maximum number of vertices is valid
    if (max <= 0)
    {
        return NULL; // Return NULL if max is non-positive
    }

    // Allocate memory for the graph structure
    graph = (graph_t)malloc(sizeof(GRAPH));
    if (!graph) 
    {
        return NULL; // Return NULL if memory allocation fails
    }

    // Allocate memory for the vertex array
    graph->vertices = (vertex_t *)malloc(sizeof(vertex_t) * max);
    if (!graph->vertices)
    {
        free(graph); // Free the graph memory if vertex allocation fails
        return NULL;
    }

    // Initialize graph parameters
    graph->max = max;            // Set the maximum number of vertices
    graph->cvertex = 0;         // Initialize current vertex count
    graph->cedge = 0;           // Initialize current edge count
    graph->directed = directed;  // Set the directed flag
    memset(graph->vertices, 0, sizeof(vertex_t) * max); // Initialize vertex pointers to NULL
    
    return graph; // Return the created graph
}

void graph_delete(graph_t graph)
{
    edge_t edge, temp;

    // Check if the graph is NULL
    if (!graph) return;

    // Iterate through each vertex in the graph
    for_each(graph, i)
    {
        edge = graph->vertices[i]->firstedge; // Get the first edge of the vertex

        // Free all edges associated with the vertex
        while (edge) 
        {
            temp = edge; // Store the current edge
            edge = edge->next; // Move to the next edge
            edge_destroy(temp); // Free the current edge
        }

        // Destroy the vertex and set its pointer to NULL
        vertex_destroy(graph->vertices[i]);
        graph->vertices[i] = NULL;
    }

    // Free the vertex array if it was allocated
    if (graph->vertices) free(graph->vertices);

    // Free the graph structure itself
    free(graph);
}

int graph_add_vertex(graph_t graph, void *data, int size)
{
    vertex_t vertex;
    int index = -1;

    // Check if the graph is NULL
    if (!graph) return -1;

    // Allocate an index for the new vertex
    index = alloc_vertex_index(graph);
    if (index < 0) return -1; // Return -1 if no available index

    // Create and initialize the new vertex
    vertex = vertex_create(data, size);
    if (!vertex)
    {
        return -1; // Return -1 if vertex creation fails
    }

    // Add the vertex to the graph and update vertex count
    graph->vertices[index] = vertex;
    graph->cvertex++;

    return index; // Return the index of the newly added vertex
}

/** 
 *  \brief Adds an edge to the graph
 *  \param[in] graph: pointer to the graph where the edge will be added
 *  \param[in] start: index of the starting vertex
 *  \param[in] end: index of the ending vertex
 *  \param[in] weight: weight of the edge
 *  \return 1 if the edge was added successfully, 0 if the addition fails
 */
int graph_add_edge(graph_t graph, int start, int end, int weight)
{
    edge_t edge;

    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate indices for the vertices
    if (start < 0 || start >= graph->max || end < 0 || end >= graph->max) return 0;
    // Ensure both vertices exist
    if (!graph->vertices[start] || !graph->vertices[end]) return 0;

    // Create a new edge from start to end
    edge = edge_create(end, weight);
    if (!edge)
    {
        return 0; // Return 0 if edge creation fails
    }

    /* Head insert: insert the edge at the beginning of the adjacency list */
    edge->next = graph->vertices[start]->firstedge;
    graph->vertices[start]->firstedge = edge;
    graph->cedge++;

    // If the graph is undirected, add a reverse edge
    if (!graph->directed) 
    {
        edge = edge_create(start, weight);
        if (!edge) 
        {
            // If reverse edge creation fails, remove the original edge
            edge = graph->vertices[start]->firstedge;
            graph->vertices[start]->firstedge = edge->next; // Remove the edge
            edge_destroy(edge); // Free the edge
            graph->cedge--; // Decrement edge count
            return 0; // Return 0 on failure
        }

        // Insert the reverse edge at the beginning of the end vertex's list
        edge->next = graph->vertices[end]->firstedge;
        graph->vertices[end]->firstedge = edge;
        graph->cedge++;
    }

    return 1; // Return 1 to indicate successful addition of the edge
}

int graph_remove_vertex(graph_t graph, int index)
{
    edge_t prev, curr, temp;

    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate the vertex index
    if (index < 0 || index >= graph->max) return 0;
    // Ensure the vertex exists
    if (!graph->vertices[index]) return 0;

    // Delete edges that are incident to the vertex from neighboring vertices
    for_each(graph, i)
    {
        prev = NULL; // Previous edge pointer
        curr = graph->vertices[i]->firstedge;

        // Traverse the adjacency list of the neighboring vertex
        while (curr)
        {
            if (curr->index == index) 
            {
                // If the edge to be removed is the first edge
                if (!prev) 
                {
                    graph->vertices[i]->firstedge = curr->next; // Remove from head
                } 
                else 
                {
                    prev->next = curr->next; // Bypass the current edge
                }

                temp = curr; // Save the edge to be deleted
                curr = curr->next; // Move to the next edge

                edge_destroy(temp); // Free the edge
                graph->cedge--; // Decrement edge count
            } 
            else 
            {
                prev = curr; // Move previous pointer forward
                curr = curr->next; // Move to next edge
            }
        }
    }

    // Delete all edges associated with the vertex being removed
    curr = graph->vertices[index]->firstedge;
    while (curr)
    {
        temp = curr; // Save current edge
        curr = curr->next; // Move to next edge
        edge_destroy(temp); // Free the edge
        graph->cedge--; // Decrement edge count
    }

    // Delete the vertex itself
    vertex_destroy(graph->vertices[index]);
    graph->vertices[index] = NULL; // Nullify the vertex pointer

    // Update the count of vertices
    graph->cvertex--;

    return 1; // Return 1 to indicate successful removal
}

int graph_remove_edge(graph_t graph, int start, int end)
{
    edge_t prev = NULL, curr;

    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate vertex indices
    if (start < 0 || start >= graph->max || end < 0 || end >= graph->max) return 0;
    // Ensure both vertices exist
    if (!graph->vertices[start] || !graph->vertices[end]) return 0;

    // Traverse the adjacency list of the starting vertex
    curr = graph->vertices[start]->firstedge;

    while (curr)
    {
        // Check if the current edge points to the ending vertex
        if (curr->index == end) 
        {
            // Remove the edge from the adjacency list
            if (!prev)
            {
                graph->vertices[start]->firstedge = curr->next; // Remove from head
            } 
            else 
            {
                prev->next = curr->next; // Bypass the current edge
            }

            edge_destroy(curr); // Free the edge
            graph->cedge--; // Decrement edge count

            break; // Exit loop after removing the edge
        } 
        else 
        {
            prev = curr; // Move previous pointer forward
            curr = curr->next; // Move to next edge
        }
    }

    // If the graph is undirected, also remove the reverse edge
    if (!graph->directed) 
    {
        prev = NULL;
        curr = graph->vertices[end]->firstedge;

        while (curr) 
        {
            // Check if the current edge points to the starting vertex
            if (curr->index == start) 
            {
                // Remove the edge from the adjacency list
                if (!prev)
                {
                    graph->vertices[end]->firstedge = curr->next; // Remove from head
                } 
                else 
                {
                    prev->next = curr->next; // Bypass the current edge
                }

                edge_destroy(curr); // Free the edge
                graph->cedge--; // Decrement edge count

                break; // Exit loop after removing the edge
            } 
            else 
            {
                prev = curr; // Move previous pointer forward
                curr = curr->next; // Move to next edge
            }
        }
    }

    return 1; // Return 1 to indicate successful removal
}

void graph_ls(graph_t graph, graph_traverse_t func)
{
    vertex_t vertex;

    // Validate input
    if (!graph) return;  // Check if the graph is valid
    if (!func) return;   // Check if the function is valid

    // Iterate over all vertices in the graph
    for_each(graph, i)
    {
        vertex = graph->vertices[i]; // Get the current vertex
        // Apply the provided function to the vertex
        func(i, vertex->data, vertex->size);
    }
}

/** 
 *  \brief Depth-First Search (DFS) traversal of a graph
 *  \param[in] graph: pointer to the graph to be traversed
 *  \param[in] index: index of the current vertex being visited
 *  \param[in] func: callback function to be executed for each visited vertex
 *  \return none
 */
static void dfs(graph_t graph, int index, graph_traverse_t func)
{
    vertex_t vertex;
    edge_t curr;

    // Check if the vertex at the given index exists
    if (graph->vertices[index])
    {
        vertex = graph->vertices[index];
        // Execute the callback function with the vertex data
        if (func) func(index, vertex->data, vertex->size);
        // Mark the vertex as visited
        vertex->flag |= FLAG_VISITED;
        curr = vertex->firstedge;

        // Traverse all adjacent edges
        while (curr)
        {
            // If the adjacent vertex has not been visited, continue DFS on it
            if (!(graph->vertices[curr->index]->flag & FLAG_VISITED)) 
            {
                dfs(graph, curr->index, func);
            }

            curr = curr->next; // Move to the next edge
        }
    }
}

void graph_dfs(graph_t graph, int start, graph_traverse_t func)
{
    // Check if the graph exists
    if (!graph) return;
    // Validate the starting vertex index
    if (start < 0 || start >= graph->max) return;
    // Ensure the starting vertex exists
    if (!graph->vertices[start]) return;

    // Reset the visited flag for all vertices
    for_each(graph, i)
    {
        graph->vertices[i]->flag &= ~FLAG_VISITED;
    }

    // Start the DFS from the specified starting vertex
    dfs(graph, start, func);
}

/** 
 *  \brief Breadth-First Search (BFS) traversal of a graph
 *  \param[in] graph: pointer to the graph to be traversed
 *  \param[in] index: index of the current vertex being visited
 *  \param[in] func: callback function to be executed for each visited vertex
 *  \param[in] queue: array used for queueing vertices during traversal
 *  \return none
 */
static void bfs(graph_t graph, int index, graph_traverse_t func, int *queue)
{
    int front = 0, rear = 0, nindex;
    vertex_t vertex, nvertex;
    edge_t curr;

    // Get the starting vertex
    vertex = graph->vertices[index];

    // Execute the callback function for the starting vertex
    func(index, vertex->data, vertex->size);
    vertex->flag |= FLAG_VISITED; // Mark the starting vertex as visited

    // Enqueue the starting vertex
    queue[rear++] = index;

    // Process the queue until it's empty
    while (front != rear) 
    {
        index = queue[front++]; // Dequeue the front vertex
        curr = graph->vertices[index]->firstedge; // Get its adjacency list

        // Traverse all adjacent edges
        while (curr) 
        {
            nindex = curr->index; // Index of the adjacent vertex

            // If the adjacent vertex has not been visited
            if (!(graph->vertices[nindex]->flag & FLAG_VISITED)) 
            {
                nvertex = graph->vertices[nindex]; // Get the adjacent vertex

                // Execute the callback function for the adjacent vertex
                func(nindex, nvertex->data, nvertex->size);
                nvertex->flag |= FLAG_VISITED; // Mark it as visited

                // Enqueue the adjacent vertex
                queue[rear++] = nindex;
            }

            curr = curr->next; // Move to the next edge
        }
    }
}

/** 
 *  \brief Initializes BFS traversal with a queue of a given maximum size
 *  \param[in] graph: pointer to the graph to be traversed
 *  \param[in] start: index of the starting vertex
 *  \param[in] func: callback function to be executed for each visited vertex
 *  \param[in] max: maximum size of the queue
 *  \return none
 */
static void t_bfs(graph_t graph, int start, graph_traverse_t func, const int max)
{
    int queue[max]; // Create a queue array

    memset(queue, 0, sizeof(int) * max); // Initialize the queue

    bfs(graph, start, func, queue); // Start BFS traversal
}

void graph_bfs(graph_t graph, int start, graph_traverse_t func)
{
    // Check if the graph exists
    if (!graph) return;
    // Validate the starting vertex index
    if (start < 0 || start >= graph->max) return;
    // Ensure the starting vertex exists
    if (!graph->vertices[start]) return;
    // Ensure the callback function is not NULL
    if (!func) return;

    // Reset the visited flag for all vertices
    for_each(graph, i)
    {
        graph->vertices[i]->flag &= ~FLAG_VISITED;
    }

    // Start BFS from the specified starting vertex
    t_bfs(graph, start, func, graph->max);
}


/** 
 *  \brief Retrieves the number of vertices in the graph
 *  \param[in] graph: pointer to the graph
 *  \return The number of vertices in the graph, or 0 if the graph is NULL
 */
int graph_vertex_count(graph_t graph)
{
    // Check if the graph is NULL
    if (!graph) return 0;

    return graph->cvertex; // Return the count of vertices
}

/** 
 *  \brief Retrieves the number of edges in the graph
 *  \param[in] graph: pointer to the graph
 *  \return The number of edges in the graph, or 0 if the graph is NULL
 */
int graph_edge_count(graph_t graph)
{
    // Check if the graph is NULL
    if (!graph) return 0;

    return graph->cedge; // Return the count of edges
}

int graph_vertex_set_data(graph_t graph, int index, void *data, int size)
{
    vertex_t vertex;

    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate the vertex index
    if (index < 0 || index >= graph->max) return 0;
    // Ensure the vertex exists
    if (!graph->vertices[index]) return 0;
    // Validate the data size
    if (size < 0) return 0;

    vertex = graph->vertices[index];

    // Set the vertex data; return 0 if it fails
    if (!vertex_set(vertex, data, size)) return 0;

    return 1; // Return 1 to indicate success
}

int graph_vertex_get_data(graph_t graph, int index, void *data, int size)
{
    vertex_t vertex;

    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate the vertex index
    if (index < 0 || index >= graph->max) return 0;
    // Ensure the vertex exists
    if (!graph->vertices[index]) return 0;
    // Ensure the data pointer is not NULL
    if (!data) return 0;

    vertex = graph->vertices[index];

    // Validate the size of the buffer
    if (size < vertex->size) return 0;

    // Data assignment using memcpy
    memcpy(data, vertex->data, vertex->size);

    return 1; // Return 1 to indicate success
}

void *graph_vertex_data(graph_t graph, int index, int *size)
{
    vertex_t vertex;

    // Check if the graph is NULL
    if (!graph) return NULL;
    // Validate the vertex index
    if (index < 0 || index >= graph->max) return NULL;
    // Ensure the vertex exists
    if (!graph->vertices[index]) return NULL;

    vertex = graph->vertices[index];

    // If size pointer is provided, store the size of the vertex data
    if (size) *size = vertex->size;

    return vertex->data; // Return the vertex data
}

int graph_out_degree(graph_t graph, int index)
{
    edge_t edge;
    vertex_t vertex;
    int degree = 0;

    // Check if the graph is NULL
    if (!graph) return -1;
    // Validate the vertex index
    if (index < 0 || index >= graph->max) return -1;
    // Ensure the vertex exists
    if (!graph->vertices[index]) return -1;

    vertex = graph->vertices[index];
    edge = vertex->firstedge;

    // Count the edges originating from the vertex
    while (edge)
    {
        degree++;
        edge = edge->next; // Move to the next edge
    }

    return degree; // Return the out-degree
}

int graph_in_degree(graph_t graph, int index)
{
    edge_t edge;
    vertex_t vertex;
    int degree = 0;

    // Check if the graph is NULL
    if (!graph) return -1;
    // Validate the vertex index
    if (index < 0 || index >= graph->max) return -1;
    // Ensure the vertex exists
    if (!graph->vertices[index]) return -1;

    // Iterate over all vertices to count edges pointing to the specified vertex
    for_each(graph, i)
    {
        vertex = graph->vertices[i];
        edge = vertex->firstedge;

        while (edge) 
        {
            // Check if the edge points to the specified vertex
            if (edge->index == index) 
            {
                degree++; // Increment in-degree
            }

            edge = edge->next; // Move to the next edge
        }
    }

    return degree; // Return the in-degree
}

int graph_is_adjacent(graph_t graph, int start, int end)
{
    edge_t edge;

    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate the vertex indices
    if (start < 0 || start >= graph->max || end < 0 || end >= graph->max) return 0;
    // Ensure both vertices exist
    if (!graph->vertices[start] || !graph->vertices[end]) return 0;

    // Iterate through the edges of the starting vertex
    edge = graph->vertices[start]->firstedge;
    while (edge)
    {
        // Check if the current edge points to the ending vertex
        if (edge->index == end) 
        {
            return 1; // Vertices are adjacent
        }
        edge = edge->next; // Move to the next edge
    }

    return 0; // Vertices are not adjacent
}

int graph_get_edge_weight(graph_t graph, int start, int end)
{
    edge_t edge;

    // Check if the graph is NULL
    if (!graph) return INT_MAX;
    // Validate the vertex indices
    if (start < 0 || start >= graph->max || end < 0 || end >= graph->max) return INT_MAX;
    // Ensure both vertices exist
    if (!graph->vertices[start] || !graph->vertices[end]) return INT_MAX;

    // Iterate through the edges of the starting vertex
    edge = graph->vertices[start]->firstedge;
    while (edge)
    {
        // Check if the current edge points to the ending vertex
        if (edge->index == end) 
        {
            return edge->weight; // Return the edge weight
        }

        edge = edge->next; // Move to the next edge
    }

    return INT_MAX; // Return INT_MAX if no edge exists between the vertices
}

int graph_set_edge_weight(graph_t graph, int start, int end, int weight)
{
    edge_t edge;

    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate the vertex indices
    if (start < 0 || start >= graph->max || end < 0 || end >= graph->max) return 0;
    // Ensure both vertices exist
    if (!graph->vertices[start] || !graph->vertices[end]) return 0;

    // Iterate through the edges of the starting vertex
    edge = graph->vertices[start]->firstedge;
    while (edge)
    {
        // Check if the current edge points to the ending vertex
        if (edge->index == end) 
        {
            edge->weight = weight; // Set the new weight
            return 1; // Return 1 to indicate success
        }

        edge = edge->next; // Move to the next edge
    }

    return 0; // Return 0 if no edge exists between the vertices
}

int graph_contains_vertex(graph_t graph, int index)
{
    // Check if the graph is NULL
    if (!graph) return 0;
    // Validate the vertex index
    if (index < 0 || index >= graph->max) return 0;
    // Ensure the vertex exists
    if (!graph->vertices[index]) return 0;

    return 1; // Return 1 if the vertex exists
}

/** 
 *  \brief Performs a topological sort on a directed acyclic graph (DAG)
 *  \param[in] graph: pointer to the graph
 *  \param[in] indegree: array to store the in-degrees of vertices
 *  \param[in] queue: array to use as a queue for vertices with zero in-degree
 */
static void topological_sort(graph_t graph, int *indegree, int *queue)
{
    int i, f = 0;
    int front = 0, rear = 0;
    int index;
    edge_t curr;

    // Calculate the in-degree for each vertex
    for_each(graph, i)
    {
        curr = graph->vertices[i]->firstedge;

        while (curr) 
        {
            indegree[curr->index]++;
            curr = curr->next;
        }
    }

    // Add vertices with zero in-degree to the queue
    for_each(graph, i)
    {
        if (indegree[i] == 0) 
        {
            queue[rear++] = i;
        }
    }

    // Process the queue
    while (front < rear) 
    {
        index = queue[front++];
        printf("%d ", index); // Print the vertex in sorted order
        curr = graph->vertices[index]->firstedge;

        while (curr) 
        {
            // Decrease the in-degree of the neighboring vertex
            if (--indegree[curr->index] == 0) 
            {
                queue[rear++] = curr->index; // Add to queue if in-degree becomes zero
            }
            curr = curr->next;
        }
    }
    printf("\n");
}

/** 
 *  \brief Initializes the queue and in-degree array, then calls the topological sort function
 *  \param[in] graph: pointer to the graph
 *  \param[in] max: maximum number of vertices
 */
static void t_topological_sort(graph_t graph, const int max)
{
    int queue[max];      // Queue for vertices with zero in-degree
    int indegree[max];   // Array to hold in-degrees of vertices

    memset(queue, 0, sizeof(int) * max);      // Initialize queue
    memset(indegree, 0, sizeof(int) * max);   // Initialize in-degree counts

    topological_sort(graph, indegree, queue);  // Perform the topological sort
}

void graph_topological_sort(graph_t graph)
{
    if (!graph) return; // Check if the graph is NULL

    t_topological_sort(graph, graph->max); // Call the helper function
}

/** 
 *  \brief Computes the shortest path from a starting vertex to all other vertices in a weighted graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] start: index of the starting vertex
 *  \param[out] dist: array to store the shortest distances from the start vertex
 *  \param[out] prev: array to store the previous vertex in the shortest path
 *  \param[out] path: array to store the vertices in the shortest path for printing
 */
static void shortest_path(graph_t graph, int start, int *dist, int *prev, int *path)
{
    int j;
    edge_t curr;
    int min_dist, min_index;
    int alt;
    int path_count;
    int prev_index;

    // Initialize distances and previous vertices
    for_each(graph, i)
    {
        dist[i] = INT_MAX; // Set all distances to infinity
        prev[i] = -1;      // Set previous vertices to -1
    }

    dist[start] = 0; // Distance from start to itself is zero

    while (1) 
    {
        min_dist = INT_MAX;
        min_index = -1;

        // Find the unvisited vertex with the smallest distance
        for_each(graph, i)
        {
            if (!(graph->vertices[i]->flag & FLAG_VISITED) && dist[i] < min_dist) 
            {
                min_dist = dist[i];
                min_index = i;
            }
        }

        // If no unvisited vertex is found, exit the loop
        if (min_index == -1) 
        {
            break;
        }

        // Mark the vertex as visited
        graph->vertices[min_index]->flag |= FLAG_VISITED;
        curr = graph->vertices[min_index]->firstedge;

        // Relaxation step
        while (curr) 
        {
            alt = dist[min_index] + graph_get_edge_weight(graph, min_index, curr->index);

            // Update distance and previous vertex if a shorter path is found
            if (alt < dist[curr->index]) 
            {
                dist[curr->index] = alt;
                prev[curr->index] = min_index;
            }

            curr = curr->next;
        }
    }

    // Print the shortest paths from the start vertex to all other vertices
    for_each(graph, i)
    {
        printf("Shortest path from %d to %d: ", start, i);
        if (dist[i] == INT_MAX) 
        {
            printf("not reachable\n");
        } 
        else 
        {
            printf("%d (", dist[i]);

            // Build the path from end to start
            path_count = 0;
            prev_index = i;

            while (prev_index != -1) 
            {
                path[path_count++] = prev_index;
                prev_index = prev[prev_index];
            }

            // Print the path in the correct order
            for (j = path_count - 1; j >= 0; j--) 
            {
                printf("%d", path[j]);

                if (j != 0) 
                {
                    printf(" -> ");
                }
            }

            printf(")\n");
        }
    }
}

/** 
 *  \brief Initializes arrays for distances, previous vertices, and paths, then calls the shortest_path function
 *  \param[in] graph: pointer to the graph
 *  \param[in] start: index of the starting vertex
 *  \param[in] max: maximum number of vertices
 */
static void t_shortest_path(graph_t graph, int start, int max)
{
    int dist[max], prev[max], path[max];

    memset(dist, 0, sizeof(int) * max); // Initialize distance array
    memset(prev, 0, sizeof(int) * max);  // Initialize previous array
    memset(path, 0, sizeof(int) * max);  // Initialize path array

    shortest_path(graph, start, dist, prev, path); // Call the shortest path function
}

void graph_shortest_path(graph_t graph, int start)
{
    // Validate input
    if (!graph) return;
    if (start < 0 || start >= graph->max) return;
    if (!graph->vertices[start]) return;

    t_shortest_path(graph, start, graph->max); // Call the helper function
}

int graph_is_connected(graph_t graph)
{
    // Check if the graph is NULL
    if (!graph) return 0;

    // Perform a depth-first search starting from the first vertex
    graph_dfs(graph, 0, NULL);

    // Check if all vertices were visited
    for_each(graph, i)
    {
        if (!(graph->vertices[i]->flag & FLAG_VISITED)) 
        {
            return 0; // If any vertex is not visited, the graph is not connected
        }
    }

    return 1; // All vertices were visited, the graph is connected
}

/** 
 *  \brief Checks if the graph is a complete graph
 *  \param[in] graph: pointer to the graph
 *  \param[out] degree: array to hold the degree of each vertex
 *  \return 1 if the graph is complete, 0 if it is not
 */
static int is_complete(graph_t graph, int *degree)
{
    // For undirected graphs, each vertex's degree should be n-1 (where n is the number of vertices)
    // For directed graphs, the sum of in-degree and out-degree of each vertex should be n-1
    for_each(graph, i)
    {
        if (graph->directed) 
        {
            // Calculate degree for directed graph
            degree[i] = graph_in_degree(graph, i) + graph_out_degree(graph, i);
        } 
        else 
        {
            // Calculate degree for undirected graph
            degree[i] = graph_out_degree(graph, i);
        }
    }

    // Check if all vertices have the required degree for a complete graph
    for_each(graph, i) 
    {
        if (degree[i] != graph->cvertex - 1) 
        {
            return 0; // Not a complete graph
        }
    }

    return 1; // It is a complete graph
}

/** 
 *  \brief Helper function to check if the graph is complete, initializes the degree array
 *  \param[in] graph: pointer to the graph
 *  \param[in] max: maximum number of vertices
 *  \return 1 if the graph is complete, 0 if it is not
 */
static int t_is_complete(graph_t graph, const int max)
{
    int degree[max]; // Array to hold degrees of vertices

    memset(degree, 0, sizeof(int) * max); // Initialize degree array

    return is_complete(graph, degree); // Call the complete check function
}

int graph_is_complete(graph_t graph)
{
    // Validate input
    if (!graph) return 0;

    return t_is_complete(graph, graph->max); // Call the helper function
}

/** 
 *  \brief Checks if the graph is bipartite using BFS
 *  \param[in] graph: pointer to the graph
 *  \param[out] color: array to hold colors of vertices (1 or 0)
 *  \param[out] queue: array to use as a queue for BFS
 *  \return 1 if the graph is bipartite, 0 if it is not
 */
static int is_bipartite(graph_t graph, int *color, int *queue)
{
    vertex_t vertex;
    edge_t curr;
    int index, next_index;
    int front = 0, rear = 0;

    // Start coloring the first vertex
    color[0] = 1; // Color the first vertex with color 1
    queue[rear++] = 0; // Enqueue the first vertex

    // BFS loop
    while (front < rear) 
    {
        index = queue[front++]; // Dequeue a vertex
        vertex = graph->vertices[index];
        curr = vertex->firstedge;

        // Explore all adjacent vertices
        while (curr) 
        {
            next_index = curr->index;

            // If the adjacent vertex has not been colored
            if (color[next_index] == -1) 
            {
                color[next_index] = 1 - color[index]; // Assign opposite color
                queue[rear++] = next_index; // Enqueue the adjacent vertex
            } 
            // If the adjacent vertex has the same color, the graph is not bipartite
            else if (color[next_index] == color[index]) 
            {
                return 0; // Not bipartite
            }

            curr = curr->next; // Move to the next edge
        }
    }

    return 1; // Graph is bipartite
}

/** 
 *  \brief Helper function to initialize color and queue arrays and check bipartiteness
 *  \param[in] graph: pointer to the graph
 *  \param[in] max: maximum number of vertices
 *  \return 1 if the graph is bipartite, 0 if it is not
 */
static int t_is_bipartite(graph_t graph, const int max)
{
    int color[max]; // Array to store colors of vertices
    int queue[max]; // Queue for BFS

    memset(color, -1, sizeof(int) * max); // Initialize colors to -1 (uncolored)
    memset(queue, 0, sizeof(int) * max);  // Initialize queue

    return is_bipartite(graph, color, queue); // Call bipartite check function
}

int graph_is_bipartite(graph_t graph)
{
    // Validate input
    if (!graph) return 0;

    return t_is_bipartite(graph, graph->max); // Call the helper function
}

int graph_is_eulerian(graph_t graph)
{
    // Validate input
    if (!graph) return 0;

    // For undirected graphs, each vertex's degree must be even
    // For directed graphs, in-degree must equal out-degree for each vertex
    for_each(graph, i)
    {
        if ((graph->directed && graph_in_degree(graph, i) != graph_out_degree(graph, i)) || 
            (!graph->directed && graph_out_degree(graph, i) % 2 != 0)) 
        {
            return 0; // Not Eulerian
        }
    }

    return 1; // The graph is Eulerian
}

/** 
 *  \brief Finds a minimum vertex cover for the graph
 *  \param[in] graph: pointer to the graph
 *  \param[out] flag: array to track whether a vertex is included in the cover
 */
static void min_vertex_cover(graph_t graph, int *flag)
{
    edge_t curr;
    int v;

    // Iterate over all vertices
    for_each(graph, u)
    {
        // If the vertex is not already included in the cover
        if (!flag[u]) 
        {
            // Include the vertex in the cover
            flag[u] = 1;
            curr = graph->vertices[u]->firstedge;

            // Traverse the edges of the vertex
            while (curr) 
            {
                v = curr->index;
                // If the adjacent vertex is not included in the cover
                if (!flag[v]) 
                {
                    // Include the adjacent vertex as well
                    flag[v] = 1;
                    // Print the edge in the vertex cover
                    printf("%d ", u);
                    printf("%d ", v);
                    break; // Stop after adding one edge
                }
                curr = curr->next; // Move to the next edge
            }
        }
    }
    printf("\n"); // Print a newline after all edges are processed
}

/** 
 *  \brief Helper function to initialize the flag array and call the min_vertex_cover function
 *  \param[in] graph: pointer to the graph
 *  \param[in] max: maximum number of vertices
 */
static int t_min_vertex_cover(graph_t graph, const int max)
{
    int flag[max]; // Array to indicate if a vertex is included in the cover

    memset(flag, 0, sizeof(int) * max); // Initialize all flags to 0

    min_vertex_cover(graph, flag); // Call the function to find the vertex cover
}

void graph_min_vertex_cover(graph_t graph)
{
    // Check if the graph is valid
    if (!graph) return;

    t_min_vertex_cover(graph, graph->max); // Call the helper function
}

// TODO: 
// void graph_minimum_spanning_tree(graph_t graph);
// int graph_max_flow(graph_t graph, int source, int sink);
// void graph_articulation_points(graph_t graph);
// void graph_bridges(graph_t graph);
// void graph_minimum_spanning_tree(graph_t graph);
// void graph_topological_sort(graph_t graph);
