#### Listas encadeadas com alocação individual

Quando a lista é encadeada, não necessitamos movimentar fisicamente na memória os dados que já estão na lista, mesmo na implementação de operações que altere a posição desses dados na lista.
Com a alocação contígua dos dados, entretanto, ainda resta o problema de se manter alocada memória no vetor além das necessidades da lista.

Uma outra possibilidade de alocação de memória é alocar cada dado individualmente, e só manter alocados os que estão efetivamente em uso pela lista. Quando um dado é removido, a memória ocupada por ele é liberada. O gerenciamento de memória fica com o sistema de alocação de memória e não mais com a lista. Nesse caso, um dado pode estar em qualquer posição da memória, e sua posição é mantida por um ponteiro e não por um índice.
Para cada posição, deve-se guardar, além do dado, a informação de sucessor (um ponteiro para o dado seguinte).
Essas duas informações (dado e ponteiro) devem ser mantidas juntas, o que geralmente é feito declarando uma estrutura que contém um dado e um ponteiro para outra dessas estruturas. É usual chamar essa estrutura de "nó" ou "nodo".

Nossa lista posa a ter então dois tipos de dados: uma estrutura que mantém informações gerais da lista (em geral se chama de descritor da lista) e uma estrutura para cada dado (um nó).

Nessa implementação, não vamos ter memória desperdiçada com espaço não usado pela lista, mas em compensação tem memória extra gasta pelo sistema de gerenciamento de memória, que vai gerenciar um número maior de alocações.

Exemplo de implementação parcial de lista encadeada com alocação individual:

```c
   /// lista.c
   #include "lista.h"  // contém  "typedef struct _lista *Lista;"

   // a estrutura para conter um nó -- contém um dado e um ponteiro para o próximo nó
   // essa estrutura é interna à implementação da lista
   typedef struct _nodo nodo;
   struct _nodo {
     dado_t dado;
     nodo *prox;
   };

   // a estrutura para o descritor da lista -- contém um ponteiro para o nó que contém o primeiro dado da lista
   typedef struct _lista {
     nodo *prim;
   } lista;
      
   Lista lista_cria(void)
   {
     // cria uma lista vazia
     Lista l;
     l = malloc(sizeof(lista));
     if (l != NULL) {
       // a lista está vazia, o primeiro elemento é representado por NULL
       l->prim = NULL;
     }
     return l;
   }
   
   void lista_destroi(Lista l)
   {
     // precisa liberar a memória de cada nó, e do descritor
     nodo *no = l->prim;
     while (no != NULL) {
       // salva o ponteiro que está dentro do nó, não podemos acessar o conteúdo do nó depois do free
       nodo *aux = no->prox;
       free(no);
       no = aux;
     }
     free(l);
   }

   bool lista_vazia(Lista l)
   {
     return l->prim == NULL;
   }
   
   // função auxiliar para alocar e inicializar um nó
   static nodo *cria_nodo(dado_t d, nodo *prox)
   {
     nodo *no = malloc(sizeof(nodo));
     if (no != NULL) {
       no->dado = d;
       no->prox = prox;
     }
     return no;
   }
   
   void lista_insere_inicio(Lista l, dado_t d)
   {
     // aloca um nó par conter o novo dado. 
     // esse nó estará no início da lista, então o nó seguinte a ele é o nó que atualmente é o primeiro da lista
     nodo *novo = cria_nodo(d, l->prim);
     // se não deu certo cai fora
     if (novo == NULL) return;
     // ele é o novo primeiro nó
     l->prim = novo;
   }

   static nodo *lista_ultimo_nodo(Lista l)
   {
     if (lista_vazia(l)) return NULL;
     nodo *no = l->prim;
     while (no->prox != NULL) {
       no = no->prox;
     }
     return no;
   }
   
   void lista_insere_fim(Lista l, dado_t d)
   {
     // se a lista estiver vazia, insere no início
     if (lista_vazia(l)) {
       lista_insere_inicio(l, d);
     }
     // aloca um nó par conter o novo dado. 
     // esse nó estará no final da lista, então o nó seguinte a ele é NULL
     nodo *novo = cria_nodo(d, NULL);
     // se não deu certo cai fora
     if (novo == NULL) return;
     // o antigo último nó deve apontar para o novo último
     nodo *ult = lista_ultimo_nodo(l);
     ult->prox = novo;
   }

   // retorna o nodo em dada posição da lista (pos 0 é o primeiro; retorna NULL se não existir)
   static nodo *lista_nodo_pos(Lista l, int pos)
   {
     if (pos < 0) return NULL;
     nodo *no = l->prim;
     int p = 0;
     while (no != NULL && p < pos) {
       no = no->prox;
       p++;
     }
     return no;
   }
   
   void lista_insere_pos(Lista l, int pos, dado_t d)
   {
     if (pos < 0) return;
     if (pos == 0) {
        lista_insere_inicio(l, d);
	return;
     }
     // acha o nó na posição anterior à inserção
     nodo *anterior = lista_nodo_pos(l, pos-1);
     if (anterior == NULL) return;
     // nó na posição seguinte à inserção
     nodo *seguinte = anterior->prox;
     // aloca um nó par conter o novo dado. 
     // esse nó estará antes de nó seguinte
     nodo *novo = cria_nodo(d, seguinte);
     // o novo nó está depois do nó anterior
     anterior->prox = novo;
   }
```

#### Exercícios

1. Implementa as funções de remoção
2. Teste a lista

