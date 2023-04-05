### Listas encadeadas

Nas implementações contíguas, a ordem física em que os dados da lista são mantidos em memória corresponde à ordem lógica desses dados na lista.
Sabendo a posição de um dos dados, com um cálculo simples se obtém a posição de qualquer outro.
Em compensação, se um dado muda sua posição lógica na lista (devido a inclusões ou remoções), os dados devem ser fisicamente movimentados na memória para que a correspondência seja mantida.

Uma outra forma de se organizar uma lista em memória é quebrando essa correspondência, e usando uma outra forma de se encontrar os dados da lista em vez de sua posição relativa na memória.
A forma mais comum de se fazer isso é manter, para cada dado na lista, uma informação que permite chegar ao dado que o sucede na lista.
Por exemplo, se os dados da lista são colocados em um vetor, a informação que permite chegar a um dado é a posição no vetor.
Uma forma de se manter essa posição é colocando a lista em dois vetores de mesmo tamanho, um contendo espaço para os dados e outro contendo a posição do sucessor de cada dado. A posição onde está o dado que é o sucessor do dado na posição 3 do vetor de dados é colocada na posição 3 do vetor de posições. 

Cada dado da lista pode estar em qualquer posição do vetor, então necessitamos saber a posição do primeiro dado da lista.
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
O primeiro dado da lista está na posição 2, informação dada por "prim". Nessa posição está o dado `X`.
No vetor de sucessores, na mesma posição do dado (2), tem o valor 3, que é a posição do dado seguinte (o segundo dado da lista).
Na posição 3, está o valor `Y` e o sucessor é 0.
Na posição 0 está o terceiro dado da lista, `Z` e o sucessor, -1. O valor -1, que não representa um índice válido nos vetores é um valor especial usado para representar que esse dado não tem sucessor, é o último da lista.

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
Tanto na inserção quanto na remoção, nenhum dado presente na lista precisou ser movimentado, o que é um dos motivos para se usar o encadeamento.

Claro, tem algumas desvantagens nessa implementação:
- é necessário mais memória, para colocar informação sobre os sucessores
- é necessário encontrar os espaços livres quando se tem uma inclusão. Uma forma de se fazer isso é organizar uma segunda lista no mesmo vetor, contendo os elementos livres, e retirar dela nas inserções, colocar nela nas remoções
- para implementar inclusão e remoção, tem que alterar a informação de sucessor do dado na posição anterior ao ponto de inserção/remoção; para encontrar esse dado temos que percorrer a lista desde o início.
- caso se use realocação de memória que libera espaço quando a área ocupada pela lista é muito inferior à capacidade, pode ser necessário deslocar dados que estejam na área que será liberada (já que um dado pode estar em qualquer lugar, não se tem garantia de que não tenha dado no final da área alocada), além de rearranjar a lista de livres.

#### Código visto em aula

Vimos em aula (5abr) a implementação de operações de inicialização e inserção em uma lista encadeada com dois vetores, nos quais são mantidas duas listas, a lista propriamente dita e a lista de locais livres.
Abaixo está o código visto em aula, com algumas alterações para facilitar a leitura.

```c
typedef struct {
  int prim;         // índice do vetor onde está o primeiro dado da lista
  int livre;        // índice do vetor onde está o primeira pos. livre
  dado_t dados[N];  // vetor que contém os dados da lista
  int prox[N];      // vetor que contém os encadeamentos
} *Lista;

// prox[i] contém o índice do vetor onde está o dado que segue o
//         dado que está no índice i do vetor

// inicializa a lista.
// a lista fica vazia (prim é -1, dizendo que não tem um índice válido
//   que armazene o primeiro dado da lista)
// todos as posições do vetor são colocadas na lista de livres
void init(Lista l)
{
  l->prim = -1;

  l->livre = 0;
  for (int i=0; i<N-1; i++) l->prox[i] = i+1;
  l->prox[N-1] = -1;
}

// encontra um índice livre, e retira da lista de livres
static int ind_livre(Lista l)
{
  if (cheia(l)) erro(); // poderia realocar...
  // pega o índice do primeiro da lista de livres
  int ind = l->livre;
  // o primeiro livre passa a ser o seguinte
  l->livre = l->prox[ind];
  return ind;
}

// insere o dado d no início da lista l
void insere_inicio(Lista l, dado_t d)
{
  // acha um índice livre
  int novo = ind_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // o dado que segue o novo primeiro dado é o antigo primeiro
  l->prox[novo] = l->prim;
  // atualiza a nova primeira posição
  l->prim = novo;
}

// insere o dado d no final da lista l
void insere_fim(Lista l, dado_t d)
{
  // se a lista estiver vazia, é mais fácil inserir no início...
  if (vazia(l)) { insere_inicio(l, d); return; }
  // acha um índice livre
  int novo = ind_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // ele é o novo último
  l->prox[novo] = -1;
  // acha a posição do último (vai virar penúltimo)
  int ult;
  for (ult = l->prim; l->prox[ult] != -1; ult = l->prox[ult])
    /*nada*/;
  // o dado que segue o antigo último é o novo último
  l->prox[ult] = novo;
}

// insere o dado d na lista l, de forma que ele fique na posição p
void insere_pos(Lista l, dado_t d, int p)
{
  // se a inserção for no início, temos uma função pronta...
  if (p == 0) { insere_inicio(l, d); return; }
  // acha um índice livre
  int novo = ind_livre(l);
  // coloca o dado no vetor
  l->dados[novo] = d;
  // acha a posição do dado que deve anteceder o novo dado
  // caso a lista não tenha elementos suficientes, acha o último
  int ant = l->prim;  // índice de um dado no vetor
  int pos_ant = 0;    // posição na lista correspondente a esse dado
  while (pos_ant < p-1) {
    if (l->prox[ant] == -1) {
      // chegamos no último da lista!
      break;
    }
    ant = l->prox[ant];
    pos_ant++;
  }
  // o índice do dado que está depois do anterior
  int dep = l->prox[ant];
  // o dado novo fica após o anterior
  l->prox[ant] = novo;
  // depois do novo fica o que estava depois do anterior
  l->prox[novo] = dep;
}
```

#### Exercícios

Faça as funções de remoção, teste a lista.
