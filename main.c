#include <stdio.h>
#include <string.h>

#include "lib/graph.h"

#define stdinFlush()                                  \
    {                                                 \
        int c;                                        \
        while ((c = getchar()) != '\n' && c != EOF) { \
        }                                             \
    }

int main(void)
{
    Graph*   graph = createGraph();
    uint32_t id    = 0;

    while (1) {
        int choose = 0;

        printf("Choose an option:\n"
               "1. Add node\n"
               "2. Add edge\n"
               "3. Delete node\n"
               "4. Delete edge\n"
               "5. Change node data\n"
               "6. Change edge data\n"
               "7. Print graph to stdout\n"
               "8. Print graph to file\n"
               "9. Traversal\n"
               "10. Find path\n"
               "11. Special operation\n"
               "12. Exit\n"
               "> ");

        if (scanf("%d", &choose) != 1) {
            printf("Invalid input\n");
            stdinFlush();
            continue;
        }

        switch (choose) {
            case 1:
            {
                uint32_t port = 0;
                printf("Enter port: ");
                if (scanf("%u", &port) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter name: ");
                stdinFlush();
                char name[NAMELEN];
                fgets(name, NAMELEN, stdin);
                size_t len = strlen(name);
                if (len > 0 && name[len - 1] == '\n') {
                    name[len - 1] = '\0';
                }

                (void)addNode(graph, name, port, id++);
                break;
            }
            case 2:
            {
                uint32_t  src = 0, dst = 0;
                uint32_t* ports    = NULL;
                uint32_t  portsNum = 0;
                printf("Enter source: ");
                if (scanf("%u", &src) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter destination: ");
                if (scanf("%u", &dst) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter number of ports: ");
                if (scanf("%u", &portsNum) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                ports = (uint32_t*)malloc(portsNum * sizeof(uint32_t));
                printf("Enter ports: ");
                for (uint32_t i = 0; i < portsNum; i++) {
                    if (scanf("%u", &ports[i]) != 1) {
                        printf("Invalid input\n");
                        break;
                    }
                }
                int res = addEdge(graph, src, dst, ports, portsNum);
                free(ports);
                if (res == NO_SUCH_NODE) {
                    printf("Error: No such node\n");
                    break;
                }
                break;
            }
            case 3:
            {
                uint32_t i = 0;
                printf("Enter node index: ");
                if (scanf("%u", &i) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                id--;
                int res = removeNode(graph, i);

                if (res == NO_SUCH_NODE) {
                    printf("Error: No such node\n");
                    break;
                }

                break;
            }
            case 4:
            {
                uint32_t src = 0, dst = 0;
                printf("Enter source: ");
                if (scanf("%u", &src) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter destination: ");
                if (scanf("%u", &dst) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                int res = removeEdge(graph, src, dst);

                if (res == NO_SUCH_NODE) {
                    printf("Error: No such node\n");
                    break;
                }
                if (res == NO_SUCH_EDGE) {
                    printf("Error: No such edge\n");
                    break;
                }

                break;
            }
            case 5:
            {
                stdinFlush();
                uint32_t i    = 0;
                uint32_t port = 0;
                char     name[NAMELEN];

                printf("Enter node index: ");
                if (scanf("%u", &i) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                printf("Enter new port: ");
                if (scanf("%u", &port) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                stdinFlush();
                printf("Enter new name: ");
                fgets(name, NAMELEN, stdin);
                size_t len = strlen(name);
                if (len > 0 && name[len - 1] == '\n') {
                    name[len - 1] = '\0';
                }

                int res = changeNodeData(graph, i, name, port);

                if (res == NO_SUCH_NODE) {
                    printf("Error: No such node\n");
                    break;
                }

                break;
            }
            case 6:
            {
                uint32_t  src = 0, dst = 0;
                uint32_t* ports    = NULL;
                uint32_t  portsNum = 0;

                printf("Enter source: ");
                if (scanf("%u", &src) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter destination: ");
                if (scanf("%u", &dst) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter number of ports: ");
                if (scanf("%u", &portsNum) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                ports = (uint32_t*)malloc(portsNum * sizeof(uint32_t));
                printf("Enter ports: ");
                for (uint32_t i = 0; i < portsNum; ++i) {
                    if (scanf("%u", &ports[i]) != 1) {
                        printf("Invalid input\n");
                    }
                }

                int res = changeEdgeData(graph, src, dst, ports, portsNum);

                free(ports);

                if (res == NO_SUCH_NODE) {
                    printf("Error: No such node\n");
                    break;
                }
                if (res == NO_SUCH_EDGE) {
                    printf("Error: No such edge\n");
                    break;
                }

                break;
            }
            case 7:
            {
                printGraphAdjList(graph);
                break;
            }
            case 8:
            {
                printGraphDot(graph, "file.dot");
                system("dot -Tpng file.dot -o graph.png");
                break;
            }
            case 9:
            {
                uint32_t start = 0;
                printf("Enter start node: ");
                if (scanf("%u", &start) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                uint32_t port = 0, transfer = 0;
                printf("Enter port: ");
                if (scanf("%u", &port) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter transfer port: ");
                if (scanf("%u", &transfer) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                int res = findNearestNodeWithPort(graph, start, port, transfer);

                if (res == NO_SUCH_NODE) {
                    printf("Error: No such node\n");
                    break;
                }
                else
                    printf("Node with port %d: %d\n", port, res);


                break;
            }
            case 10:
            {
                uint32_t src = 0, dst = 0, transfer = 0;
                printf("Enter source: ");
                if (scanf("%u", &src) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter destination: ");
                if (scanf("%u", &dst) != 1) {
                    printf("Invalid input\n");
                    break;
                }
                printf("Enter transfer port: ");
                if (scanf("%u", &transfer) != 1) {
                    printf("Invalid input\n");
                    break;
                }

                Path p = shortestPath(graph, src, dst, transfer);

                if (p.length == 1) {
                    printf("Error: No path\n");
                }
                else {
                    for (uint32_t i = 0; i < p.length; i++) {
                        printf("Node %u\n", p.nodes[i]);
                    }
                }

                free(p.nodes);
                break;
            }
            case 11:
            {
                uint32_t            components = 0;
                ConnectedComponent* c          = findConnectedComponents(graph, &components);

                for (uint32_t i = 0; i < components; i++) {
                    printf("Component %u:\n", i);
                    for (uint32_t j = 0; j < c[i].size; j++) {
                        printf("Node %u\n", c[i].nodes[j]);
                    }
                }

                for (uint32_t i = 0; i < components; i++) {
                    free(c[i].nodes);
                }
                free(c);

                break;
            }
            case 12:
            {
                goto exit;
            }
            default:
            {
                printf("Invalid input\n");
                break;
            }
        }
    }

exit:
    freeGraph(graph);

    return 0;
}
