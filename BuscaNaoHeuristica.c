#include <stdio.h>
#include <stdlib.h>

typedef struct grafo Grafo;

struct grafo{
    int ponderado, vertices, grau_max;
    int* grau;
    int** arestas;
    float** pesos;
};

Grafo *graph;

Grafo *criaGrafo(int vertices, int grau_max, int ponderado){
    Grafo *graph = (Grafo*) malloc(sizeof(struct grafo));
    int i;
    if(graph != NULL){
        graph->vertices = vertices;
        graph->grau_max = grau_max;
        graph->ponderado = (ponderado != 0)?1:0;
        graph->grau = (int*)calloc( vertices, sizeof(int));
        for(i = 0; i < vertices; i++)
            graph->arestas[i] = (int*)malloc(grau_max*sizeof(int));
        if(graph->ponderado){
            graph->pesos = (float**)malloc(vertices*sizeof(float*));
            for(i = 0; i < vertices; i++)
                graph->pesos[i] = (float*)malloc(grau_max*sizeof(float));
        }
    }
    return graph;
}

void liberaGrafo(Grafo* graph){
    int i;
    if(graph != NULL){
        for(i = 0; i < graph->vertices; i++)
            free(graph->arestas[i]);
        free(graph->arestas);
        if (graph->ponderado){
            for(i = 0; i<graph->vertices; i++)
                free(graph->pesos[i]);
            free(graph->pesos);
        }
        free(graph->grau);
        free(graph);
    }
}

int insereAresta(Grafo* graph, int origem, int destino, int orientado, float peso){
    if(graph == NULL)
        return 0;
    if(origem < 0 || origem >= graph->vertices)
        return 0;
    if(destino < 0 || destino >= graph->vertices)
        return 0;

    graph->arestas[origem][graph->grau[origem]] = destino;
    if(graph->ponderado)
        graph->pesos[origem][graph->grau[origem]] = peso;
    graph->grau[origem]++;

    if(orientado == 0)
        insereAresta(graph, destino, origem, 1, peso);
    return 1;
}

int removeAresta(Grafo* graph, int origem, int destino,int orientado){
    if(graph == NULL)
        return 0;
    if(origem < 0 || origem >= graph->vertices)
        return 0;
    if(destino < 0 || destino >= graph->vertices)
        return 0;
    int i = 0;
    while(i < graph->grau[origem] && graph->arestas[origem][i] != destino)
        i++;
    if(i == graph->grau[origem])
        return 0;
    graph->grau[origem]--;
    graph->arestas[origem][i] = graph->arestas[origem][graph->grau[origem]];
    if(graph->ponderado)
        graph->pesos[origem][i] = graph->pesos[origem][graph->grau[origem]];
    if(orientado == 0)
        removeAresta(graph, destino, origem, 1);
    return 1;
}

void buscaProfundidade(Grafo *graph, int inicial, int *visitado, int cont){
    int i;
    visitado[inicial] = cont;
    for(i = 0; i < graph->grau[inicial]; i++)
        if(!visitado[graph->arestas[inicial][i]])
            buscaProfundidade(graph, graph->arestas[inicial][i], visitado, cont+1);
}

//Resolver quebra-cabeças (Ex.: Labirinto)
void buscaProfundidadeGrafo(Grafo *graph, int inicial, int *visitado){
    int i, cont = 1;
    for(i = 0; i < graph->vertices; i++)
        visitado[i] = 0;
    buscaProfundidade(graph, inicial, visitado, cont);
}

void buscaLarguraGrafo(Grafo *graph, int inicial, int *visitado){
    int i, vert, NV, cont = 1, *fila, IF = 0, FF = 0;
    for(i = 0; i < graph->vertices; i++)
        visitado[i] = 0;
    NV = graph->vertices;
    fila = (int*) malloc(NV * sizeof(int));
    FF++;
    fila[FF] = inicial;
    visitado[inicial] = cont;
    while(IF != FF){
        IF = (IF + 1) % NV;
        vert = fila[IF];
        cont++;
        for(i = 0; i < graph->grau[vert]; i++){
            if(!visitado[graph->arestas[vert][i]]){
                FF = (FF + 1) % NV;
                fila[FF] = graph->arestas[vert][i];
                visitado[graph->arestas[vert][i]] = cont;
            }
        }
    }
    free(fila);
}

int main() {
    
    int orientado = 1;
    Grafo* graph = criaGrafo(5, 5, 0);
    insereAresta(graph, 0, 1, orientado, 0);
    insereAresta(graph, 1, 3, orientado, 0);
    insereAresta(graph, 1, 2, orientado, 0);
    insereAresta(graph, 2, 4, orientado, 0);
    insereAresta(graph, 3, 0, orientado, 0);
    insereAresta(graph, 3, 4, orientado, 0);
    insereAresta(graph, 4, 1, orientado, 0);
    int visitado[5];

    /* Exemplo Busca Em Profundidade
    buscaProfundidadeGrafo(graph, 0, visitado);
    */
    /* Exemplo Busca Em Largura */
    buscaLarguraGrafo(graph, 0, visitado);

    for(int i = 0;  i < 5; i++)
        printf("%d\n", visitado[i]);

    liberaGrafo(graph);

    return 0;
}