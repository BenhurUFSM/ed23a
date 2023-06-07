## Grafos (iii)

### Ordem topológica

Uma subclasse bastante importante de grafos são os **DAGs (grafos direcionados acíclicos)**. Por exemplo, toda árvore é um DAG. Um DAG pode ser usado para modelar dependências, como em uma sequência de tarefas, em que umas dependem das outras -- pré-requisitos em disciplinas, restrições na ordem de colocação de roupas, dependências entre arquivos em uma IDE, as várias operações necessárias em uma construção e as dependências entre elas, por exemplo.

Por exemplo, considere a tarefa de se vestir. Você tem um conjunto de vestimentas (sapato, meia, calça, cueca, cinto, camisa, casaco), e algumas restrições na ordem em que essas vestimentas podem ser colocadas (meia antes do sapato, por exemplo). (Admito que é um exemplo um pouco sexista, mas foi o que apareceu). O problema é encontrar uma ordem em que as vestimentas podem ser vestidas sem quebrar nenhuma restrição e acabar como o super-homem, com as cuecas por cima das calças.

Esse problema pode ser modelado por um grafo, em que as peças de vestuário são os vértices e as dependências as arestas (se a meia deve vir antes do sapato, tem uma aresta de meia para sapato).
As arestas poderiam ser:
```
meia->sapato
calça->sapato
cueca->calça
calça->cinto
camisa->casaco
camisa->cinto
```
Existem várias ordens possíveis, como [meia, cueca, calça, camisa, cinto, sapato, casaco] ou [cueca, calça, camisa, casaco, cinto, meia, sapato].
Uma ordem como essa é chamada **ordem topológica**, e só é possível se o grafo não contiver ciclos.
Uma ordem topológica contém todos os nós de um grafo em uma ordem linear, em que não existe uma caminho no grafo que leve de um nó a outro que o antecede na ordem linear.

Dado um grafo, como encontrar uma ordem topológica?
Existem duas soluções comuns: uma baseada no grau de entrada dos vértices e outra baseada em um percurso em profundidade no grafo.

A solução baseada no grau de entrada leva em consideração que qualquer nó que tem grau de entrada igual a zero (não tem nenhuma aresta que incida nele) corresponde a uma ação que não tem nenhuma dependência, e portanto pode ser executada.
A ideia é executar qualquer uma das ações com grau de entrada 0 (ou colocá-la no final da lista de ordem topológica) e remover esse nó do grafo. Essa remoção altera o grau de entrada dos nós adjacentes ao que foi removido, possivelmente zerando seu grau de entrada. O algoritmo continua até que não restem mais nós. Caso sobrem nós em que nenhum tenha grau de entrada zero, o grafo contém ciclo e não existe uma ordem topológica. Todo DAG tem pelo menos um nó com grau de entrada 0.

O algoritmo seria:
```
   1. cria uma fila vazia
   2. encontre um nó com grau de entrada 0
   3. insira esse nó no final da fila
   4. remova esse nó do grafo (e as arestas que partem dele)
   5. repita os passos 2 a 4 até remover todos os nós do grafo
   6. a fila contém uma ordem topológica (ou o grafo contém ciclo, se sobraram nós)
```
Claro, para implementar o algoritmo não precisa realmente destruir o grafo (apesar de ser uma implementação possível: copie o grafo e destrua a cópia).
Pode-se usar estruturas auxiliares para manter o grau de entrada de todos os nós, e uma fila auxiliar com os nós que já foram identificados com grau zero mas que ainda não foram colocados na fila de saída (só colocamos na fila de saida depois de alterar os graus de entrada dos nós adjacentes).

O algoritmo está implementado na função abaixo, para um grafo implementado com matriz de adjacências
(é praticamente igual à função de detecção de ciclos vista anteriormente).
Como ele é implementado com uma fila para manter os nós que já foram detectados com grau de entrada 0 e ainda não analisados, os nós saem dessa fila na mesma ordem em que entram. Uma ordem diferente de saida dessa fila resultaria em ordens topológicas diferentes, em grafos em que mais de uma ordem é possível. Em particular, se os nós (ou as arestas) têm prioridades, a fila poderia ser uma fila de prioridade.
```c
fila ordem_topologica(int n_nos, bool grafo[n_nos][n_nos])
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
  // inicializa uma fila com todos os nós que têm GE 0
  fila f = fila_cria();
  for (int no=0; no<n_nos; no++) {
    if (ge[no] == 0) {
      fila_insere(f, no);
    }
  }
  // inicializa uma fila que conterá os nós em ordem topológica
  fila ordem = fila_cria();
  int ordenados = 0;  // conta o número de nós na fila ordem
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
    fila_insere(ordem, no);
    ordenados++;
  }
  fila_destroi(f);
  if (ordenados != n_nos) {
    // o grafo tem ciclo, não tem ordem topológica
    fila_esvazia(ordem);
  }
  return ordem;
}
```

A outra forma comum de se gerar uma ordem topológica é com um percurso em profundidade.
A idéia é que quando se retorna de um nó em um percurso em profundidade, todos os nós atingíveis a partir dele (todos os nós que dependem dele) já foram visitados -- é o contrário do que se quer.
Logo dá para gerar uma ordem topológica invertida.
Colocando os elementos encontrados em uma pilha, automaticamente se tem a inversão de ordem quando os nós forem retirados da pilha.
O código de detecção de ciclos por percurso em profundidade anterior poderia ser alterado para gerar uma ordem topológica da seguinte forma:
```c
typedef enum { nao_visitado, em_visita, ja_visitado} marca_t;

bool percorre_prof(int n_nos, bool grafo[n_nos][n_nos], 
                   marca_t marca[n_nos], int no, pilha ordem)
{
  if (marca[no] == em_visita) return false;  // detectou um ciclo!
  if (marca[no] == ja_visitado) return true;
  marca[no] = em_visita;
  for (int adj = 0; adj < n_nos; adj++) {
    if (grafo[no][adj]) {
      if (acha_ciclo(n_nos, grafo, marca, adj)) return true;
    }
  }
  pilha_insere(ordem, no); // já visitou todos os nós que dependem deste
  marca[no] = ja_visitado;
  return true;
}

pilha ordem_topologica(int n_nos, bool grafo[n_nos][n_nos])
{
  pilha ordem = pilha_cria();
  marca_t marca[n_nos];
  for (int no = 0; no < n_nos; no++) marca[no] = nao_visitado;  // todos marcados como não visitados
  for (int no = 0; no < n_nos; no++) {
    if (marca[no] == nao_visitado) {
      if (!profundidade(n_nos, grafo, marca, no, ordem)) {
        // detectou um ciclo
        pilha_esvazia(ordem);
        break;
      }
    }
  }
  return ordem; // deve ser destruída por quem recebe
}
```

O código visto em aula está em [gra2.c](gra2.c).

#### Exercícios

1. Quais as ordens topológicas possíveis para os nós (A a F) do grafo representado pelas arestas:
   ```
   A->B, B->C, C->D, A->D, E->F, F->C
   ```
2. No grafo do exercício anterior, cada aresta representa uma ação que demora tempo 1 para ser realizada; ações podem ser realizadas concorrentemente; uma ação só pode ser realizada após todas as que a precedem no grafo. Qual o tempo mínimo para realizar todas as ações representadas no grafo?

