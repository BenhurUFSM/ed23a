## Matrizes

Matrizes são vetores multidimensionais. 
Em algumas linguagens, vetores e matrizes só podem ser declarados com tamanho conhecido em tempo de compilação.
Essa restrição pode ser muito limitante em várias situações mais dinâmicas, em que o programa só sabe das suas necessidades em tempo de execução.

Em outras linguagens, o tamanho de vetores e matrizes pode ser definido em tempo de execução, mas não pode ser alterado uma vez declarados.
A linguagem C, a partir da padronização C99 permite esse tipo de declaração, como abaixo:
```c
  int nl, nc;
  scanf("%d %d", &nl, &nc);
  double mat[nl][nc]; // uma matriz assim não pode ser inicializada com "= { ... }"
```

Algumas linguagens têm vetores e matrizes redimensionáveis.
Não é o caso de C. No caso de vetores, é simples de implementar com alocação e realocação dinâmica, como já vimos.
No caso de matrizes, a alocação dinâmica é um tanto menos simples, e tem várias possibilidades, todas com vantagens e desvantagens em relação às demais.

Quando o número de colunas é fixo e conhecido em tempo de compilação, dá para usar um método semelhante ao usado para vetores, considerando que um vetor pode ser implementado por um ponteiro para um elemento, e os demais elementos seguem nas posições seguintes da memória:
```c
   tipo *p;
   p = malloc(n*sizeof(tipo));
   // p[i] é um valor do tipo "tipo" 
```
Declarando "tipo" como um inteiro, podemos tratar "p" como um vetor de inteiros:
```c
   typedef int tipo;            // o tipo "tipo" é um inteiro
   tipo *p;                     // "p" é um ponteiro para inteiro
   p = malloc(n*sizeof(tipo));  // "p" aponta para o primeiro de "n" inteiros
   // p[i] é um dos inteiros apontados por "p", um valor do tipo "int" 
```
Se o "tipo" for declarado como um vetor de inteiros, podemos tratar "p" como um vetor de vetores de inteiros:
```c
   typedef int tipo[100];       // o tipo "tipo" é um vetor de 100 inteiros
   tipo *p;                     // "p" é um ponteiro para vetor de 100 inteiros
   p = malloc(n*sizeof(tipo));  // "p" aponta para o primeiro de "n" vetores de 100 inteiros
   // p[i] é um dos vetores apontados por "p", um valor do tipo "vetor de 100 int"
   // p[i][j] é um dos inteiros desse vetor, um valor do tipo "int"
```

Uma matriz implementada dessa forma pode ser redimensionada no número de linhas, da mesma forma que um vetor alocado dinamicamente pode ser redimensionado.
Nessa implementação, todos os dados da matriz ficam contíguos em memória, o que geralmente melhora o desempenho no acesso, por não necessitar acessar um ponteiro intermediário (como é o caso da próxima solução), e por nao impor sobrecarga extra no sistema de memória *cache*, que geralmente acontece quando os dados da matriz estão mais espalhados pela memória.
Por outro lado, caso seja necessário suporte ao redimensionamento das duas dimensões, esse método não funciona.

Uma forma usual de se implementar uma matriz bidimensional em C, que pode ser livremente redimensionada é usar um vetor dinâmico de ponteiros para vetores dinâmicos. Cada linha de valores é alocada independentemente, e pode ser realocada quando necessário, e a matriz é representada por um vetor de ponteiros para essas linhas. Em código, a alocação de uma matriz de *m*x*n* *double*s poderia ser assim:
```c
   double **mat;
   mat = malloc(m * sizeof(double *)); // aloca vetor com m ponteiros
   for (int i=0; i<m; i++) {
     mat[i] = malloc(n * sizeof(double)); // aloca espaço para a i-ésima linha
   }
   m[3][4] = 23; // por causa da sintaxe de acesso a ponteiros, 
                 //  a sintaxe de acesso aos elementos da matriz é igual à de matrizes "normais"
```
Essa forma suporta inclusive que linhas diferentes tenham tamanhos diferentes, permitindo, por exemplo, a implementação de matrizes triangulares. A grande desvantagem, além da complexidade de alocação e liberação da memória (e o gasto de tempo e memória que isso envolve), está no custo em desempenho, pela necessidade de acesso ao ponteiro de linha e à sobrecarga causada ao sistema de memória *cache*.

Uma forma parecida, mas que diminui as desvantagens de número de alocações e liberações de memória, além de manter os dados contíguos é fazer a alocação de todos os dados em um único bloco de memória, e apontar os ponteiros de linha para posições no interior desse bloco:
```c
   double **mat;
   mat = malloc(m * sizeof(double *)); // aloca vetor com m ponteiros
   mat[0] = malloc(m * n * sizeof(double)); // aloca espaço para todos os dados da matriz
   for (int i=1; i<m; i++) {
     mat[i] = mat[0] + i * n; // aponta o i-ésimo ponteiro para o início da linha no bloco
   }
   m[3][4] = 23; // por causa da sintaxe de acesso a ponteiros, 
                 //  a sintaxe de acesso aos elementos da matriz é igual à de matrizes "normais"
```

Uma outra forma de alocação da matriz é realizar a alocação de todos os elementos como um vetor, e acessar os elementos calculando o índice nesse vetor à partir dos número de linha e coluna que se quer acessar. Com isso, evita-se o acesso a um vetor intermediário, mas em compensação perde-se a sintaxe de matriz para o acesso aos elementos, em geral substituindo-se por um acesso através de funções:
```c
   double *mat;
   mat = malloc(m * n * sizeof(double)); // aloca espaço para todos os dados da matriz
   poe_na_matriz(m, 3, 4, 23);
   // ou
   mat[ind(3, 4)] = 23;
   
   // o calculo do indice seria algo como:
int ind(int lin, int col) {
  return mat + lin*n + col;
}
   // ou, se tem a matriz tem um descritor:
int ind(matriz *mat, int lin, int col) {
  return mat->dados + lin*mat->n_col + col;
}
```

### Matriz esparsa

Uma matriz é dita esparsa quando contém a grande maioria de seus elementos com o mesmo valor, geralmente 0.
Grandes matrizes esparsas são comuns em áreas como simulação em matemática e engenharia, e em alguns casos de processamento de imagens.
Não é incomum que essas matrizes sejam grandes a ponto de impossibilitar a implementação de algoritmos na memória disponível, e a grande maioria dessa memória é usada para armazenar zeros. 
Uma estrutura de dados otimizada poderia armazenar somente os valores diferentes de zero, potencialmente economizando grande quantidade de memória.

Claro que, além dos valores não-zero, necessita-se de mais informação, para localizar eses valores em suas posições corretas na matriz.
Várias formas de representação de matrizes esparsas são usadas.

Uma delas é armazenar os valores não zero em uma estrutura de busca, usando as coordenadas (linha e coluna) do valor como chave.
Para acessar um valor da matriz, busca-se pelas coordenadas na estrutura, e se não for encontrado, o valor é considerado zero.

Outra ideia é armazenar com listas, uma para cada linha (ou coluna), contendo os elementos não zero dessa linha, cada um com a coluna respectiva. O acesso a um elemento da matriz consiste em buscar pela coluna na lista correspondente à linha.
Ordenando-se por coluna dentro da lista a busca pode ser otimizada. Mais ainda se a lista for implementada como um vetor, em que se pode fazer uma busca binária.

Uma outra forma é armazenar a matriz em 3 vetores do mesmo tamanho, um deles contendo todos os elementos não zero, outro contendo a coluna correspondente a cada valor e o terceiro contendo a linha. Se os dados forem armazenados nesses vetores em ordem de linha (todos os dados da primeira linha seguidos pelos da segunda etc), o vetor que contém as informações de linha vai conter vários valores repetidos seguidos (tantos quantos forem os dados não zero contidos em uma linha). Pode-se comprimir essa informação, e chega-se ao formato de matriz esparsa conhecido como "Yale".

Nesse formato, usa-dois vetores, um com os dados não zero e outro com o número da coluna de cada dado (esse vetores têm tamanho correspondente ao número de elementos não zero na matriz) e um terceiro vetor, contendo a posição nos vetores anteriores onde está o primeiro elemento de cada linha (ou onde estaria, caso essa linha não tenha nenhum elemento).
Esse terceiro vetor contém um valor a mais no final, que contém a posição onde estaria o primeiro elemento da linha seguinte, caso ela existisse (esse valor corresponde ao número de elementos não nulos). Esse vetor tem, portanto, tamanho correspondente ao número de linhas na matriz, mais um.

Por exemplo, abaixo está uma matriz esparsa 5x10 com 7 elementos não zero (os zeros não estão representados) e os 3 vetores usados para representá-la.
```
     0 1 2 3 4 5 6 7 8 9
   0       2       1
   1     4             9
   2 
   3           3     
   4 7           8
   
   val 2 1 4 9 3 7 8    <- os elementos não zeros, em ordem de linha
   col 3 7 2 9 5 0 6    <- a coluna onde cada elemento do vetor anterior está
   lin 0 2 4 4 5 7      <- o índice nos vetores anteriores onde está o 1o elemento de cada linha
```
Para encontrar um elemento (digamos, na posição *<i,j>*) da matriz nesses vetores, inicialmente descobre-se os índices dos vetores `val` e `col` que contêm os elementos da linha desejada. O vetor `lin` contém o índice do primeiro elemento de cada linha, então o primeiro índice sai direto desse vetor. O último índice é logo antes de onde está o primeiro da linha seguinte, que também está nesse vetor.
```c
   tipo valor_na_pos(int i, int j)
   {
      tipo dado = 0; // dado a ser retornado, inicializado em 0 para o caso de nao ser encontrado
      int ii = lin[i];    // onde começa a linha desejada
      int is = lin[i+1];  // onde começa a linha seguinte
      int p;
      for (p = ii; p < is; p++) {  // procura nessas posições
        if (col[p] == j) {  // achei!
          dado = val[p];
          break;
        }
        if (col[p] > j) break; // as colunas estão em ordem
      }
      return dado;
    }
```
Quando atribui um valor a uma posição da matriz, quatro situações podem acontecer:
- o valor é zero, e a posição não é encontrada - deixa assim, posição não encontrada representa valor zero;
- o valor é zero, e a posição é encontrada - remove o valor da matriz;
- o valor não é zero, e a posição não é encontrada - insere o valor na matriz;
- o valor não é zero e a posição é encontrada - substitui o valor no vetor `val`.

A atribuição pode ser implementada com código semelhante ao acima (até antes do `return`). No final do laço, `p` contém a posição nos vetores `val` e `col` do dado a ser removido ou onde o dado deve ser inserido. Os vetores devem ser aumentados ou diminuídos e os valores dessa posição até o final devem ser movimentados para a posição seguinte ou anterior. Os valores do vetor `lin` em todas as posições à partir de `i+1` devem ser decrementados ou incrementados de 1.
A decisão sobre o que fazer pode ser tomada analisando o valor de `dado` e o valor a atribuir, de acordo com as situações acima.

Uma implementação equivalente poderia ser feita armazenando por coluna em vez de linha.

Em geral, as principais operações com matrizes (soma, multiplicação entra matrizes ou entre matrizes e escalares ou vetores, etc) são implementadas de forma otimizada levando em consideração a forma como as matrizes são armazenadas, em vez de acessá-las como se fossem não esparsas, por questões de desempenho. Dependendo da forma de representação das matrizes esparsas, essas operações podem ser bastante otimizadas (ou não). Em geral essa é uma informação importante na hora de se escolher a representação mais adequada ao problema que se tem.
