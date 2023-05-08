## Árvores

Uma árvore é uma estrutura de dados que organiza os dados que guarda de uma forma hierárquica, muito semelhante a uma hierarquia de diretórios e arquivos.
Cada dado em uma árvore é armazenado em um "nó".
Uma árvore é um conjunto de nós, com as seguintes características:
- se o conjunto de nós está vazio, a árvore é vazia;
- senão, um dos nós do conjunto é chamado de "raiz" da árvore, e os demais são divididos em subconjuntos disjuntos, cada um deles uma árvore, chamadas de subárvores da raiz.

Existem várias formas de se representar graficamente uma árvore, a mais comum é representar os nós como círculos, dentro dos quais está o valor do nó, o círculo que corresponde à raiz acima dos que correspondem às raízes de suas subárvores, com setas ligando a raiz da árvore às raízes das subárvores.

Alguns conceitos de árvores:
- filhos de um nó: raízes das subárvores da árvore que tem esse nó como raiz;
- nó raiz: nó da árvore que não tem pai (uma árvore tem um único nó raiz);
- nó folha ou nó externo: nó da árvore que não tem filho;
- nó interno: nó que tem filho;
- grau de um nó: número de filhos que o nó tem;
- grau de uma árvore: maior grau encontrado em um nó da árvore;
- caminho em uma árvore: sequência de nós (não repetidos) da árvore, onde dois nós adjacentes têm a relação pai-filho;
- comprimento de um caminho: número de arestas em um caminho, um a menos que o número de nós do caminho (alguns autores consideram que o comprimento de um caminho é o número de nós do caminho, mas aparentemente esses autores são minoritários);
- profundidade de um nó: comprimento do caminho entre a raiz da árvore e esse nó;
- nível de uma árvore: conjunto de nós que estão na mesma profundidade; a raiz está no nível 0, seus filhos no nível 1, os filhos deles on nível 2, etc;
- altura de uma árvore: comprimento do maior caminho entre a raiz e alguma folha da árvore ou profundidade do nó mais profundo da árvore; uma árvore somente com o nó raiz tem altura 0; uma árvore vazia tem altura -1.

Quando a ordem dos filhos de um nó é importante, diz-se que a árvore é **ordenada**. Nesse caso, a árvore que tem a raiz A e os filhos B e C é considerada diferente da árvore que tem raiz A e filhos C e B. Se as árvores forem não ordenadas, essas duas árvores são consideradas iguais.

Nesta disciplina, nos concentraremos em um tipo especial de árvores, as árvores de grau 2, ou *árvores binárias*.

### Árvores binárias

Em uma árvore binária, cada nó tem no máximo 2 filhos. Nos restringiremos ao estudo de árvores ordenadas, então a ordem entre esses dois filhos é uma informação importante. Como são só dois filhos, eles são comumente chamados de filho esquerdo e direito.

Árvores binárias são estruturas definidas de forma recursiva, uma árvore binária pode ser:
- vazia; ou
- um nó, chamado de raiz e dois filhos, que são árvores binárias.

Uma forma comum de implementação de uma árvore é como um ponteiro para um nó, e esse nó contém o valor associado ao nó e dois ponteiros para os nós raiz das árvores filhas. Uma árvore vazia pode ser implementada como um ponteiro nulo ou como um nó cujo valor é ignorado e tem os dois ponteiros para nulo. Na primeira implementação, será alocado um nó para cada valor armazenado na árvore. Na segunda, será alocado, além desses, um nó a mais para cada filho vazio, duplicando o número de nós ocupados pela árvore, mas simplificando a interface de funções que alteram a árvore (veremos mais tarde na discussão sobre inserção e remoção).

```c
   typedef struct _no no_t;
   struct _no {
     dado_t val;
     no_t *esq;
     no_t *dir;
   };
   typedef no_t arv_t;  // um sinônimo, uma árvore é representada por um ponteiro para um nó
```

Grande parte dos algoritmos que operam sobre árvores são mais facilmente implementados em forma recursiva.
Por exemplo, para calcular quantos nós tem uma árvore, pode-se usar uma definição recursiva:
   - uma árvore vazia tem 0 nós
   - uma árvore não vazia tem um nó a mais que a soma dos nós de suas duas subárvores.
Em código C:
```c
   int num_nos(arv_t *a)
   {
      if (vazia(a)) return 0;
      return 1 + num_nos(a->esq) + num_nos(a->dir);
   }
```
Para calcular a altura de uma árvore:
   - uma árvore vazia tem altura -1
   - uma árvore não vazia tem altura que é um além da maior altura entre as árvores filhas.
Em código C:
```c
   int altura(arv_t *a)
   {
      if (vazia(a)) return -1;
      return 1 + maior(altura(a->esq), altura(a->dir));
   }
```
A implementaçã dessas funções de forma não recursiva é um tanto mais trabalhosa.

A função que testa se uma árvore é vazia testa se o ponteiro é NULL no caso de se implementar ávore vazia como ponteiro NULL ou testa se os ponteiros esq e dir são NULL, se for essa a implementação escolhida.

#### Exercícios

1. Faça uma função que recebe uma árvore e retorna quantos nós folha ela tem
2. Faça uma função que recebe uma árvore e retorna quantos nós não folha ela tem
3. Faça uma função que recebe uma árvore e um valor e retorna um booleano que diz se a árvore contém ou não um nó com esse valor. Use a função auxiliar `bool valores_iguais(dado_t, dado_t)` para comparar dois valores.
4. Faça uma função que recebe uma árvore e um valor e retorna o nível nessa árvore que está o nó que contém esse valor, ou -1 se o valor não estiver em nenhum nó da árvore. A raiz de uma árvore está no nível 0, os filhos da raiz no nível 1 etc.
5. Faça uma função que recebe uma árvore e retorna o "fator de equilíbrio" do nó raiz dessa árvore. O "fator de equilíbrio" a ser considerado é a diferença entre a altura da subárvore esquerda e a da direita.

Para testar essas funções, pode fazer uma árvore que armazena dados inteiros, e uma função que aloca um nó e retorna um ponteiro para ele, recebendo como argumentos o valor do nó e os ponteiros para os dois filhos. Uma árvore com a raiz com valor 1, o filho esquerdo com valor 2 e o direito com o valor 3 poderia ser criada assim (com árvore vazia sendo representada como NULL):
```c
   arv_t *no(dado_t d, arv_t *esq, arv_t *dir)
   {
      arv_t *n = malloc(sizeof(arv_t));
      assert(n != NULL);
      n->val = d;
      n->esq = esq;
      n->dir = dir;
      return n;
   }
   //...
   arv_t *a = no(1, no(2, NULL, NULL), no(3, NULL, NULL));
```
Ou por exemplo assim, com a árvore vazia representada por um nó com ponteiros NULL:
```c
   arv_t *cria_arv_vazia(void)
   {
      no_t *n = malloc(sizeof(no_t));
      assert(n != NULL);
      // o valor nunca vai ser usado
      n->esq = NULL;
      n->dir = NULL;
      return n;
   }
   
   void troca_valor(no_t *a, dado_t d)
   {
      if (vazia(a)) {  // vai deixar de ser uma árvore vazia, coloca árvores vazias como filhos
        a->esq = cria_arv_vazia();
        a->dir = cria_arv_vazia();
      }
      a->val = d;
   }
   //...
   arv_t *a = cria_arv_vazia(); // ou poderia ser arv_cria(), pra ficar como os TAD vistos antes
   troca_valor(a, 1);
   troca_valor(a->esq, 2);
   troca_valor(a->dir, 3);
```


### Percurso em uma árvore

Um percurso em uma árvore é uma forma de caminhamento entre os nós da árvore, que visita todos os nós.
Dependendo da ordem em que os nós são visitados, tem-se percursos diferentes.
Os principais tipos de percurso em uma árvore são:
- percurso em largura: visita os nós, a partir da raiz, um nível por vez (primeiro visita a raiz, depois todos seus filhos, depois todos seus netos etc);
- percurso em profundidade: visita os nós, percorrendo um ramo até seu final antes de seguir pelo ramo seguinte. Tem três percursos em profundidade principais:
   - percurso em pré-ordem: visita primeiro um nó, antes de realizar um percurso em pré-ordem iniciando em cada um de seus filhos;
   - percurso em pós-ordem: percorre em pós-ordem cada um dos filhos de um nó antes de visitar o nó;
   - percurso em-ordem (somente em árvores binárias): percorre em-ordem a subárvore esquerda, então visita o nó, e depois percorre em-ordem a subárvore direita.

Esse percursos podem ainda ser subclassificados em "à esquerda" e "à direita", de acordo com a ordem que os filhos são percorridos.

Esses percursos são tipicamente implementados por funções recursivas (exceto o percurso em largura), como abaixo:
```
   pre_ordem(arvore a)
   {
      se a não está vazia:
         visita(a)
         para cada sa, subárvore de a:
            pre_ordem(sa)
   }
```
No caso de uma árvore binária implementada como acima, um percurso em-ordem à esquerda poderia ser, em C:
```c
   void em_ordem(arv_t *a)
   {
     if (vazia(a)) return;
     em_ordem(a->esq);
     visita(a);
     em_ordem(a->dir);
   }
```

Em vez de uma função recursiva, pode-se implementar o percurso em profundidade com uma função não recursiva e a ajuda de uma pilha. O percurso em pré-ordem é o mais simples de ser implementado dessa forma:
```c
   void pre_ordem(arv_t *a)
   {
     if (vazia(a)) return;
     Pilha p = pilha_cria();
     pilha_insere(p, a);
     while (!pilha_vazia(p)) {
       a = pilha_remove(p);
       visita(a);
       if (!vazia(a->dir)) pilha_insere(p, a->dir);
       if (!vazia(a->esq)) pilha_insere(p, a->esq);
     }
     pilha_destroi(p);
   }
```

O percurso em largura é tipicamente implementado com o auxílio de uma fila:
```
   largura(arvore a)
   {
      fila f
      se a não está vazia:
         insere(f, a)
      enquanto f não estiver vazia:
         arvore b = remove(f)
         visita(b)
         para cada sa, subárvore de b:
            insere(f, sa)
   }
```

Considere a árvore descrita abaixo ('/' representa um filho inexistente; quem não tá na coluna da esquerda é folha):
|  nó | esq | dir |
| :---: | :---: | :---: |
|  a  | b   | c   |
|  b  | d   | e   |
|  c  | /   | f   |
|  d  | g   | /   |
|  e  | i   | j   |
|  f  | h   | k   |

Em alguns dos percursos comuns, os nós seriam visitados na ordem abaixo:
| percurso | ordem da visita |
| :--- | :---:
| pré-ordem à esquerda | a b d g e i j c f h k |
| pré-ordem à direita  | a c f k h b e j i d g |
| em-ordem à esquerda  | g d b i e j a c h f k |
| em largura           | a b c d e f g i j h k |

### Exercícios

Implemente a árvore acima e funções para realizar os percursos mostrados.
