## Grafos (iv)

### Árvore geradora

Um grafo não orientado, acíclico, conexo é uma árvore. Uma árvore com N nós possui N-1 arestas. Se se adiciona uma nova aresta, será gerado um ciclo. Se se remove uma aresta, o grafo deixa de ser conexo.

Qualquer subgrafo de um grafo não orientado que seja uma árvore é chamado de sub-árvore desse grafo. Qualquer sub-árvore de um grafo que contenha todos os nós do grafo é chamada de **árvore geradora** do grafo (*spanning tree* em inglês). Todo grafo não orientado conexo tem pelo menos uma árvore geradora.

Um percurso em largura ou em profundidade pode ser usado para se encontrar uma árvore geradora de um grafo: toda aresta que leva a um nó não visitado faz parte da árvore geradora.

Por exemplo, considere o grafo com os nós `a,b,c,d,e` e as arestas `ac,bc,cd,ce,de`.
Esse grafo tem 3 árvores geradoras: `ac,bc,cd,ce`, `ac,bc,cd,de` e `ac,bc,ce,de`.

Uma árvore geradora pode ser representada por um vetor de pais: um vetor com um elemento para cada nó, contendo a identificação do nó que é pai dele na árvore.
É comum usar essa representação quando se constrói a árvore geradora com um algoritmo de percurso, o pai de um nó sendo aquele de onde se chega ao nó pela primeira vez.
Existem vários vetores de pais possíveis para representar a mesma árvore geradora (c cada um dos nós como raiz).
Por exemplo, a última árvore geradora acima (`ac,bc,ce,de`) poderia ser representada por `-caec`, `c-bec`, `cc-ec`, `cce-d` ou `ccee-` (`-` é colocado como pai do nó raiz).

Em um grafo ponderado, com valores associados a suas arestas, o custo de uma árvore geradora é a soma dos valores de suas arestas.
Uma árvore geradora de um grafo é chamada de **árvore geradora de custo mínimo** ou árvore geradora mínima se nenhuma outra árvore geradora desse grafo tiver um custo menor.

Ainda no exemplo acima, se as arestas tivessem os pesos `ac4,bc7,cd3,ce2,de1`, as árvores geradoras teriam custo `ac,bc,cd,ce 16`, `ac,bc,cd,de 15`, `ac,bc,ce,de 14`, sendo essa última a árvore geradora mínima. Se a aresta `ce` tivesse peso 3, essa última árvore teria peso 15 como a penúltima, e ambas seriam árvores geradoras mínimas.

Como extrair uma árvore geradora mínima de um grafo?
Imagine que separamos nosso grafo em duas partições: alguns dos nós para um lado e os demais para o outro. Algumas das arestas do grafo ligam esses dois lados (ligam um nó que está de um lado com outro que está do outro). Como a árvore geradora é conexa, pelo menos uma dessas arestas faz parte de qualquer árvore geradora. A aresta de menor custo entre essas certamente faz parte da árvore geradora mínima. Essa é a ideia básica dos dois algoritmos mais conhecidos, o de *Prim* e o de *Kruskal*.

#### Algoritmo de Prim

Este algoritmo funciona fazendo crescer uma árvore, desde um único nó até que ela tenha todos os nós do grafo. A cada passo do algoritmo é acrescentada uma das arestas do grafo, que faz a árvore crescer um nó.
O algoritmo começa em um nó qualquer da árvore, e a cada passo, escolhe, entre as arestas que partem de um dos nós que já estão na árvore e chegam em algum dos nós que ainda não estão nela, aquele de menos peso (caso exista mais de um, qualquer deles serve). Essa aresta e o nó que ela conecta são acrescentados na árvore.
Esse processo se repete até que a árvore contenha todos os nós do grafo.

Uma forma de se implementar esse algoritmo é com o uso de três vetores com uma entrada para cada nó: um vetor de visitado, que marca quais nós já foram visitados (e já pertencem à árvore), um vetor de custos, que contém o custo da aresta mais barata já vista, que chega de algum nó já na árvore até esse nó, e o vetor de predecessores, que contém o nó de onde parte essa aresta de menor custo. Esse vetor de predecessores, no final do algoritmo, será o vetor de pais que representa a árvore de custo mínimo (e é o resultado do algoritmo).

```c
  vetor prim(grafo g, no r) {
    custo[*] = INFINITO;
    visitado[*] = false;
    custo[r] = 0;
    pred[r] = NENHUM;
    while (tem nó não visitado) {
      n = acha o nó não visitado de menor custo
      visitado[n] = true;
      // n agora faz parte da árvore
      // verifica se tem custo menor para algum nó fora da árvore à partir de n
      for (a = cada aresta que parte de n) {
        d = destino(a)  // a aresta considerada vai de n para d
        if (!visitado[d]) { // d não está na árvore
          if (peso(a) < custo[d]) {  // "a" é mais barata que a anterior
            custo[d] = peso(a); // substitui o custo para chegar em d
            pred[d] = n; // a forma mais barata encontrada até agora para chegar em d é por n
          }
        }
      }
    }
    return pred;
  }
```

Para acelerar a operação de encontrar o nó não visitado de menor custo, geralmente se usa uma "fila de prioridades", que funciona como uma fila, em que cada elemento tem uma prioridade, e sempre que se remove um elemento da fila, se obtém o de maior prioridade entre os que estão na fila. Para o algoritmo de Prim, a maior prioridade é dada pelo menor custo.

#### Algoritmo de Kruskal

No algoritmo de Kruskal a árvore de custo mínimo é construída juntando-se duas sub-árvores por vez, até que só tenha uma árvore. Inicia-se com N árvores, cada uma com um nó, que são os nós do grafo. A cada passo, escolhe-se, entre as arestas que ligam árvores diferentes, aquela de menor custo. Isso faz com que as duas árvores ligadas por essa aresta virem uma sub-árvore só. Depois de selecionar N-1 arestas, restará só uma árvore.

Uma forma de implementar esse algoritmo é ordenando todas as arestas em ordem crescente de custo. Então inicia-se com o grafo constituído somente pelos nós, e  analisa-se as arestas nessa ordem. Para cada aresta, se ela conecta dois nós que pertencem a sub-árvores diferentes ela é adicionada ao grafo, caso contrário ela é descartada.

Uma forma simples de se decidir se dois nós pertencem à mesma árvore é escolhendo o "nó chefe" de uma árvore; se dois nós têm o mesmo nó chefe, pertencem à mesma árvore, se não, pretencem a árvores distintas. Um vetor com uma entrada por nó pode ser usado para implementar isso: esse vetor contém, para cada nó, a identificação do nó chefe de sua árvore. Se o nó é o chefe, a informação no vetor é o próprio nó. Quando duas árvores são unidas, escolhe-se entre os dois chefes envolvidos qual será o chefe da nova árvore (por exemplo, o de menor número de identificação). Marca-se como chefe do outro o que foi escolhido. Quando se buscar a identificação do chefe de um nó que tinha como chefe o nó que foi demovido, tem que verificar se ele ainda é o chefe: quando se busca o chefe de um nó, deve-se buscar até chegar em um nó que é chefe dele mesmo.

```c
  int acha_chefe(no n) {
    no ch = chefe[n];
    while (ch != chefe[ch]) ch = chefe[ch];
    chefe[n] = ch; // otimizacao, na proxima busca vai direto
    return ch;
  }
  void une_subarvores(no n1, no n2) {
    no ch1 = acha_chefe(n1);
    no ch2 = acha_chefe(n2);
    // escolhe ch1 como chefe
    chefe[ch2] = ch1;
  }
  void inicializa_chefes(grafo g) {
    for (n = cada nó em g) chefe[n] = n;
  }
```
O algoritmo de Kruskal pode então ser escrito assim:
```c
grafo kruskal(grafo g) {
  grafo resultado = cria_grafo_vazio();
  for (n = cada nó em g) insere_no(resultado, n);
  inicializa_chefes(resultado);
  fila_de_prioridade f;
  for (a = cada aresta em g) insere(f, a);
  while (!vazia(f)) {
    a = remove(f);
    ch1 = acha_chefe(origem(a));
    ch2 = acha_chefe(destino(a));
    if (ch1 != ch2) {
      insere_aresta(resultado, a);
      une_subarvores(ch1, ch2);
    }
  }
  return resultado;
}
```

#### Exercício

1. Aplique os algoritmos de Prim e de Kruskal no grafo não orientado com as arestas *ab4,ah8,bh11,bc8,hi7,ic2,hg1,ig6,cd7,cf4,gf2,df14,de9,fe10*, partindo do nó *a*.

