#ifndef GRAFO_H
#define GRAFO_H

#include <stdbool.h>

typedef struct _grafo *Grafo;

// aloca e inicializa um grafo com n vértices
// o grafo pode ser orientado ou não (afeta o processamento das arestas)
// retorna um ponteiro para o grafo alocado
Grafo g_cria(int n, bool orientado);

// libera a memória usada pelo grafo
void g_destroi(Grafo self);

// retorna o número de vértices do grafo
int g_nvertices(Grafo self);

// insere uma aresta no grafo (ou altera o peso de uma aresta já existente)
void g_ins_aresta(Grafo self, int origem, int destino, float peso);

// remove uma aresta do grafo
void g_rem_aresta(Grafo self, int origem, int destino);

// inicia uma consulta a arestas do grafo.
// as próximas chamadas a g_proxima_aresta devem retornar cada aresta do grafo
void g_arestas(Grafo self);

// inicia uma consulta a arestas do grafo.
// as próximas chamadas à g_proxima_aresta devem retornar cada aresta do grafo que parte do vértice origem
void g_arestas_que_partem(Grafo self, int origem);

// retorna os dados sobre a próxima aresta de uma consulta
// retorna true se ok ou false se não tem mais arestas ou se não foi iniciada uma consulta
// cada ponteiro pode ser NULL, para o caso de não se estar interessado na informação associada
// não se deve inserir ou remover arestas no grafo com uma consulta em andamento
bool g_proxima_aresta(Grafo self, int *origem, int *destino, float *peso);

#endif // GRAFO_H
