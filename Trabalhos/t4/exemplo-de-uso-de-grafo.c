#include "grafo.h"
#include <stdio.h>

int main()
{
  Grafo g;
  int o, d;
  float p;
  g = g_cria(50, false);
  g_ins_aresta(g, 1, 3, 24.5);
  g_ins_aresta(g, 3, 7, 12);
  g_arestas(g);
  printf("Todas as arestas:\n");
  // deve imprimir (talvez em outra ordem):
  // 1 -> 3 (24.50)
  // 3 -> 1 (24.50)
  // 3 -> 7 (12.00)
  // 7 -> 3 (12.00)
  while (g_proxima_aresta(g, &o, &d, &p)) {
    printf("%d -> %d (%.2f)\n", o, d, p);
  }

  printf("Arestas que partem do nó 3:\n");
  g_arestas_que_partem(g, 3);
  // deve imprimir (talvez em outra ordem):
  // 3 -> 1 (24.50)
  // 3 -> 7 (12.00)
  while (g_proxima_aresta(g, &o, &d, &p)) {
    printf("%d -> %d (%.2f)\n", o, d, p);
  }

  g_rem_aresta(g, 7, 3);
  printf("Arestas que partem do nó 3 depois da remoção:\n");
  g_arestas_que_partem(g, 3);
  // não deve imprimir nada
  // ERRATA deve imprimir a 3->1
  while (g_proxima_aresta(g, &o, &d, &p)) {
    printf("%d -> %d (%.2f)\n", o, d, p);
  }
  g_destroi(g);
}

// se o grafo for criado com "true" em vez de "false", deve imprimir
// 1 -> 3 (24.50)
// 3 -> 7 (12.00)
// -------
// 3 -> 7 (12.00)
// -------
// 3 -> 7 (12.00)
