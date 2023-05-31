## Grafos (ii)

### Implementação de grafos

Tem duas ideias principais de se implementar grafos, com listas de adjacências e com matriz de adjacências.

Na primeira, se mantém um conjunto de listas, chamadas **listas de adjacências**, uma lista para cada nó.
Cada lista contém uma entrada para cada aresta que parte desse nó.
Cada entrada contém o nó destino dessa aresta.

Por exemplo, o grafo orientado `g3` composto pelos conjuntos `V` e `E` abaixo:
```
   V = {a, b, c, d, e}
   E = {(a,b), (a,c), (c,b), (d,b), (d,d), (d,e), (e,b)}
```
seria implementado pelas 5 listas abaixo, uma para cada nó:
```
   a -> [ b, c ]
   b -> [ ]
   c -> [ b ]
   d -> [ b, d, e ]
   e -> [ b ]
```
Os nós podem ser nomeados por números, e as listar armazenadas em um vetor indexado por esses números, ou poderia ter uma estrutura para mapear a identificação de um nó para sua posição no vetor. Pode ser também uma lista de listas em vez de um vetor.

O grafo `g4` composto pelos conjuntos `V` e `E` abaixo:
```
   V = {a, b, c, d, e}
   E = {{a,b}, {a,c}, {c,b}, {d,b}, {d,d}, {d,e}, {e,b}}
```
seria implementado pelas 5 listas abaixo, uma para cada nó:
```
   a -> [ b, c ]
   b -> [ a, c, d, e ]
   c -> [ a, b ]
   d -> [ b, d, e ]
   e -> [ b, d ]
```

Na implementação com **matriz de adjacências**, usa-se uma matriz quadrada, com uma linha e uma coluna para cada nó do grafo.
Cada elemento da matriz contém um valor que diz se existe uma aresta que vai do nó correspondente à linha desse elemento até o nó correspondente à coluna desse elemento.
Se o grafo for não orientado, a matriz será simétrica.

Os grafos `g3` e `g4` acima seriam implementados pelas matrizes abaixo:
```
g1 a b c d e
a  0 1 1 0 0
b  0 0 0 0 0
c  0 1 0 0 0
d  0 1 0 1 1
e  0 1 0 0 0

g2 a b c d e
a  0 1 1 0 0
b  1 0 1 1 1
c  1 1 0 0 0
d  0 1 0 1 1
e  0 1 0 1 0
```

No caso de grafos ponderados, se houver valores associados aos nós, em geral se implementa um vetor para conter esses valores.
Para arestas ponderadas, no caso de listas cada entrada na lista contém, além do nó destino da aresta o seu peso; no caso de matriz de adjacências, em vez de se colocar zero ou um em cada posição da matriz, coloca-se o valor do peso da aresta. Caso seja possível ter aresta com peso zero, usa-se um outro valor para representar uma aresta inexistente.

O percurso em profundidade visto anteriormente poderia ser implementado em um grafo representado por uma matriz assim:
```c
void percurso_profundidade(int n_nos, bool grafo[n_nos][n_nos])
{
  bool marcado[n_nos];
  for (int no = 0; no < n_nos; no++) marcado[no] = false;  // todos desmarcados
  for (int no = 0; no < n_nos; no++) {
    if (!marcado[no]) {
      percorre_profundidade(n_nos, grafo, marcado, no);
    }
  }
}

void percorre_profundidade(int n_nos, bool grafo[n_nos][n_nos], 
                           bool marcado[n_nos], int no)
{
  if (!marcado[no]) {
    marcado[no] = true;
    visita(no);
    for (int adj = 0; adj < n_nos; adj++) {
      if (grafo[no][adj]) {
        percorre_profundidade(n_nos, grafo, marcado, adj);
      }
    }
  }
}
```
Uma possível implementação do percurso em largura em um grafo implementado como um vetor de listas está abaixo.
```c
void percurso_largura(int n_nos, lista grafo[n_nos])
{
  bool marcado[n_nos];
  for (int no = 0; no < n_nos; no++) marcado[no] = false;  // todos desmarcados
  fila f;
  f = fila_cria();
  for (int no = 0; no < n_nos; no++) {
    insere(f, no);
    while (!fila_vazia(f)) {
      int m = fila_remove(f);
      if (marcado[m]) continue;
      marcado[m] = true;
      visita(m);
      int adj;
      for (lista_inicia_percurso(grafo[m]); lista_proximo_dado(grafo[m], &adj); ) { // lista_proximo_dado retorna false se terminar
        fila_insere(f, adj);
      }
    }
  }
  fila_destroi(f);
}
```

#### Detecção de ciclos

A deteção de ciclos em um grafo não direcionado é mais simples que a detecção em um grafo direcionado.
Em um grafo não direcionado, pode-se fazer um percurso no grafo, e se esse percurso levar a um nó que já foi visitado,
é sinal que o grafo tem um ciclo, porque se existem mais de um caminho para se chegar a um nó do grafo quer dizer que existe um ciclo envolvendo esses dois caminhos.

No caso de um grafo direcionado, o fato de se ter mais de um caminho para atingir um nó não implica na existência de um ciclo, porque pode não haver um caminho de volta, necessário para a existência do ciclo.

Para detectar se um grafo não direcionado é acíclico, o algoritmo é simples: percorre-se o grafo, e se durante o percurso chega-se a um nó já marcado, é porque o grafo contém um ciclo e não é acíclico.

O percurso em profundidade visto anteriormente poderia ser alterado para detectar ciclos assim:
```c
bool aciclico(int n_nos, bool grafo[n_nos][n_nos])
{
  bool marcado[n_nos];
  for (int no = 0; no < n_nos; no++) marcado[no] = false;  // todos desmarcados
  for (int no = 0; no < n_nos; no++) {
    if (marcado[no]) continue;
    if (acha_ciclo(n_nos, grafo, marcado, no)) return false;
  }
  return true;
}

bool acha_ciclo(int n_nos, bool grafo[n_nos][n_nos], 
                bool marcado[n_nos], int no)
{
  if (marcado[no]) return true;
  marcado[no] = true;
  for (int adj = 0; adj < n_nos; adj++) {
    if (grafo[no][adj]) {
      if (acha_ciclo(n_nos, grafo, marcado, adj)) return true;
    }
  }
  return false;
}
```

Para o caso de grafos direcionados, a detecção é um pouco mais complexa. O ciclo é detectado se durante o percurso se chega a um nó que faz parte do percurso atual, e não a qualquer nó que já tenha sido visitado. Precisamos distinguir entre 3 estados de cada nó: ainda não visitado, em visita (ainda não se terminou de visitar os seus adjacentes) e já visitados (após o percurso de todos seus adjacentes). Só se detecta um ciclo quando se chega em um nó que está "em visita", porque quer dizer que se chegou de novo a esse nó durante a visita a seus adjacentes. Quando se chega a um nó já visitado, quer dizer simplesmente que tem mais de um caminho para se chegar ao mesmo nó, não que haja um ciclo.
O código pode ser então:
```c
typedef enum { nao_visitado, em_visita, ja_visitado} marca_t;
bool aciclico(int n_nos, bool grafo[n_nos][n_nos])
{
  marca_t marca[n_nos];
  for (int no = 0; no < n_nos; no++) marca[no] = nao_visitado;  // todos marcados como não visitados
  for (int no = 0; no < n_nos; no++) {
    if (marca[no] == nao_visitado) {
      if (acha_ciclo(n_nos, grafo, marca, no)) return false;
    }
  }
  return true;
}

bool acha_ciclo(int n_nos, bool grafo[n_nos][n_nos], 
                marca_t marca[n_nos], int no)
{
  if (marca[no] == em_visita) return true;
  if (marca[no] == ja_visitado) return false;
  marca[no] = em_visita;
  for (int adj = 0; adj < n_nos; adj++) {
    if (grafo[no][adj]) {
      if (acha_ciclo(n_nos, grafo, marca, adj)) return true;
    }
  }
  marca[no] = ja_visitado;
  return false;
}
```
As marcas são também chamadas de coloração (branco, cinza e preto).
Esse mesmo código pode ser usado em grafos não direcionados.

Outra forma de detecção de ciclos é baseada na idéia de que um nó que pertence a um ciclo obrigatoriamente tem pelo menos uma aresta de chegada e uma de saída (o grau de entrada e o grau de saída de um nó pertencente a um ciclo não podem ser 0).
A idéia é remover do grafo todo nó que tenha grau de entrada (ou saída) 0. Após remover um desses nós, as arestas que partem (ou chegam) dele são removidas, diminuindo o grau de entrada (ou saída) de outros nós. Se não se conseguir remover todos os nós, é porque o grafo tem um ciclo.

Abaixo tem uma implementação dessa idéia, com grafo implementado por matriz de adjacências, considerando o grau de entrada dos nós. A implementação considerando grau de saída seria muito semelhante.
Em vez de destruir o grafo retirando nós, essa implementação mantém um vetor com o grau de entrada de cada nó, que vão sendo alterados conforme cada nó com grau de entrada zero é analisado. Os nós que já foram identificados com grau de entrada 0 e ainda não foram analisados são mantidos em uma fila.
```c
bool aciclico_por_grau_de_entrada(int n_nos, bool grafo[n_nos][n_nos])
{
  // inicializa um vetor com o grau de entrada dos nós
  int ge[n_nos];
  for (int no=0; no<n_nos; no++) {
    ge[no] = 0;
    // incrementa o GE do nó para cada aresta que chega nele
    for (int no2=0; no2<n_nos; no2++) {
      if (grafo[no2][no]) ge[no]++;
    }
  }
  // inicializa uma fila com todos os nós que têm GE 0 (isso poderia ser feito no final do segundo for acima)
  fila f = fila_cria();
  for (int no=0; no<n_nos; no++) {
    if (ge[no] == 0) {
      fila_insere(f, no);
    }
  }
  int analisados = 0;  // conta o número de nós analisados (que têm GE 0)
  while (!fila_vazia(f)) {
    int no = fila_remove(f);
    // decrementa o GE de cada nó destino de uma aresta que parte do nó analisado
    // insere na fila se o GE ficou 0
    for (int no2=0; no2<n_nos; no2++) {
      if (grafo[no][no2]) {
        ge[no2]--;
        if (ge[no2] == 0) fila_insere(f, no2);
      }
    }
    analisados++;
  }
  fila_destroi(f);
  // se todos os nós foram analisados, o grafo é acíclico.
  return analisados == n_nos;
}
```
