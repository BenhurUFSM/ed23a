### Lista encadeada

Nas implementações contíguas, a ordem física em que os dados da lista são mantidos em memória corresponde à ordem lógica desses dados na lista.
Sabendo a posição de um dos dados, com um cálculo simples se obtém a posição de qualquer outro.
Em compensação, se um dado muda sua posição lógica na lista (devido a inclusões ou remoções), os dados devem ser fisicamente movimentados na memória para que a correspondência seja mantida.

Uma outra forma de se organizar uma lista em memória é quebrando essa correspondência, e usando uma outra forma de se encontrar os dados da lista em vez de sua posição relativa na memória.
A forma mais comum de se fazer isso é manter, para cada dado na lista, uma informação que permite chegar ao dado que o sucede na lista.
Por exemplo, se os dados da lista são colocados em um vetor, a informação que permite chegar a um dado é a posição no vetor.
Uma forma de se manter essa posição é colocando a lista em dois vetores de mesmo tamanho, um contendo espaço para os dados e outro contendo a posição do sucessor de cada dado. A posição onde está o dado que é o sucessor do dado na posição 3 do vetor de dados é colocada na posição 3 do vetor de posições. Como cada dado da lista pode estar em qualquer posição do vetor, necessitamos saber a posição do primeiro dado da lista.
Quando uma lista é organizada dessa forma, com um dado levando à informação do seguinte e assim por diante, dizemos que a lista é "encadeada".

Por exemplo, nesse esquema, a lista com os dados `X Y Z` poderia ser armazenada como abaixo:
```
   cap 5
   prim 2
   num 3
   dados       Z ? X Y ?
   sucessores -1 ? 3 0 ?
```
Os sinais `?` marcam valores nos vetores que não estão sendo ocupadas pela lista.
O primeiro dado da lista está na posição 2 (é o dado `X`).
No vetor de sucessores, na mesma posição (2) do dado, tem o valor 3, que é a posição do dado seguinte (o segundo dado da lista).
Na posição 3, está o valor `Y` e o sucessor é 0.
Na posição 0 está o terceiro dado da lista `Z` e o sucessor -1. -1 é um valor especial para representar que esse dado não tem sucessor, é o último da lista.

Se dessa lista fosse removido o segundo dado (na posição 1 da lista e 3 do vetor), a lista ficaria assim:
```
   cap 5
   prim 2
   num 2
   dados       Z ? X Y ?
   sucessores -1 ? 0 0 ?
```

Se na lista original fosse inserido o dado W na segunda posição da lista (entre o X e o Y), a lista poderia ficar assim:
```
   cap 5
   prim 2
   num 4
   dados       Z W X Y ?
   sucessores -1 3 1 0 ?
```
ou assim:
```
   cap 5
   prim 2
   num 4
   dados       Z ? X Y W
   sucessores -1 ? 4 0 3
```
Tanto na inserção quanto na remoção, nenhum dado presente na lista precisou ser movimentado

Claro, tem algumas desvantagens nessa implementação:
- é necessário mais memória, para colocar informação sobre os sucessores
- é necessário encontrar os espaços livres quando se tem uma inclusão. Uma forma de se fazer isso é organizar uma segunda lista no mesmo vetor, contendo os elementos livres, e retirar dela nas inserções, colocar nela nas remoções
- para implementar inclusão e remoção, tem que alterar a informação de sucessor do dado na posição anterior ao ponto de inserção/remoção; para encontrar esse dado temos que percorrer a lista desde o início.
- caso se use realocação de memória que libera espaço quando a área ocupada pela lista é muito inferior à capacidade, pode ser necessário deslocar dados que estejam na área que será liberada (já que um dado pode estar em qualquer lugar, não se tem garantia de que não tenha dado no final da área alocada), além de rearranjar a lista de livres.

A alocação pode ser um pouco otimizada, colocando os valores dos sucessores junto a cada dado, e alocando um só vetor.

Uma outra possibilidade de alocação de memória é alocar cada dado individualmente, e só manter alocados os que estão efetivamente em uso pela lista. Quando um dado é removido, a memória ocupada por ele é liberada. O gerenciamento de memória fica com o sistema de alocação de memória e não mais com a lista. Nesse caso, um dado pode estar em qualquer posição da memória, e sua posição é mantida por um ponteiro e não por um índice.
Para cada posição, deve-se guardar, além do dado, a informação de sucessor (um ponteiro para o dado seguinte).
Essas duas informações (dado e ponteiro) devem ser mantidas juntas, o que geralmente é feito declarando uma estrutura que contém um dado e um ponteiro para outra dessas estruturas. É usual chamar essa estrutura de "nó" ou "nodo".

Nossa lista posa a ter então dois tipos de dados: uma estrutura que mantém informações gerais da lista (em geral se chama de descritor da lista) e uma estrutura para cada dado (um nó).

Nessa implementação, não vamos ter memória desperdiçada com espaço não usado pela lista, mas em compensação tem memória extra gasta pelo sistema de gerenciamento de memória, que vai gerenciar um número maior de alocações.

Uma implementação possível para a interface de lista vista antes, usando lista encadeada com alocação individual poderia ser:
```c
   #include "lista.h"

   // a estrutura para conter um nó -- contém um dado e um ponteiro para o próximo nó
   // essa estrutura é interna à implementação da lista
   typedef struct no_t no_t;
   struct no_t {
     dado_t d;
     no_t *prox;
   };

   // a estrutura para o descritor da lista -- contém um ponteiro para o nó que contém o primeiro dado da lista
   typedef struct _lista {
     int num;
     no_t *prim;
   } lista;
      
   Lista lista_cria(int cap)
   {
     // cria uma lista vazia -- ignora a capacidade, a lista poderá conter tantos nós quantos couberem na memória
     // poderia também guardar a capacidade e limitar o número máximo de dados na lista
     Lista l;
     l = malloc(sizeof(lista));
     if (l != NULL) {
       l->num = 0;
       l->prim = NULL;
     }
     return l;
   }
   
   void lista_destroi(Lista l)
   {
     // precisa liberar a memória de cada nó, e do descritor
     no_t *no = l->prim;
     while (no != NULL) {
       // salva o ponteiro que está dentro do nó, não podemos acessar o conteúdo do nó depois do free
       no_t *aux = no->prox;
       free(no);
       no = aux;
     }
     free(l);
   }
      
   int lista_num_elem(Lista l)
   {
     return l->num;
   }

   bool lista_dado(Lista l, int pos, dado_t *pd)
   {
     if (pos < 0 || pos >= l->num) return false;
     // tem que percorrer a lista, até encontrar o nó na posição pos
     no_t *no = l->prim; // inicia no primeiro
     int pos_no = 0;     // ele está na posição 0
     while (pos_no < pos) {
       no = no->prox;    // ainda não chegamos na posição desejada, vamos pro próximo
       pos_no++;         // que está na próxima posição
     }
     // 'no' aponta para o nó que tem o dado na posição pos -- copia o dado pra quem chamou
     *pd = no->d;
     return true;
   }

   bool lista_insere(Lista l, int pos, dado_t d)
   {
     if (pos < 0 || pos > l->num) return false;
     // vamos inserir um novo dado, para isso precisamos um novo nó
     no_t *novo = malloc(sizeof(no_t));
     if (novo == NULL) return false;
     // o novo nó vai conter o novo dado
     novo->d = d;
     // temos que encontrar o nó que precede o ponto de inserção, porque o nó seguinte a esse nó será o novo nó
     if (pos == 0) {
       // se a inserção é no início da lista, o novo nó é o primeiro da lista, e o seguinte é o antigo primeiro
       // cuidado para fazer a atribuição na ordem certa e não perder o valor do l->prim antigo
       novo->prox = l->prim;
       l->prim = novo;
     } else {
       // a inserção não é no início, temos que encontrar o nó anterior (o que está na posição pos-1)
       no_t *ant = l->prim;
       int pos_ant = 0;
       while (pos_ant < pos-1) {
         ant = ant->prox;
         pos_ant++;
       }
       // o nó que segue o novo é o que estava na posição pos (o próximo ao que está na posição pos-1)
       // o novo nó passa a estar na posição pos, então ele é o novo sucessor do que está em pos-1
       novo->prox = ant->prox;
       ant->prox = novo;
     }
     // temos um dado a mais na lista
     l->num++;
     return true;
   }

   bool lista_remove(Lista l, int pos)
   {
     if (pos < 0 || pos >= l->num) return false;
     no_t *vitima; // vai apontar para o nó a ser removido
     if (pos == 0) {
       // remoção do primeiro nó, o novo primeiro é o seguinte a ele
       vitima = l->prim;
       l->prim = vitima->prox;
     } else {
       // como na inserção, temos que encontrar o nó que está logo antes do nó que será removido
       no_t *ant = l->prim;
       int pos_ant = 0;
       while (pos_ant < pos-1) {
         ant = ant->prox;
         pos_ant++;
       }
       // a vítima é quem está logo depois de quem está logo antes de quem será removido
       vitima = ant->prox;
       // quem passa a estar depois do anterior é quem estava depois da vítima
       ant->prox = vitima->prox;
     }
     // o nó removido não está mais no encadeamento da lista
     // libera a memória do nó removido, e diminui o número de dados na lista
     free(vitima);
     l->num--;
     return true;
   }
```

#### Exercícios

1. Implementa a lista com encadeamentos por posição em um vetor.
2. Implementa suporte a posições negativas.
3. O programa cliente funciona em todas as implementações de lista sem alterações?

* * *

### Busca em lista encadeada

Na interface de listas que vimos até agora, o acesso a um elemento da lista se dá por posição.
Pode-se também ter acesso por conteúdo.
Vimos em aula duas formas, uma para verificar a existência de um determinado valor na lista, e outra com a procura de um elemento à partir de parte do valor (um código, por exemplo).
Para essas implementações, necessitamos poder comparar dois valores; para manter a implementação da lista independente do tipo de dados que ela armqzena, colocamos a comparação de elementos em uma função externa.
A implementação vista em aula está abaixo:

O programa principal:
```c
#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

bool iguais(dado_t d1, dado_t d2)
{
    return d1.codigo == d2.codigo;
}


int main()
{
    lista_t *l;
    
    l = lista_cria(10);
    for (int i=0; i<10000; i++) {
        dado_t d = {i, rand()%10000/100.0};
        lista_insere(l, 0, d);
    }
    for (int i=0; i<10; i++) {
        int n = rand()%20000;
        dado_t d = {n, 0};
        dado_t d2;
        if (lista_dado_igual(l, d, &d2)) {
            printf("%d pertence a lista, cod=%d, val=%.2f\n", n, d2.codigo, d2.valor);
        } else {
            printf("%d nao pertence a lista\n", n);
        }
    }
    lista_destroi(l);
    return 0;
}
```
A interface, lista.h (com a definição do tipo de dados como uma estrutura com um código e um valor):
```c
#ifndef _LISTA_H_
#define _LISTA_H_
#include <stdbool.h>
   
typedef struct {
    int codigo;
    double valor;
} dado_t;

typedef struct lista lista_t;
   
lista_t *lista_cria(int cap);
void lista_destroi(lista_t *l);
   
// retorna o número de dados atualmente na lista l
int lista_num_elem(lista_t *l);
// coloca em *pd o dado na posição pos da lista l, retorna false se não ok (e não coloca nada em *pd)
bool lista_dado(lista_t *l, int pos, dado_t *pd);
// insere d na posicao pos da lista l, retorna true se ok
bool lista_insere(lista_t *l, int pos, dado_t d);
// remove o dado na posição pos da lista l, retorna true se ok
bool lista_remove(lista_t *l, int pos);

// retorna verdadeiro se a lista contem um dado igual a d
bool lista_pertence(lista_t *l, dado_t d);
// retorna posicao de um dado igual a d na lista l, ou -1
int lista_posicao(lista_t *l, dado_t d);
// coloca em *d2 o dado igual a d e retorna true, ou retorna false
bool lista_dado_igual(lista_t *l, dado_t d, dado_t *d2);

// função auxiliar, externa, para a comparação de dois dados 
//   (uma forma de tornar a lista mais independente do dado seria fornecer acesso à essa função fornecendo 
//   um ponteiro à ela na inicialização da lista)
bool iguais(dado_t d1, dado_t d2);
#endif //_LISTA_H_
```
A implementação da lista, lista.c:
```c
/// Implementação de lista com encadeamento por ponteiros e elocação individual de nós
#include "lista.h"
#include <stdlib.h>

// a estrutura para conter um nó -- contém um dado e um ponteiro para o próximo nó
// essa estrutura é interna à implementação da lista
typedef struct no_t no_t;
struct no_t
{
  dado_t d;
  no_t *prox;
};

// a estrutura para o descritor da lista -- contém um ponteiro para o nó que contém o primeiro dado da lista
struct lista
{
  int num;
  no_t *prim;
};

lista_t *
lista_cria (int cap)
{
  // cria uma lista vazia -- ignora a capacidade, a lista poderá conter tantos nós quantos couberem na memória
  // poderia também guardar a capacidade e limitar o número máximo de dados na lista
  lista_t *l;
  l = malloc (sizeof (lista_t));
  if (l != NULL)
    {
      l->num = 0;
      l->prim = NULL;
    }
  return l;
}

void
lista_destroi (lista_t * l)
{
  // precisa liberar a memória de cada nó, e do descritor
  no_t *no = l->prim;
  while (no != NULL)
    {
      // salva o ponteiro que está dentro do nó, não podemos acessar o conteúdo do nó depois do free
      no_t *aux = no->prox;
      free (no);
      no = aux;
    }
  free (l);
}

int
lista_num_elem (lista_t * l)
{
  return l->num;
}

bool
lista_dado (lista_t * l, int pos, dado_t * pd)
{
  if (pos < 0 || pos >= l->num)
    return false;
  // tem que percorrer a lista, até encontrar o nó na posição pos
  no_t *no = l->prim;		// inicia no primeiro
  int pos_no = 0;		// ele está na posição 0
  while (pos_no < pos)
    {
      no = no->prox;		// ainda não chegamos na posição desejada, vamos pro próximo
      pos_no++;			// que está na próxima posição
    }
  // no aponta para o nó que tem o dado na posição pos -- copia o dado pra quem chamou
  *pd = no->d;
  return true;
}

bool
lista_insere (lista_t * l, int pos, dado_t d)
{
  if (pos < 0 || pos > l->num)
    return false;
  // vamos inserir um novo dado, para isso precisamos um novo nó
  no_t *novo = malloc (sizeof (no_t));
  if (novo == NULL)
    return false;
  // o novo nó vai conter o novo dado
  novo->d = d;
  // temos que encontrar o nó que precede o ponto de inserção, porque o nó seguinte a esse nó será o novo nó
  if (pos == 0)
    {
      // se a inserção é no início da lista, o novo nó é o primeiro da lista, e o seguinte é o antigo primeiro
      // cuidado para fazer a atribuição na ordem certa e não perder o valor do l->prim antigo
      novo->prox = l->prim;
      l->prim = novo;
    }
  else
    {
      // a inserção não é no início, temos que encontrar o nó anterior (o que está na posição pos-1)
      no_t *ant = l->prim;
      int pos_ant = 0;
      while (pos_ant < pos - 1)
	{
	  ant = ant->prox;
	  pos_ant++;
	}
      // o nó que segue o novo é o que estava na posição pos (o próximo ao que está na posição pos-1)
      // o novo nó passa a estar na posição pos, então ele é o novo sucessor do que está em pos-1
      novo->prox = ant->prox;
      ant->prox = novo;
    }
  // temos um dado a mais na lista
  l->num++;
  return true;
}

bool
lista_remove (lista_t * l, int pos)
{
  if (pos < 0 || pos >= l->num)
    return false;
  no_t *vitima;			// vai apontar para o nó a ser removido
  if (pos == 0)
    {
      // remoção do primeiro nó, o novo primeiro é o seguinte a ele
      vitima = l->prim;
      l->prim = vitima->prox;
    }
  else
    {
      // como na inserção, temos que encontrar o nó que está logo antes do nó que será removido
      no_t *ant = l->prim;
      int pos_ant = 0;
      while (pos_ant < pos - 1)
	{
	  ant = ant->prox;
	  pos_ant++;
	}
      // a vítima é quem está logo depois de quem está logo antes de quem será removido
      vitima = ant->prox;
      // quem passa a estar depois do anterior é quem estava depois da vítima
      ant->prox = vitima->prox;
    }
  // o nó removido não está mais no encadeamento da lista
  // libera a memória do nó removido, e diminui o número de dados na lista
  free (vitima);
  l->num--;
  return true;
}

// retorna verdadeiro se a lista contem um dado igual a d
bool lista_pertence(lista_t *l, dado_t d)
{
    for (no_t *el = l->prim; el != NULL; el = el->prox) {
        if (iguais(el->d, d)) {
            return true;
        }
    }

    return false;
}

// retorna posicao de um dado igual a d na lista l, ou -1
int lista_posicao(lista_t *l, dado_t d){
    int ct = 0;
    
    for (no_t *el = l->prim; el != NULL; el = el->prox) {
        if (iguais(el->d, d)) return ct;
        ct++;
    }

    return -1;
}
// coloca em *d2 um dado igual a d na lista l e retorna true, ou retorna false
bool lista_dado_igual(lista_t *l, dado_t d, dado_t *d2)
{
    for (no_t *el = l->prim; el != NULL; el = el->prox) {
        if (iguais(el->d, d)) {
            if (d2 != NULL) *d2 = el->d;
            return true;
        }
    }

    return false;
}
```

### Listas duplamente encadeadas

Por vezes, necessitamos de operações de inclusão e remoção em ambas as pontas da lista, e a lista simplesmente encadeada não tem um desempenho bom o suficiente para todas essas operações. Não é muito difícil de se otimizar para se ter um bom desempenho em três dessas operações, mas não nas quatro.
A adição de um segundo encadeamento na lista resolve essa limitação, ao custo de mais memória para armazenar mais um ponteiro em cada nó da lista.
Cada nó passa a ter dois ponteiros: um auq aponta para o próximo elemento da lista como antes e um outro que aponta para o elemento anterior.
Em todas as operações que alteram a lista (inclusão e remoção de elementos, temos que manter atualizados esses dois ponteiros, o que torma a implementação um pouco mais complexa.
No descritor da lista, se coloca um ponteiro para o último elemento da lista, além do primeiro, para permitir a que a inclusão e remoção no final da lista seja tão fácil quanto no início.

#### Implementação

Suponha dque o descritor da lista e o nó sejam declarados como abaixo:
```c
   struct lista {
     int num;    // número de elementos na lista
     no_t *prim; // nó que contém o primeiro elemento da lista
     no_t *ult;  // nó que contém o último elemento da lista
   };
   struct no {
     dado_t info; // a informação que é mantida por este nó
     no_t *prox;  // ponteiro para o nó seguinte
     no_t *ant;   // ponteiro para o nó anterior
   };
```

A operação de remoção de um nó qualquer da lista seria simples, tendo um ponteiro para o nó a remover:
```c
   ///
   no_t *n;
   /// faz n apontar para o nó a remover
   // cria variáveis auxiliares para apontar para os nós envolvidos
   no_t *depois = n->prox; // o nó que está depois do nó a ser removido
   no_t *antes = n->ant;   // o nó que está antes do nó a ser removido
   // remove o nó dos encadeamentos
   antes->prox = depois;   // o nó que segue o nó anterior ao removido é o que está depois do removido
   depois->ant = antes;    // o nó que antecede o nó que está após o removido é o que está antes do removido
   // libera a memória do nó removido
   free(n);
   l->num--;
```
Esse código funciona quando se está removendo um nó intermediário, mas teremos problemas com o primeiro e o último nós (os ponteiros `antes` ou `depois` podem ser `NULL` (e não podem ser dereferenciados), e devemos alterar o descritor. Devemos tomar alguns cuidados especiais:
```c
   /// ...
   // remove o nó dos encadeamentos
   if (antes != NULL) {
     antes->prox = depois;   // o nó que segue o nó anterior ao removido é o que está depois do removido
   } else {
     l->prim = depois;
   }
   if (depois != NULL) {
     depois->ant = antes;    // o nó que antecede o nó que está após o removido é o que está antes do removido
   } else {
     l->ult = antes;
   }
   /// ...
```
Esse código funciona para a remoção quando a lista tem um só elemento?

Na inserção, tem que tomar cuidados semelhantes.

Tendo os dois encadeamentos e o número de elementos, pode-se otimizar um pouco a busca de um elemento por posição: se a posição buscada for mais próxima do início que do final da lista, realiza-se a busca a partir do primeiro elemento em direção ao final da lista, e se for mais próxima do final, busca-se a partir do último elemento em direção ao início.

### Outras alternativas de implementação de listas

#### Ponteiro para o último elemento no descritor da lista simplesmente encadeada

Dessa forma, a operação de inclusão no final da lista pode ser otimizada, sem ser necessário de percorrer a lista toda até o final. A remoção no final não é afetada, necessitando o percurso de toda a lista.

#### Lista circular

Ao invés de o último elemento do encadeamento ter um ponteiro em NULL, ele aponta para o primeiro elemento.
Em uma lista simplesmente encadeada, dá para nesse caso se manter no descritor somente um ponteiro para o último elemento e não para o primeiro, e ainda assim não necessitar percorrer a lista toda para inserção e remoção no início e inserção no final.

#### Uso de nó sentinela

Usa-se um nó suplementar, que ;e alocado na inicialização da lista e somente removido na sua destruição. Esse nó não contém um dado válido, serve simplesmente para garantir que sempre existe um nó no encadeamento, mesmo que a lista esteja vazia, e é para esse nó que o descritor aponta.
Geralmente é usado com a implementação circular de lista encadeada.
Torna mais simples as operações de inserção e remoção, porque não existe mais o caso especial de se inserir ou remover no início ou final do encadeamento.

#### Não uso do descritor

Nesse caso, usa-se um ponteiro para o primeiro nó para se representar a lista.
Economiza-se memória, mas a interface de acesso à lista fica menos clara, porque operações que alteram a lista (inserção e remoção) podem ter que alterar o ponteiro que representa a lista, que pertence ao usuário. O uso de um nó sentinela pode auxiliar, mas o argumento de economia de memória enfraquece.

### Exercícios

Tem um monte no livro da Nina.
