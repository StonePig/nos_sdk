/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  graph.h
 *         \unit  graph
 *        \brief  This is a C language graph
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#ifndef __graph_H
#define __graph_H

/* Version infomation */
#define GRAPH_V_MAJOR                       1
#define GRAPH_V_MINOR                       0
#define GRAPH_V_PATCH                       0

/* graph type definition, hiding structural members, not for external use */

typedef struct GRAPH *graph_t;

/** 
 *  \brief traverse callback function type
 *  \param[in] index: vertex index
 *  \param[in] data: address of data
 *  \param[in] size: size of data
 *  \return none
 */
typedef void (*graph_traverse_t)(int index, void *data, int size);

/** 
 *  \brief Creates and initializes a graph
 *  \param[in] max: maximum number of vertices in the graph
 *  \param[in] directed: flag indicating if the graph is directed (non-zero) or undirected (zero)
 *  \return A pointer to the newly created graph, or NULL if creation fails
 */
graph_t graph_create(int max, int directed);

/** 
 *  \brief Destroys a graph and frees associated resources
 *  \param[in] graph: pointer to the graph to be destroyed
 *  \return none
 */
void graph_delete(graph_t graph);

/** 
 *  \brief Adds a vertex to the graph
 *  \param[in] graph: pointer to the graph to which the vertex will be added
 *  \param[in] data: pointer to the data associated with the vertex
 *  \param[in] size: size of the data to be copied
 *  \return The index of the added vertex, or -1 if the addition fails
 */
int graph_add_vertex(graph_t graph, void *data, int size);

/** 
 *  \brief Adds an edge to the graph
 *  \param[in] graph: pointer to the graph where the edge will be added
 *  \param[in] start: index of the starting vertex
 *  \param[in] end: index of the ending vertex
 *  \param[in] weight: weight of the edge
 *  \return 1 if the edge was added successfully, 0 if the addition fails
 */
int graph_add_edge(graph_t graph, int start, int end, int weight);

/** 
 *  \brief Removes a specified vertex and its associated edges from the graph
 *  \param[in] graph: pointer to the graph from which the vertex will be removed
 *  \param[in] index: index of the vertex to be removed
 *  \return 1 if the vertex was removed successfully, 0 if the removal fails
 */
int graph_remove_vertex(graph_t graph, int index);

/** 
 *  \brief Removes a specified edge from the graph
 *  \param[in] graph: pointer to the graph from which the edge will be removed
 *  \param[in] start: index of the starting vertex of the edge
 *  \param[in] end: index of the ending vertex of the edge
 *  \return 1 if the edge was removed successfully, 0 if the removal fails
 */
int graph_remove_edge(graph_t graph, int start, int end);

/** 
 *  \brief Performs a linear search on the graph and applies the provided function to each vertex
 *  \param[in] graph: pointer to the graph
 *  \param[in] func: function to apply to each vertex (takes vertex index, data, and size)
 */
void graph_ls(graph_t graph, graph_traverse_t func);

/** 
 *  \brief Initiates a DFS traversal from a specified starting vertex
 *  \param[in] graph: pointer to the graph to be traversed
 *  \param[in] start: index of the starting vertex
 *  \param[in] func: callback function to be executed for each visited vertex
 *  \return none
 */
void graph_dfs(graph_t graph, int start, graph_traverse_t func);

/** 
 *  \brief Initiates a BFS traversal from a specified starting vertex
 *  \param[in] graph: pointer to the graph to be traversed
 *  \param[in] start: index of the starting vertex
 *  \param[in] func: callback function to be executed for each visited vertex
 *  \return none
 */
void graph_bfs(graph_t graph, int start, graph_traverse_t func);

/** 
 *  \brief Sets data for a specified vertex in the graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] index: index of the vertex
 *  \param[in] data: pointer to the data to be set
 *  \param[in] size: size of the data
 *  \return 1 if the data was set successfully, 0 if it fails
 */
int graph_vertex_set_data(graph_t graph, int index, void *data, int size);

/** 
 *  \brief Retrieves data from a specified vertex in the graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] index: index of the vertex
 *  \param[out] data: pointer to where the data will be copied
 *  \param[in] size: size of the buffer for data
 *  \return 1 if the data was retrieved successfully, 0 if it fails
 */
int graph_vertex_get_data(graph_t graph, int index, void *data, int size);

/** 
 *  \brief Retrieves the data of a specified vertex in the graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] index: index of the vertex
 *  \param[out] size: pointer to store the size of the vertex data (if not NULL)
 *  \return Pointer to the vertex data, or NULL if it fails
 */
void *graph_vertex_data(graph_t graph, int index, int *size);

/** 
 *  \brief Retrieves the out-degree of a specified vertex in a directed graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] index: index of the vertex
 *  \return The out-degree of the vertex, or -1 if it fails
 */
int graph_out_degree(graph_t graph, int index);

/** 
 *  \brief Retrieves the in-degree of a specified vertex in a directed graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] index: index of the vertex
 *  \return The in-degree of the vertex, or -1 if it fails
 */
int graph_in_degree(graph_t graph, int index);

/** 
 *  \brief Checks if two vertices are adjacent in the graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] start: index of the starting vertex
 *  \param[in] end: index of the ending vertex
 *  \return 1 if the vertices are adjacent, 0 if they are not or if it fails
 */
int graph_is_adjacent(graph_t graph, int start, int end);

/** 
 *  \brief Retrieves the weight of the edge between two vertices in a weighted graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] start: index of the starting vertex
 *  \param[in] end: index of the ending vertex
 *  \return The weight of the edge if it exists, or INT_MAX if it fails or the edge does not exist
 */
int graph_get_edge_weight(graph_t graph, int start, int end);

/** 
 *  \brief Sets the weight of the edge between two vertices in a weighted graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] start: index of the starting vertex
 *  \param[in] end: index of the ending vertex
 *  \param[in] weight: new weight for the edge
 *  \return 1 if the weight was set successfully, 0 if it fails
 */
int graph_set_edge_weight(graph_t graph, int start, int end, int weight);

/** 
 *  \brief Checks if a vertex exists in the graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] index: index of the vertex
 *  \return 1 if the vertex exists, 0 if it does not or if it fails
 */
int graph_contains_vertex(graph_t graph, int index);

/** 
 *  \brief Public function to perform topological sort on the graph
 *  \param[in] graph: pointer to the graph
 */
void graph_topological_sort(graph_t graph);

/** 
 *  \brief Public function to find shortest paths from a starting vertex in the graph
 *  \param[in] graph: pointer to the graph
 *  \param[in] start: index of the starting vertex
 */
void graph_shortest_path(graph_t graph, int start);

/** 
 *  \brief Checks if the graph is connected
 *  \param[in] graph: pointer to the graph
 *  \return 1 if the graph is connected, 0 if it is not
 */
int graph_is_connected(graph_t graph);

/** 
 *  \brief Public function to determine if the graph is a complete graph
 *  \param[in] graph: pointer to the graph
 *  \return 1 if the graph is complete, 0 if it is not
 */
int graph_is_complete(graph_t graph);

/** 
 *  \brief Public function to determine if the graph is bipartite
 *  \param[in] graph: pointer to the graph
 *  \return 1 if the graph is bipartite, 0 if it is not
 */
int graph_is_bipartite(graph_t graph);

/** 
 *  \brief Checks if the graph is Eulerian (can traverse each edge exactly once)
 *  \param[in] graph: pointer to the graph
 *  \return 1 if the graph is Eulerian, 0 if it is not
 */
int graph_is_eulerian(graph_t graph);

/** 
 *  \brief Public function to compute the minimum vertex cover for the graph
 *  \param[in] graph: pointer to the graph
 */
void graph_min_vertex_cover(graph_t graph);

#endif
