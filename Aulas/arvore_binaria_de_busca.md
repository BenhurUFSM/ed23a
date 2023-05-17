### Árvores binárias de busca

Uma árvore binária de busca (ABB) é uma árvore binária, ordenada, com nós com valores comparáveis entre si.
Com "comparáveis entre si", quer dizer que eles têm uma ordem absoluta definida, ou seja, se A, B e C são valores que podem ser colocados na árvore, se A < B e B < C então A < C.
Os nós em uma ABB são organizados de forma que todos os nós da subárvore esquerda do nó que contém o valor V contêm valores menores que V, e todos os nós da subárvore direita do nó que contém V contêm valores maiores que V. Todas as subárvores de uma ABB são ABB.

A busca por um valor em uma ABB é rápida e simples: comparando-se o valor buscado com o valor de um nó, elimina-se da busca uma das subárvores desse nó (se o valor buscado é menor que o valor do nó, ele certamente não estará entre os valores da subárvore direita, que só contém valores superiores ao valor do nó. A busca só precisa percorrer um caminho na árvore para decidir se o valor procurado está ou não nessa árvore. No pior dos casos, percorre a altura da árvore. Se a árvore é equilibrada (as subárvores esquerda e direita de cada nó têm "pesos" semelhantes), a altura de uma árvore binária com *n* nós é próxima a *log<sub>2</sub>n*.

Um percurso em-ordem em uma ABB visitará os nós em ordem crescente. Pode-se usar uma ABB para ordenar um conjunto de dados: insere-se todos os dados em uma ABB inicialmente vazia, e depois percorre-se a árvore em-ordem.

#### Busca

A **busca** por um dado em uma ABB pode ser implementada por uma função como a abaixo, que retorna um ponteiro para o dado armazenado na árvore que compara como igual ao valor procurado, ou NULL:
```c
   dado_t *busca(arv_t *a, dado_t valor)
   {
     if (vazia(a)) {
       return NULL;               // o valor certamente não está em uma árvore vazia!
     } else if (iguais(valor, a->dado)) {
       return &(a->dado);         // achei!
     } else if (menor(valor, a->dado)) {
       return busca(a->esq, val); // valor pequeno, retorna o resultado da busca na subárvore esquerda
     } else {
       return busca(a->dir, val); // valor grande, continua pela direita
     }
   }
```
Uma versão não recursiva:
```c
   dado_t *busca(arv_t *a, dado_t valor)
   {
     while (!vazia(a)) {
       if (iguais(valor, a->dado)) {
         return &a->dado;   // achei!
       } else if (menor(valor, a->dado)) {
         a = a->esq;        // valor pequeno, continua a busca na subárvore esquerda
       } else {
         a = a->dir;        // valor grande, continua pela direita
     }
     return NULL;           // o valor certamente não está em uma árvore vazia!
   }
```

#### Inserção

Em uma árvore binária, pode-se considerar que todos os seus nós têm dois filhos, que podem ser uma ABB normal ou uma árvore vazia. Uma árvore binária com *n* nós não vazios tem *n+1* nós vazios. Quando se faz uma **inserção** em uma ABB, o novo valor irá ocupar uma dessas *n+1* posições vazias, e a decisão de qual delas depende exclusivamente do valor a ser inserido: tem uma para valores inferiores ao menor valor já presente na árvore, outra para um valor entre o menor e o segundo menor valores da árvore e assim por diante.
O algoritmo para escolher a posição de inserção é muito semelhante ao de busca (afinal, se esse dado já estivesse na árvore e fosse buscado, o algoritmo de busca deveria encontrá-lo).

Em outras palavras, em uma inserção uma árvore vazia será transformada em uma árvore com um só nó. Essa transformação tem consequências em como implementar a inserção, dependendo da forma como uma árvore vazia é implementada.
Caso a árvore vazia seja implementada como um ponteiro nulo, a inserção de um valor nessa árvore deve alterar esse ponteiro.
Se uma função de inserção tem a interface ```void insere(arv_t *a, dado_t valor)``` e `a` é uma árvore vazia, a função receberá em `a` um valor nulo, e não terá como alterar o valor do ponteiro que contém esse nulo, para que aponte para o novo nó. A função teria que ter uma interface alternativa, ou ela recebe um ponteiro para o ponteiro para poder alterá-lo (`void insere(arv_t **a, dado_t valor)`), ou ela retorna o valor com o ponteiro para o novo nó (`arv_t *insere(arv_t *a, dado_t valor)`). 
Quando se implementa a árvore vazia com um nó vazio, esse problema não existe, porque o ponteiro que representa a árvore já está apontando para um nó; esse nó é transformado em um nó folha (e cria-se mais dois nós vazios para serem seus filhos), e o ponteiro que representa a árvore não precisa ser alterado, continua apontando para o mesmo nó de antes da inserção. A desvantagem é que a árvore tem *2n+1* nós em vez de para *n* dados.

A figura abaixo mostra o estado de uma árvore vazia e da mesma árvore após a inserção de um valor, para o caso de implementação de árvore vazia com ponteiro nulo e com nó nulo.
![figura](https://github.com/BenhurUFSM/ed21a/blob/main/Complementos/ed21a-inser%C3%A7%C3%A3o%20em%20%C3%A1rvore.png)

As três implementações alternativas de inserção em uma ABB estão abaixo:
```c
    // inserção em uma ABB com árvore vazia sendo representada por um nó nulo
    void insere(arv_t *a, dado_t valor)
    {
       if (vazia(a)) {  // a->esq e a->dir são NULL
          a->dado = valor;
          a->esq = cria_no_vazio();
          a->dir = cria_no_vazio();
       } else if (iguais(valor, a->dado)) {
          ; // ou reaje de outra forma para inserção de valor já existente
       } else if (menor(valor, a->dado) {
          insere(a->esq, valor);   // valor pequeno, insere na subárvore esquerda
       } else {
          insere(a->dir, valor);   // valor grande, insere na subárvore direita
       }
    }
```
```c
    // inserção em uma ABB com árvore vazia sendo representada por ponteiro nulo
    //   versão com retorno da árvore
    arv_t *insere(arv_t *a, dado_t valor)
    {
       if (vazia(a)) {  // a == NULL
          a = cria_no(valor, NULL, NULL);
       } else if (iguais(valor, a->dado)) {
          ; // ou reaje de outra forma para inserção de valor já existente
       } else if (menor(valor, a->dado)) {
          a->esq = insere(a->esq, valor);   // valor pequeno, insere na subárvore esquerda
       } else {
          a->dir = insere(a->dir, valor);   // valor grande, insere na subárvore direita
       }
       return a;
    }
```
```c
    // inserção em uma ABB com árvore vazia sendo representada por ponteiro nulo
    //   versão com ponteiro para ponteiro
    void insere(arv_t **pa, dado_t valor)
    {
       if (vazia(*pa)) {  // *pa == NULL
          *pa = cria_no(valor, NULL, NULL);
       } else if (iguais(valor, (*pa)->dado)) {
          ; // ou reaje de outra forma para inserção de valor já existente
       } else if (menor(valor, (*pa)->dado)) {
          insere(&((*pa)->esq), valor);   // valor pequeno, insere na subárvore esquerda
       } else {
          insere(&((*pa)->dir), valor);   // valor grande, insere na subárvore direita
       }
    }
```
Dá para melhorar um pouco a sintaxe pesada usando variável auxiliar:
```c
    // inserção em uma ABB com árvore vazia sendo representada por ponteiro nulo
    //   versão com ponteiro para ponteiro
    void insere(arv_t **pa, dado_t valor)
    {
       arv_t *a;
       a = *pa;
       if (vazia(a)) {  // a == NULL
          a = cria_no(valor, NULL, NULL);
          *pa = a;
       } else if (iguais(valor, a->dado)) {
          ; // ou reaje de outra forma para inserção de valor já existente
       } else if (menor(valor, a->dado)) {
          insere(&(a->esq), valor);   // valor pequeno, insere na subárvore esquerda
       } else {
          insere(&(a->dir), valor);   // valor grande, insere na subárvore direita
       }
    }
```
Ou talvez com uma versão não recursiva:
```c
    // inserção em uma ABB com árvore vazia sendo representada por ponteiro nulo
    //   versão com ponteiro para ponteiro, não recursiva
    void insere(arv_t **pa, dado_t valor)
    {
       arv_t *a;
       a = *pa;
       while (!vazia(a)) {  // a == NULL
          if (iguais(valor, a->dado)) {
             return; // ou reaje de outra forma para inserção de valor já existente
          } else if (menor(valor, a->dado)) {
             pa = &(a->esq);   // valor pequeno, continua na subárvore esquerda
             a = *pa;
          } else {
             pa = &(a->dir);   // valor grande, continua na subárvore direita
             a = *pa;
             // por que não dá pra fazer   a = a->dir;  pa = &a;  ??
          }
       }
       a = cria_no(valor, NULL, NULL);
       *pa = a;
    }
```


#### Remoção

A **remoção** tem um problema semelhante à inserção, em que uma folha pode ser transformada em uma árvore vazia, exigindo a troca do ponteiro para nulo, no caso de implementação com ponteiros nulos. A solução é semelhante à da inserção.
Por ter um código mais limpo, vamor considerar a implementação com árvores vazias representadas como nó nulo. A implementação com ponteiro nulo fica com exercício.

A remoção tem três casos, um muito simples, um simples e um menos simples: 
- o caso muito simples é quando o dado a ser removido não está na árvore, não há nada a ser feito;
- o caso simples é quando o dado a ser removido está em um nó folha, basta transformá-lo em uma árvore vazia;
- o caso menos simples é quando o nó que contém o dado a ser removido é um nó interno da árvore, que contém filhos.

Vamos deixar o terceiro caso para mais tarde.

Nos demais casos, a remoção não é muito diferente da busca ou inserção:
```c
   void remove(arv_t *a, dado_t valor)
   {
      if (vazia(a)) {
         return;                     // arvore vazia não tem o dado, não faz nada
      } else if (iguais(valor, a->dado)) {
         if (folha(a)) {  // o caso fácil, transforma a em árvore vazia
            a->dado = NAO_TEM_DADO_AQUI;
            free(a->esq);  // libera o nó vazio
            free(a->dir);  // o outro também
            a->esq = a->dir = NULL; // para reconhecermos como nó vazio mais tarde
         } else {  // o caso menos fácil, o nó tem filho(s)!
            // TODO
         }
      } else if (menor(valor, a->dado)) {
         remove(a->esq, valor);      // valor pequeno, remove da esquerda
      } else {
         remove(a->dir, valor);      // valor grande, remove da direita
      }
   }
```

O problema da remoção de um nó interno é o que fazer com os filhos do nó removido, eles devem permanecer na árvore.
Para evitar o caos, o que normalmente se faz é escolher um valor mais abaixo na árvore que possa ocupar o lugar do valor removido, e depois remover esse valor da subárvore onde ele se encontra. Como esse valor está mais baixo na árvore, deve chegar uma hora em que o velor a ser removido está em uma folha, e voltamos ao caso simples.
Para garantir que não sejam necessárias maiores alterações na árvore, deve-se escolher um valor substituto que mantém o restante da árvore correta em relação às restrições de uma ABB (todos os valores na subárvore direita do nó alterado devem continuar maiores que o valor escolhido para esse nó, e os da esquerda devem ser menores). Dois valores na árvore são candidatos ideais: os dois vizinhos do valor removido, considerando a ordem dos valores dos nós.

Por exemplo, se uma árvore contém os valores 1, 2, 3, 4, 5, 6, 7, 8, 9 e o valor 5 está na raiz, os valores 1, 2, 3, 4 estão na subárvore esquerda e os valores 6, 7, 8, 9 na subárvore direita. Se o valor 5 for removido, colocando-se o valor 4 ou 6 na raiz garante que os valores remanescentes na subárvore esquerda são todos menores que a nova raiz e os da direita maiores. Para qualquer outro valor escolhido para ocupar o lugar do 5 não se tem essa garantia. O valor a ser escolhido é o maior valor da subárvore esquerda ou o menos valor na subárvore direita. Precisamos de uma função para encontrá-los.

Não é complicado. Por exemplo maior valor em uma ABB é o maior valor da subárvore direita, a não ser que ela não exista, em que é o valor da raiz:
```c
   dado_t maior_valor(arv_t *a)
   {
      if (vazia(a)) {
         // temos um problema, esta função não deve ser usada em árvores vazias!
         badabum();
      }
      if (vazia(a->dir)) {
         return a->dado;
      }
      return maior_valor(a->dir);
   }
```

Agora podemos completar a remoção:
```c
   void remove(arv_t *a, dado_t valor)
   {
      if (vazia(a)) {
         return;                     // arvore vazia não tem o dado, não faz nada
      } else if (iguais(valor, a->dado)) {
         if (folha(a)) {  // o caso fácil, transforma a em árvore vazia
            esvazia_no(a);
         } else {  // o caso menos fácil, o nó tem filho(s)!
            dado_t novo_valor;  // este é o novo valor a colocar no nó
            if (!vazio(a->esq)) { // preferência pela esquerda, poderia ser pela direita, ou aleatório
               novo_valor = maior_valor(a->esq);
               remove(a->esq, novo_valor);
            } else {
               novo_valor = menor_valor(a->dir);
               remove(a->dir, novo_valor);
            }
            a->dado = novo_valor;
         }
      } else if (menor(valor, a->dado)) {
         remove(a->esq, valor);      // valor pequeno, remove da esquerda
      } else {
         remove(a->dir, valor);      // valor grande, remove da direita
      }
   }
```

Caso a remoção do dado seja algo mais complicado (por exemplo, se o dado é um ponteiro para uma estrutura complicada, que tem que ser liberada com cuidado), pode-se trocar o dado (novo_valor) pelo dado a remover (colocando o dado a remover mais abaixo na árvore), e continuar com a remoção do mesmo valor de antes:
```c
    ...
    no_t *no_menor = no_com_o_maior_valor(a->esq);
    troca_valores(a, no_menor);
    remove(a->esq, valor); // ou curto-circuita, e chama remove(no_menor, valor)
    ... 
```
