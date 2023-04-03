## Listas

Uma lista é um tipo abstrato de dados que mantém um conjunto de dados, além de manter a ordem relativa entre esses dados.
Um dos dados é o primeiro dado da lista, outro é o segundo etc até o último dado da lista.
Cada dado pode ser de qualquer tipo, e a lista não se preocupa com isso, somente em manter esses dados e a ordem entre eles.
Exemplos de listas:
- lista de sentenças em uma função
- lista de strings que formam um texto
- lista de compras
- uma string é uma lista de caracteres
- lista de palavras chutadas em uma partida de wordle
- lista de partidas em um campeonato de xadrez

### Operações em uma lista

O conjunto de operações abaixo é típico de uma lista:
- obter o número de dados na lista
- obter o dado em determinada posição da lista
- remover o dado em determinada posição da lista
- inserir um dado em determinada posição da lista

### Listas restritas

Em algumas aplicações, é comum necessitar-se de listas em que nem todas as operações são necessárias. Isso permite algumas otimizações importantes na implementação dessas estruras. As mais usuais são limitações nar operações de inserção e remoção, permitindo que sejam realizadas somente nas primeira e/ou na última posição. São tão comuns que tem nome próprio:
- **fila**: só permite inserção em um lado e remoção no outro lado da lista. Os dados saem da fila na mesma ordem em que entraram e são por vezes chamadas FIFO (first in first out).
- **pilha**: só permite inserção e remoção em um dos lados da lista. Os dados saem da pilha em ordem inversa à que entraram; são por vezes chamadas LIFO (last in first out).
- **deque**: permite inserção e remoção nas duas extremidades da lista.

Por exemplo, a função abaixo inverte todos os elementos de uma fila recebida como argumento, usando uma pilha auxiliar:
```c
   void fila_inverte(Fila f)
   {
     Pilha p = pilha_cria();
     while (!fila_vazia(f)) {
       pilha_insere(p, fila_remove(f));
     }
     while (!pilha_vazia(p)) {
       fila_insere(f, pilha_remove(p));
     }
     pilha_destroi(p);
   }
```
Se no exemplo acima o primeiro while parasse na metade (quando o número de elementos nas duas estruturas fossem iguais), qual seria o conteúdo da fila no final? A fila é recebida com os valores de 1 a 10.

### Implementação de lista

Uma lista pode ser implementada de diversas formas. É comum classificar essas formas de acordo como a memória é alocada e como a ordem dos dados é implementada. A alocação pode ser estática ou dinâmica, a ordem dos dados pode ser mantida por contiguidade ou por encadeamento.

#### Lista contígua

Os dados são mantidos em uma região contígua de memória, em um vetor ou em uma região alocada dinamicamente (que pode ser tratada como um vetor).
O primeiro elemento da lista é colocado na primeira posição desse vetor, o segundo na segunda posição etc.
O número de elementos que se pode colocar na lista corresponde ao tamanho do vetor.
Para se saber quantos elementos existem na lista (que geralmente é inferior ao tamanho do vetor), em geral se usa ou um elemento demarcador de fim (como em uma string em C) ou uma variável auxiliar que contém o número de elementos (dentro de uma struct que descreve a lista).
Na maioria dos casos se usa a segunda opção, por ser mais econômica (é necessário um inteiro e não o espaço de um item) e mais rápida (a operação de se obter o número de elementos na lista é mais simples).

A implementação das operações básicas:
- o número de elementos é obtido diretamente, já que tem uma variável com essa informação
- a obtenção do dado em uma posição da lista é realizada com a operação a acesso a elemento do vetor nessa mesma posição
- a remoção de um dado em determinada posição implica no deslocamento físico de todos os dados da lista, desde a posição seguinte até o final da lista; a remoção do último elemento não necessita movimentação alguma, mas a do primeiro necessita movimentação de todos os dados restantes na lista
- a inserção de um elemento em uma posição da lista necessita da movimentação de todos os dados da lista a partir dessa posição até o final, para gerar espaço para o elemento inserido; a inserção no final da lista não necessita de movimentação, mas no início implica na movimentação da lista inteira

Esse tipo de lista pode também ser implementado com alocação dinâmica, sem se limitar o número máximo de itens que a lista pode conter.
Nesse caso, em uma inserção em uma lista que já ocupa toda a memória disponível, aloca-se uma região maior, copia-se o conteúdo da lista para a nova região e libera-se a região anterior. Deve ser mantida uma nova variável com a capacidade atual da memória alocada para a lista.
No caso de ser ter uma lista em que o número de elementos pode diminuir bastante depois de ter sido grande, para não se manter o consumo de memória muito alto, a operação de remoção pode também realocar a memória para uma capacidade inferior quando a capacidade e a utilização forem muito diferentes.

Uma outra forma semelhante de se implementar uma lista contígua é em uma implementação **circular**, onde o primeiro elemento da lista não necessariamente é colocado na primeira posição do vetor (mas os elementos continuam contíguos em memória, o segundo elemento está logo depois do primeiro etc).
Uma nova variável é necessária, para informar em que posição do vetor está o primeiro elemento da lista.
Uma remoção do primeiro elemento da lista implica somente na alteração da variável que diz onde está o primeiro elemento, não necessita deslocar fisicamente os elementos dentro do vetor (claro, tem que alterar também a informação sobre o número de elementos, como antes).
O espaço não usado no início do vetor é usado quando a posição final do vetor já está usada e há uma nova inserção -- o elemento após o que está na última posição do vetor é colocado na primeira posição.
Por exemplo, as variáveis representadas abaixo armazenam uma lista com os valores `3 8 9`, os valores 1 e 2 no vetor não estão na lista. `cap` é a capacidade (quantos elementos cabem no vetor), `pri` é o índice no vetor onde está o primeiro elemento da lista, `num` é o número de elementos na lista, `vet` é o vetor.
```
   cap 5
   pri 3
   num 3
   vet 9 1 2 3 8
```
A inserção de um valor no início dessa lista pode ser colocado sobre o valor `2`, sem necessitar deslocar nenhum dado; a inserção no final da lista pode ser colocado sobre o `1`, sem necessitar deslocar. 
As 4 operações de inserção e remoção nas extremidades da lista são implementadas de forma simples, e seu tempo de execução independe do número de elementos contidos na lista.
A inserção em alguma outra posição irá implicar em deslocamento de dados na memória, mas pode-se escolher deslocar o início da lista para posições anteriores do vetor ou o final da lista para posições posteriores, o que for menor.

Para encontrar a posição `pv` no vetor onde está o item na posição `pl` da lista, faz-se `pv = (pri+pl)%cap`.

A alocação dinâmica de memória para um vetor circular é um pouco mais trabalhosa, porque tem que deslocar as duas porções da lista para regiões diferentes do novo espaço.

Um exemplo de implementação de uma lista com alocação estática, contígua, nâo circular, está abaixo.
Os dados armazenados na lista são do tipo `dado_t` (que pode ser qualquer coisa).
```c
   // lista.h
   #ifndef _LISTA_H_
   #define _LISTA_H_
   #include <stdbool.h>
   
   typedef struct _lista *Lista;
   
   Lista lista_cria(int cap);
   void lista_destroi(Lista l);
   
   // retorna o número de dados atualmente na lista l
   int lista_num_elem(Lista l);
   
   // coloca em *pd o dado na posição pos da lista l, retorna false se não ok (e não coloca nada em *pd)
   bool lista_dado(Lista l, int pos, dado_t *pd);
   
   // insere d na posicao pos da lista l, retorna true se ok
   bool lista_insere(Lista l, int pos, dado_t d);
   
   // remove o dado na posição pos da lista l, retorna true se ok
   bool lista_remove(Lista l, int pos);
   #endif //_LISTA_H_
```
```c
   // lista.c
   #include "lista.h"
   
   typedef struct _lista {
     int cap;
     int num;
     dado_t *vet;
   } lista;
   
   Lista lista_cria(int cap)
   {
     // aloca memória para o descritor da lista
     Lista l = malloc(sizeof(lista));
     if (l == NULL) return NULL;
     // aloca memória para os dados da lista
     l->vet = malloc(cap * sizeof(dado_t));
     if (l->vet == NULL) {
       free(l);
       return NULL;
     }
     l->cap = cap;
     l->num = 0;
     return l;
   }
   
   void lista_destroi(Lista l)
   {
     // libera a memória dos dados e do descritor
     free(l->vet);
     free(l);
   }
   
   int lista_num_elem(Lista l)
   {
     // essa é fácil, o número de elementos tá no descritor
     return l->num;
   }
   
   bool lista_dado(Lista l, int pos, dado_t *pd)
   {
     // vê se existe alguém nessa pos
     if (pos < 0 || pos >= l->num) {
       return false;
     }
     *pd = l->vet[pos];
     return true;
   }
   
   bool lista_insere(Lista l, int pos, dado_t d)
   {
     // vê se inserção não é possível
     if (pos < 0 || pos > l->num || l->num >= l->cap) {
       return false;
     }
     // desloca dados para gerar espaço (movimenta as posições desde pos até o final da lista para a direita)
     // movimenta de trás pra diante para nao sobrescrever
     for (int i = l->num - 1; i > pos; i--) {
       l->vet[i] = l->vet[i-1];
     }
     // em vez do for, daria para usar uma função de movimentação de memória:
     // memmove(&l->vet[pos+1], &l->vet[pos], (l->num - pos)*sizeof(dado_t));
     // coloca o novo dado na posição e incrementa o número de dados
     l->vet[pos] = d;
     l->num++;
     return true;
   }
   
   bool lista_remove(Lista l, int pos)
   {
     // vê se existe alguém nessa pos
     if (pos < 0 || pos >= l->num) {
       return false;
     }
     // movimenta os dados após o removido
     for (int i = pos; i < l->num-1; i++) {
       l->vet[i] = l->vet[i+1];
     }
     l->num--;
     return true;
   }
```

A implementação de uma pilha com essa mesma estrutura seria um pouco mais simples:
```c
   // pilha.h
   #ifndef _PILHA_H_
   #define _PILHA_H_
   #include <stdbool.h>
   
   typedef struct _pilha *Pilha;
   
   Pilha pilha_cria(int cap);
   void pilha_destroi(Pilha p);
   
   // diz se a pilha está vazia
   bool pilha_vazia(Pilha p);
   
   // diz se a pilha está cheia
   bool pilha_cheia(Pilha p);
   
   // retorna o dado no topo da pilha, sem remover
   dado_t pilha_topo(Pilha p);
   
   // remove o dado no topo da pilha e retorna ele
   dado_t pilha_remove(Pilha p);
   
   // insere d na pilha
   void pilha_insere(Pilha p, dado_t d);

   #endif //_PILHA_H_
```
```c
   // pilha.c
   #include "pilha.h"
   #include <assert.h>
   
   typedef struct _pilha {
     int cap;
     int num;
     dado_t *vet;
   } pilha;
   
   Pilha pilha_cria(int cap)
   {
     // aloca memória para o descritor
     Pilha p = malloc(sizeof(pilha));
     if (p == NULL) return NULL;
     // aloca memória para os dados
     p->vet = malloc(cap * sizeof(dado_t));
     if (p->vet == NULL) {
       free(p);
       return NULL;
     }
     p->cap = cap;
     p->num = 0;
     return p;
   }
   
   void pilha_destroi(Pilha p)
   {
     // libera a memória dos dados e do descritor
     free(p->vet);
     free(p);
   }
   
   bool pilha_vazia(Pilha p)
   {
     return p->num == 0;
   }
   
   bool pilha_cheia(Pilha p)
   {
     return p->num == p->cap;
   }
   
   dado_t pilha_topo(Pilha p)
   {
     assert(!pilha_vazia(p));
     return p->vet[p->num-1];
   }
   
   dado_t pilha_remove(Pilha p)
   {
     assert(!pilha_vazia(p));
     return p->vet[--p->num];
   }
   
   void pilha_insere(Pilha p, dado_t d)
   {
     assert(!pilha_cheia(p));
     p->vet[p->num++] = d;
   }
```

#### Exercícios

1. Como seria a implementação de uma fila com a estrutura acima?
1. Faça um programa para testar a implementação de lista acima. Defina `dado_t` com `double`, por exemplo, inserindo `typedef double dado_t;` em `lista.h`. A implementação deve realizar diversas inclusões e remoções, inclusive incluindo dados quando a lista estiver cheia ou removendo quando tiver vazia. Conserte erros que encontrar (a lista foi implementada no editor, nunca sequer compilada).
2. Em algumas implementações, posições negativas são consideradas válidas, e representam os alementos em posições a partir do final da lista (-1 é o último elemento, -2 o penúltimo etc). Altere a implementação da lista para que funcione dessa forma. Em uma lista com 2 elementos, as posições com elementos são -2, -1, 0 e 1; as posições válidas para inserção são -3, -2, -1, 0, 1 e 2.
3. Altere a implementação de lista (só lista.c) para que tenha alocação dinâmica -- dobre a capacidade na inclusão quando tiver cheia, diminua a capacidade pela metade na remoção quando estiver ocupando menos de 1/3 da capacidade. Use o mesmo programa (sem alteração) para testar.
4. Altere de novo, para que seja circular, com alocação dinâmica. Teste novamente com o mesmo programa.
