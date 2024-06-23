#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "queue.h"

Graph* createGraph()
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));

    if (!graph) return NULL;

    graph->nodesNum      = 0;
    graph->nodesCapacity = 10;
    graph->nodes         = (Node*)malloc(sizeof(Node) * graph->nodesCapacity);
    graph->adjList       = (Edge**)malloc(sizeof(Edge*) * graph->nodesCapacity);

    return graph;
}

void freeGraph(Graph* graph)
{
    for (size_t i = 0; i < graph->nodesCapacity; ++i) {
        Edge* current = graph->adjList[i];
        while (current != NULL) {
            Edge* temp = current;
            current    = current->next;
            free(temp->ports);
            free(temp);
        }
    }
    free(graph->adjList);
    free(graph->nodes);
    free(graph);
}

int addNode(Graph* graph, const char* name, uint32_t port, uint32_t id)
{
    Node n;
    strncpy(n.name, name, NAMELEN - 1);
    n.port = port;
    n.i    = id;

    if (graph->nodesNum >= graph->nodesCapacity) {
        graph->nodesCapacity *= 2;
        graph->nodes = realloc(graph->nodes, sizeof(Node) * graph->nodesCapacity);
    }
    graph->nodes[graph->nodesNum++] = n;

    return 0;
}

int addEdge(Graph* g, uint32_t src, uint32_t dst, uint32_t* ports, uint32_t portsNum)
{
    if (src >= g->nodesNum || dst >= g->nodesNum) {
        return NO_SUCH_NODE;
    }

    // dst -> src ребро
    Edge* e     = (Edge*)malloc(sizeof(Edge));
    e->ports    = (uint32_t*)malloc(sizeof(uint32_t) * portsNum);
    e->portsNum = portsNum;
    memcpy(e->ports, ports, portsNum * sizeof(uint32_t));
    e->next = g->adjList[src];
    e->i    = dst;

    g->adjList[src] = e;

    if (src != dst) {
        // src -> dst ребро
        Edge* e2     = (Edge*)malloc(sizeof(Edge));
        e2->ports    = (uint32_t*)malloc(sizeof(uint32_t) * portsNum);
        e2->portsNum = portsNum;
        memcpy(e2->ports, ports, portsNum * sizeof(uint32_t));
        e2->next = g->adjList[dst];
        e2->i    = src;

        g->adjList[dst] = e2;
    }

    return 0;
}

static void removeEdgesWithNode(Graph* g, uint32_t node)
{
    for (uint32_t i = 0; i < g->nodesNum; ++i) {
        if (i == node) continue;

        Edge* prev    = NULL;
        Edge* current = g->adjList[i];
        while (current != NULL) {
            if (current->i == node) {
                if (prev == NULL) {
                    g->adjList[i] = current->next;
                }
                else {
                    prev->next = current->next;
                }
                free(current->ports);
                free(current);
                current = (prev == NULL) ? g->adjList[i] : prev->next;
            }
            else {
                prev    = current;
                current = current->next;
            }
        }
    }
}

int removeNode(Graph* g, uint32_t id)
{
    if (id >= g->nodesNum) {
        return NO_SUCH_NODE;
    }

    // Удаление всех исходящих ребер из узла id
    Edge* current = g->adjList[id];
    while (current) {
        Edge* tmp = current;
        current   = current->next;
        free(tmp->ports);
        free(tmp);
    }
    g->adjList[id] = NULL;

    // Удаление всех входящих ребер в узел id
    removeEdgesWithNode(g, id);

    // Сдвиг узлов в массиве nodes
    for (uint32_t i = id; i < g->nodesNum - 1; ++i) {
        g->nodes[i]   = g->nodes[i + 1];
        g->nodes[i].i = i;
    }
    g->nodesNum--;

    // Обновление списка смежности
    for (uint32_t i = 0; i < g->nodesNum; ++i) {
        Edge* edge = g->adjList[i];
        while (edge) {
            if (edge->i > id) {
                edge->i--;
            }
            edge = edge->next;
        }
    }

    return 0;
    return 0;
}

int removeEdge(Graph* g, uint32_t src, uint32_t dst)
{
    if (src >= g->nodesNum || dst >= g->nodesNum) {
        return NO_SUCH_NODE;
    }

    Edge* a    = g->adjList[src];
    Edge* prev = NULL;

    if (a == NULL) return NO_SUCH_EDGE;

    while (a->i != dst) {
        prev = a;
        a    = a->next;

        if (a == NULL) return NO_SUCH_EDGE;
    }

    if (prev == NULL) {
        g->adjList[src] = a->next;
    }
    else {
        prev->next = a->next;
    }
    free(a->ports);
    free(a);

    Edge* b = g->adjList[dst];

    if (b == NULL) return NO_SUCH_EDGE;

    while (b->i != src) {
        prev = b;
        b    = b->next;
    }
    if (prev == NULL) {
        g->adjList[dst] = b->next;
    }
    else {
        prev->next = b->next;
    }
    free(b->ports);
    free(b);

    return 0;
}

int changeNodeData(Graph* g, uint32_t id, const char* name, uint32_t port)
{
    if (id >= g->nodesNum) return NO_SUCH_NODE;

    Node* n = &g->nodes[id];
    n->port = port;
    memset(n->name, 0, NAMELEN);
    strncpy(n->name, name, NAMELEN - 1);

    return 0;
}

int changeEdgeData(Graph* g, uint32_t src, uint32_t dst, uint32_t* ports, uint32_t portsNum)
{
    if (src >= g->nodesNum || dst >= g->nodesNum) {
        return NO_SUCH_NODE;
    }
    Edge* a = g->adjList[src];

    if (a == NULL) return NO_SUCH_EDGE;

    while (a->i != dst) {
        a = a->next;
        if (a == NULL) return NO_SUCH_EDGE;
    }

    a->portsNum = portsNum;
    memcpy(a->ports, ports, portsNum * sizeof(uint32_t));

    Edge* b = g->adjList[dst];

    if (b == NULL) return NO_SUCH_EDGE;

    while (b->i != src) {
        b = b->next;
    }

    b->portsNum = portsNum;
    memcpy(b->ports, ports, portsNum * sizeof(uint32_t));

    return 0;
}

void printGraphAdjList(Graph* g)
{
    for (uint32_t i = 0; i < g->nodesNum; i++) {
        printf("%d ->", g->nodes[i].i);
        Edge* e = g->adjList[i];
        while (e) {
            printf(" %d", e->i);
            e = e->next;
        }
        printf(" NULL");
        printf("\n");
    }
}

void printGraphDot(Graph* g, const char* filename)
{
    FILE* file = fopen(filename, "w");

    fprintf(file, "Graph {\n");
    for (size_t i = 0; i < g->nodesNum; ++i) {
        fprintf(file, "  %u [label=\"%s\"];\n", g->nodes[i].i, g->nodes[i].name);
    }

    for (uint32_t i = 0; i < g->nodesNum; ++i) {
        Edge* current = g->adjList[i];
        while (current != NULL) {
            // Проверка, что ребро еще не было записано (для устранения повторов)
            if (current->i > i) {
                fprintf(file, "  %u -- %u [label=\"", i, current->i);
                for (size_t j = 0; j < current->portsNum; ++j) {
                    fprintf(file, "%u", current->ports[j]);
                    if (j < current->portsNum - 1) {
                        fprintf(file, ",");
                    }
                }
                fprintf(file, "\"];\n");
            }
            current = current->next;
        }
    }

    fprintf(file, "}\n");
    fclose(file);
}

int findNearestNodeWithPort(Graph* graph, size_t start_node, size_t target_port, size_t transfer_port)
{
    if (start_node >= graph->nodesNum) {
        return NO_SUCH_NODE;
    }

    // Массив для хранения посещенных узлов
    int* visited = (int*)malloc(graph->nodesNum * sizeof(int));
    for (uint32_t i = 0; i < graph->nodesNum; ++i) {
        visited[i] = 0;
    }

    // Создание очереди для BFS
    Queue* queue = createQueue(100);

    // Изначально не добавляем start_node в очередь
    visited[start_node] = 1;
    Edge* current       = graph->adjList[start_node];
    while (current != NULL) {
        uint32_t neighbor_index = current->i;
        int      port_match     = 0;
        for (uint32_t i = 0; i < current->portsNum; ++i) {
            if (current->ports[i] == transfer_port) {
                port_match = 1;
                break;
            }
        }
        if (port_match) {
            visited[neighbor_index] = 1;
            enqueue(queue, neighbor_index, 1);
        }
        current = current->next;
    }

    // Пока очередь не пуста
    while (!isEmpty(queue)) {
        QueueNode current_node  = dequeue(queue);
        uint32_t  current_index = current_node.node_index;

        // Проверка, соответствует ли целевой порт
        if (graph->nodes[current_index].port == target_port) {
            // Освобождение памяти, занятой очередью и массивом visited
            free(visited);
            free(queue);
            return current_index;
        }

        // Перебор всех соседних узлов
        current = graph->adjList[current_index];
        while (current != NULL) {
            uint32_t neighbor_index = current->i;
            // Проверка, был ли уже посещен этот сосед
            if (!visited[neighbor_index]) {
                // Проверка, доступен ли порт для передачи информации
                int port_match = 0;
                for (uint32_t i = 0; i < current->portsNum; ++i) {
                    if (current->ports[i] == transfer_port) {
                        port_match = 1;
                        break;
                    }
                }
                if (port_match) {
                    visited[neighbor_index] = 1;
                    enqueue(queue, neighbor_index, current_node.distance + 1);
                }
            }
            current = current->next;
        }
    }

    // Освобождение памяти, занятой очередью и массивом visited
    free(visited);
    freeQueue(queue);

    // Если не найден узел, соответствующий заданным условиям, возвращаем NO_SUCH_NODE
    return NO_SUCH_NODE;
}

Path shortestPath(Graph* graph, uint32_t src, uint32_t dst, uint32_t transfer_port)
{
    uint32_t dist[graph->nodesNum];
    for (uint32_t i = 0; i < graph->nodesNum; ++i) {
        dist[i] = UINT_MAX;
    }
    dist[src] = 0;

    // Релаксация рёбер
    for (uint32_t i = 0; i < graph->nodesNum - 1; ++i) {
        for (uint32_t j = 0; j < graph->nodesNum; ++j) {
            Edge* current = graph->adjList[j];
            while (current != NULL) {
                if (current->ports != NULL) {
                    for (uint32_t k = 0; k < current->portsNum; ++k) {
                        if (current->ports[k] == transfer_port && dist[j] != UINT_MAX && dist[current->i] > dist[j] + 1) {
                            dist[current->i] = dist[j] + 1;
                        }
                    }
                }
                current = current->next;
            }
        }
    }

    // Построение пути
    Path path;
    path.nodes            = (uint32_t*)malloc(graph->nodesNum * sizeof(uint32_t));
    uint32_t current_node = dst;
    path.length           = 0;
    while (current_node != src && dist[current_node] != UINT_MAX) {
        path.nodes[path.length++] = current_node;
        Edge* current             = graph->adjList[current_node];
        int   found               = 0;
        while (current != NULL) {
            if (current->ports != NULL) {
                for (uint32_t k = 0; k < current->portsNum; ++k) {
                    if (current->ports[k] == transfer_port && dist[current_node] == dist[current->i] + 1) {
                        current_node = current->i;
                        found        = 1;
                        break;
                    }
                }
            }
            if (found) break;
            current = current->next;
        }
    }
    path.nodes[path.length++] = src;

    // Переворачивание пути, чтобы он начинался с src и заканчивался dst
    for (uint32_t i = 0; i < path.length / 2; ++i) {
        uint32_t temp                   = path.nodes[i];
        path.nodes[i]                   = path.nodes[path.length - i - 1];
        path.nodes[path.length - i - 1] = temp;
    }

    return path;
}

static void dfs(Graph* graph, uint32_t node, int* visited, ConnectedComponent* component)
{
    visited[node]                       = 1;
    component->nodes[component->size++] = node;

    Edge* current = graph->adjList[node];
    while (current != NULL) {
        if (!visited[current->i]) {
            dfs(graph, current->i, visited, component);
        }
        current = current->next;
    }
}

ConnectedComponent* findConnectedComponents(Graph* graph, uint32_t* num_components)
{
    int visited[graph->nodesNum];
    for (uint32_t i = 0; i < graph->nodesNum; ++i) {
        visited[i] = 0;
    }

    *num_components                = 0;
    ConnectedComponent* components = (ConnectedComponent*)malloc(graph->nodesNum * sizeof(ConnectedComponent));

    for (uint32_t i = 0; i < graph->nodesNum; ++i) {
        if (!visited[i]) {
            components[*num_components].nodes = (uint32_t*)malloc(graph->nodesNum * sizeof(uint32_t));
            components[*num_components].size  = 0;
            dfs(graph, i, visited, &components[*num_components]);
            (*num_components)++;
        }
    }

    return components;
}
