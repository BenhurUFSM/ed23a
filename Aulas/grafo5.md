## Grafos (v)

### Caminho mais curto

Em um grafo ponderado, o comprimento de um caminho é a soma dos pesos das arestas que constituem esse caminho. O caminho mais curto entre dois nós do grafo é o caminho que tem o menor comprimento, entre todos os caminhos que unem esses nós.

O **algoritmo de Dijkstra** é um algoritmo para encontrar os caminhos mais curtos partindo de um dos nós do grafo até cada um dos nós atingíveis (produz uma árvore de caminhos mais curtos). A ideia do algoritmo é chutar o caminho mais curto para cada nó (em infinito), e tentar ir melhorando esse valor conforme vai percorrendo o grafo. A cada nó em que chega, verifica se a partir desse nó consegue-se diminuir o custo para chegar a algum dos nós vizinhos. O nó escolhido a cada vez é aquele que tem o menor custo conhecido entre os nós não visitados.

O algoritmo de Dijkstra então é algo como:
```c
  dijstra(grafo g, no inicial)
  {
    for (n = cada nó em g) {
      visitado[n] = falso
      distancia[n] = INFINITO;
      pai[n] = INDEFINIDO;
    }
    distancia[inicial] = 0;
    while (tem nó não visitado) {
      n = nó não visitado com menor distancia;
      visitado[n] = true;
      for (a = cada aresta que parte de n) {
        d = destino(a);
        n_dist = distancia[n] + peso(a);
        if (n_dist < distancia[d]) {
          distancia[d] = n_dist;
          pai[d] = n;
        }
      }
    }

    return distancia, pai;
  }
```

Para melhorar o desempenho da busca do nó não visitado com menor distância, em geral usa-se uma fila de prioridade.

Você pode ver uma animação deste algoritmo (e vários outros) na [página do prof. David Galles](https://www.cs.usfca.edu/~galles/visualization/Dijkstra.html).

O algoritmo de Dijkstra não funciona em grafos que tem pesos negativos, e calcula os caminhos mínimos com uma única origem.

O algoritmo de **Floyd-Warshall** calcula o caminho mais curto entre quaisquer pares de nós, e aceita pesos negativos (mas não ciclos com peso negativo).
Esse algoritmo produz como resultado uma matriz (*D*) contendo o peso do menor caminho (a menor distância) entre quaisquer dois nós.
Opcionalmente, pode também produzir uma outra matriz (*P*), contendo na posição *i,j* o nó que deve ser visitado antes de *j* no menor caminho de *i* a *j* (o predecessor de j), permitindo reconstruir todos os menores caminhos (se em *P\[i,j\]* tem *k*, olha em *P\[i,k\]* para o anterior etc até chegar em *i*).

O algoritmo inicializa a matriz de distâncias mínimas com o peso da aresta que liga *i* a *j* na posição *D\[i,j\]*. As demais posições são preenchidas com infinito, exceto a diagonal principal, que é preenchida com 0. Essa matriz contém, então, o custo para chegar de um nó a outro, percorrendo no máximo uma aresta. O algoritmo escolhe um nó *k*, e verifica se é possível reduzir algum dos custos passando por esse nó. Em *D\[i,j\]* tem o custo do menor caminho até agora para ir de *i* para *j*; em *D\[i,k\]* o menor custo para ir de *i* até *k* e em *D\[k,j\]* o menor custo conhecido até agora para ir de *k* até *j*. Se *D\[i,k\] + D\[k,j\]* for menor que *D\[i,j\]*, o caminho passando por *k* é mais curto que o mais curto conhecido até agora, então substitui esse novo valor em *D\[i,j\]*. Faz isso para *k* sendo cada nó do grafo, em todas as posições *i,j* da matriz.

Para a construção da matriz *P*, o algoritmo inicializa ela com o valor do nó *i* na posição *P\[i,j\]* para cada aresta que ligue *i* a *j*, e com um valor nulo nas demais posições. Cada vez que a posição *D\[i,j\]* é alterada (porque *D\[i,k\] + D\[k,j\]* < *D\[i,j\]*), a posição *P\[i,j\]* é alterada para o valor em *P\[k,j\]* (porque, no caminho de i a j, tem que passar por k antes de ir para j).

Por exemplo, considere o grafo orientado contendo *V={a,b,c}* e *E={(a,a,2), (a,b,8), (b,a,3), (a,c,5), (c,b,2)}* (as arestas são (origem,destino,peso)). Abaixo está a sequência de valores das matrizes no início do algoritmo, e após considerar *k* como *a*, *b* e *c*.

|    D0  |  a  |  b  |  c  |     |   P0  |  a  |  b  |  c  |
|   ---  | --- | --- | --- | --- |  ---  | --- | --- | --- |
| **a**  |  0  |  8  |  5  |     | **a** |  a  |  a  |  a  | 
| **b**  |  3  |  0  |  -  |     | **b** |  b  |  -  |  -  |
| **c**  |  -  |  2  |  0  |     | **c** |  -  |  c  |  -  |

|    Da  |  a  |  b  |  c  |     |   Pa  |  a  |  b  |  c  |
|   ---  | --- | --- | --- | --- |  ---  | --- | --- | --- |
| **a**  |  0  |  8  |  5  |     | **a** |  a  |  a  |  a  | 
| **b**  |  3  |  0  |**8**|     | **b** |  b  |  -  |**a**|
| **c**  |  -  |  2  |  0  |     | **c** |  -  |  c  |  -  |

|    Db  |  a  |  b  |  c  |     |   Pb  |  a  |  b  |  c  |
|   ---  | --- | --- | --- | --- |  ---  | --- | --- | --- |
| **a**  |  0  |  8  |  5  |     | **a** |  a  |  a  |  a  | 
| **b**  |  3  |  0  |  8  |     | **b** |  b  |  -  |  a  |
| **c**  |**5**|  2  |  0  |     | **c** |**b**|  c  |  -  |

|    Dc  |  a  |  b  |  c  |     |   Pc  |  a  |  b  |  c  |
|   ---  | --- | --- | --- | --- |  ---  | --- | --- | --- |
| **a**  |  0  |**7**|  5  |     | **a** |  a  |**c**|  a  | 
| **b**  |  3  |  0  |  8  |     | **b** |  b  |  -  |  a  |
| **c**  |  5  |  2  |  0  |     | **c** |  b  |  c  |  -  |

O algoritmo pode ser escrito assim:
```c
floyd_warshall(grafo G)
{
   n = num_nos(G)
   matriz D[n][n] = { infinitos }
   matriz P[n][n] = { nulos }
   para cada aresta a em G:
      D[origem(a)][destino(a)] = peso(a);
      P[origem(a)][destino(a)] = origem(a);
   for (i=0; i<n; i++):
      D[i][i] = 0;
   for (k=0; k<n; k++):
      for (i=0; i<n; i++):
         for (j=0; j<n; j++):
            nova_dist = D[i][k] + D[k][j]
            if (nova_dist < D[i][j]):
               D[i][j] = nova_dist
               P[i][j] = P[k][j]
   return D, P
}
```


### Exercício

1. Aplique o algoritmo de Floyd-Warshall ao grafo orientado com arestas *ab0,bc4,cd1,da6,db5*. Compare as distâncias obtidas com o algoritmo de Dijkstra, partindo de cada nó.
