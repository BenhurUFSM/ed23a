### Listas duplamente encadeadas

Por vezes (em uma fila do tipo deque, por exemplo), necessitamos de operações de inclusão e remoção em ambas as pontas da lista, e a lista simplesmente encadeada não tem um desempenho bom o suficiente para todas essas operações. Não é muito difícil de se otimizar para se ter um bom desempenho em três dessas operações, mas não nas quatro.
A adição de um segundo encadeamento na lista resolve essa limitação, ao custo de mais memória para armazenar mais um ponteiro em cada nó da lista.
Cada nó passa a ter dois ponteiros: um que aponta para o próximo elemento da lista (como em uma lista simplesmente encadeada) e um outro que aponta para o elemento anterior.
Em todas as operações que alteram a lista (inclusão e remoção de elementos), temos que manter atualizados esses dois ponteiros, o que torma a implementação um pouco mais complexa.
No descritor da lista, se coloca um ponteiro para o último elemento da lista, além do primeiro, para permitir a que a inclusão e remoção no final da lista seja tão fácil quanto no início.

#### Implementação

Suponha que o descritor da lista e o nó sejam declarados como abaixo:
```c
   typedef struct lista *Lista;
   typedef struct nodo nodo;
   struct lista {
     int num;    // número de elementos na lista
     nodo *prim; // nó que contém o primeiro elemento da lista
     nodo *ult;  // nó que contém o último elemento da lista
   };
   struct nodo {
     dado_t info; // a informação que é mantida por este nó
     nodo *prox;  // ponteiro para o nó seguinte
     nodo *ant;   // ponteiro para o nó anterior
   };
```

A operação de remoção de um nó qualquer da lista seria simples, tendo um ponteiro para o nó a remover:
```c
   ///
   nodo *vitima;
   // ... (faz 'vitima' apontar para o nó a remover)
   // cria variáveis auxiliares para apontar para os nós envolvidos
   nodo *depois = vitima->prox; // o nó que está depois do nó a ser removido
   nodo *antes = vitima->ant;   // o nó que está antes do nó a ser removido
   // remove o nó dos encadeamentos
   antes->prox = depois;   // o nó que segue o nó anterior ao removido é o que está depois do removido
   depois->ant = antes;    // o nó que antecede o nó que está após o removido é o que está antes do removido
   // libera a memória do nó removido
   free(vitima);
   l->num--;
```
Esse código funciona quando se está removendo um nó intermediário, mas teremos problemas com o primeiro e o último nós (os ponteiros `antes` ou `depois` podem ser `NULL` (e não podem ser dereferenciados), e devemos alterar o descritor. Devemos tomar alguns cuidados especiais:
```c
   /// ...
   // remove o nó dos encadeamentos
   if (antes != NULL) {
     antes->prox = depois;   // o nó que segue o nó anterior ao removido é o que está depois do removido
   } else {
     l->prim = depois;       // o nó removido era o primeiro da lista, o que segue é o novo primeiro
   }
   if (depois != NULL) {
     depois->ant = antes;    // o nó que antecede o nó que está após o removido é o que está antes do removido
   } else {
     l->ult = antes;         // o nó removido era o último da lista, o antecessor dele é o novo último
   }
   /// ...
```
Esse código funciona para a remoção quando a lista tem um só elemento?

Na inserção, tem que tomar cuidados semelhantes.

Tendo os dois encadeamentos e o número de elementos, pode-se otimizar um pouco a busca de um elemento por posição: se a posição buscada for mais próxima do início que do final da lista, realiza-se a busca a partir do primeiro elemento em direção ao final da lista, e se for mais próxima do final, busca-se a partir do último elemento em direção ao início.

### Outras alternativas de implementação de listas

#### Ponteiro para o último elemento no descritor da lista simplesmente encadeada

Dessa forma, a operação de inclusão no final da lista pode ser otimizada, sem ser necessário percorrer a lista toda até o final. A remoção no final não é afetada, necessitando o percurso de toda a lista.

#### Lista circular

Ao invés de o último elemento do encadeamento ter um ponteiro em NULL, ele aponta para o primeiro elemento.
Em uma lista simplesmente encadeada, dá para nesse caso se manter no descritor somente um ponteiro para o último elemento e não para o primeiro, e ainda assim não necessitar percorrer a lista toda para inserção e remoção no início e inserção no final.

#### Uso de nó sentinela

Usa-se um nó suplementar, que é alocado na inicialização da lista e somente removido na sua destruição. Esse nó não contém um dado válido, serve simplesmente para garantir que sempre existe um nó no encadeamento, mesmo que a lista esteja vazia, e é para esse nó que o descritor aponta.
Geralmente é usado com a implementação circular de lista encadeada.
Torna mais simples as operações de inserção e remoção, porque não existem mais os casos em que se necessite alterar o ponteiro do descritor, porque ele aponta sempre para o sentinela.

#### Não uso do descritor

Nesse caso, usa-se um ponteiro para o primeiro nó para se representar a lista.
Economiza-se memória, mas a interface de acesso à lista fica menos clara, porque operações que alteram a lista (inserção e remoção) podem ter que alterar o ponteiro que representa a lista, que pertence ao usuário.
O uso de um nó sentinela pode evitar esse problema.

### Outros aspectos de listas (e de outras estruturas de dados)

#### Listas genéricas

Nas implementações de listas que vimos, o dado armazenado na lista tem o tipo 'dado_t'.
Como fazer se o programa necessita listas de mais de um tipo de dados?
