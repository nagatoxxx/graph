#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define NAMELEN 64

// коды ошибок
#define NO_SUCH_NODE -1
#define NO_SUCH_EDGE -2

// узел графа
typedef struct
{
    uint32_t i; // уникальный идентификатор узла, для более
                // эффективного обращения к ребрам и
                // спискам связности
    char     name[NAMELEN];
    uint32_t port;
} Node;

typedef struct AdjNode_t
{
    uint32_t          i;    // индекс вершины
    struct AdjNode_t* next; // односвязный список

    uint32_t* ports; // указатель на массив с портами для данного узла
    uint32_t  portsNum;
} Edge;

typedef struct
{
    Node*    nodes;
    uint32_t nodesNum;
    uint32_t nodesCapacity;
    Edge**   adjList; // i-тый элемент adjList соответствует списку связности
                      // для узла с идентификатором i
} Graph;

// путь в графе
typedef struct
{
    uint32_t* nodes;  // Узлы пути
    uint32_t  length; // Длина пути
} Path;

// компонента связности
typedef struct
{
    uint32_t* nodes; // Узлы связной компоненты
    uint32_t  size;  // Размер связной компоненты
} ConnectedComponent;

// создает граф, возвращает указатель на созданный граф
Graph* createGraph();

// очищает память, выделенную под граф
void freeGraph(Graph* g);

// добавляет новый узел с заданной информацией в граф
// возвращает уникальный id этого узла
int addNode(Graph* g, const char* name, uint32_t port, uint32_t id);

// создание нового ребра между src и dst
int addEdge(Graph* g, uint32_t src, uint32_t dst, uint32_t* ports, uint32_t portsNum);

// удаление узла графа
int removeNode(Graph* g, uint32_t id);

// удаление ребра из src в dst
int removeEdge(Graph* g, uint32_t src, uint32_t dst);

// изменение данных в соответсвующем узле
int changeNodeData(Graph* g, uint32_t id, const char* name, uint32_t port);

// изменение данных ребра
int changeEdgeData(Graph* g, uint32_t src, uint32_t dst, uint32_t* ports, uint32_t portsNum);

// поиск ближайшего узла с учетом портов
int findNearestNodeWithPort(Graph* graph, size_t start_node, size_t target_port, size_t transfer_port);

// находит кратчайший путь между двумя компьютерами с учетом ограничений
Path shortestPath(Graph* graph, uint32_t src, uint32_t dst, uint32_t transfer_port);

// выводит граф в виде списков смежности
void printGraphAdjList(Graph* g);

// выводит граф с помощью graphviz
void printGraphDot(Graph* g, const char* filename);

// находит компоненты связности в графе
ConnectedComponent* findConnectedComponents(Graph* graph, uint32_t* num_components);

#endif
