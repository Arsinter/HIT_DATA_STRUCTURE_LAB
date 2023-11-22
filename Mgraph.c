#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int minLOWCOST(int LOWCOST[], int MSTSet[], int V) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (MSTSet[v] == 0 && LOWCOST[v] < min) {
            min = LOWCOST[v];
            min_index = v;
        }
    }

    return min_index;
}

void printMST(int parent[], int** graph, int V) {
    printf("Edge \tWeight\n");
    for (int i = 1; i < V; i++) {
        printf("%d - %d \t%d \n", parent[i], i, graph[i][parent[i]]);
    }
}

void primMST(int** graph, int V) {
    int parent[V];
    int LOWCOST[V];
    int MSTSet[V];

    for (int i = 0; i < V; i++) {
        LOWCOST[i] = INT_MAX;
        MSTSet[i] = 0;
    }
    LOWCOST[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minLOWCOST(LOWCOST, MSTSet, V);
        MSTSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && MSTSet[v] == 0 && graph[u][v] < LOWCOST[v]) {
                parent[v] = u;
                LOWCOST[v] = graph[u][v];
            }
        }
    }

    printMST(parent, graph, V);
}

// 结构体表示边
struct Edge {
    int begin, end, weight;
};

// 结构体表示图
struct Graph {
    int V, E;
    struct Edge* edge;
};

// 初始化图
void initGraph(struct Graph* graph, int V, int E) {
    graph->V = V;
    graph->E = E;

    graph->edge = (struct Edge*)malloc(E * sizeof(struct Edge));
}

// 从文件中读取图的信息
void readGraphFromFile(struct Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file.\n");
        exit(1);
    }

    int V, E;
    fscanf(file, "%d %d", &V, &E);

    initGraph(graph, V, E);

    for (int i = 0; i < E; i++) {
        fscanf(file, "%d %d %d", &graph->edge[i].begin, &graph->edge[i].end, &graph->edge[i].weight);
    }

    fclose(file);
}


// 并查集的数据结构和操作
int* parent;

int find(int i) {
    if (parent[i] == -1)
        return i;
    return find(parent[i]);
}

void unionSet(int x, int y) {
    int xset = find(x);
    int yset = find(y);
    parent[xset] = yset;
}

// Kruskal算法
void kruskal(struct Graph* graph) {
    struct Edge result[graph->V - 1];  // 保存最小生成树的结果
    int e = 0;  // 用于结果数组的索引
    int i = 0;  // 用于遍历所有边

    // 初始化所有顶点的父节点为-1，表示独立的集合
    for (int v = 0; v < graph->V; v++)
        parent[v] = -1;

    // 按权重升序排序所有边
    for (int v = 0; v < graph->E; v++) {
        for (int j = 0; j < graph->E - v - 1; j++) {
            if (graph->edge[j].weight > graph->edge[j + 1].weight) {
                struct Edge temp = graph->edge[j];
                graph->edge[j] = graph->edge[j + 1];
                graph->edge[j + 1] = temp;
            }
        }
    }

    // 选取V-1条边以保证生成树不会形成环
    while (e < graph->V - 1 && i < graph->E) {
        struct Edge next_edge = graph->edge[i++];

        int x = find(next_edge.begin);
        int y = find(next_edge.end);

        if (x != y) {
            result[e++] = next_edge;
            unionSet(x, y);
        }
    }

    // 打印生成树的边
    printf("The edge of MST is:\n");
    for (int i = 0; i < e; ++i)
        printf("%d - %d weight:%d\n", result[i].begin, result[i].end, result[i].weight);
}

int main() {
    int V, E;

    FILE* file = fopen("C:\\Users\\Fanful\\CLionProjects\\DSlab3MST\\graph.txt", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    fscanf(file, "%d %d", &V, &E);

    int** graph = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        graph[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            graph[i][j] = 0;
        }
    }

    for (int i = 0; i < E; i++) {
        int src, dest, weight;
        fscanf(file, "%d %d %d", &src, &dest, &weight);
        graph[src][dest] = weight;
        graph[dest][src] = weight;
    }

    fclose(file);

    primMST(graph, V);

    for (int i = 0; i < V; i++) {
        free(graph[i]);
    }
    free(graph);

    struct Graph graphKruskal;

    readGraphFromFile(&graphKruskal, "C:\\Users\\Fanful\\CLionProjects\\DSlab3MST\\graph.txt");

    parent = (int*)malloc(graphKruskal.V * sizeof(int));

    kruskal(&graphKruskal);

    // 释放内存
    free(parent);
    free(graphKruskal.edge);

    return 0;
}
