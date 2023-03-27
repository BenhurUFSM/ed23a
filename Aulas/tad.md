## Tipos abstratos de dados (TADs)

Veja (e leia) a seção 2.2 do livro da Nina Edelweiss e da Renata Galante e/ou o capítulo sobre TAD no livro da Adriana Vetorazzo etc.


### Implementação de TAD em C

Um tipo abstrato de dados é constituído por:
- descrição do conjunto de valores que um dado desse tipo pode ter
- as operações que podem ser realizadas sobre dados desse tipo.

É importante ter uma separação entre a implementação de um TAD e seu uso.
Para que essa separação seja possível, se necessita de uma boa definição da interface entre essas partes.

Na disciplina, iremos implementar TADs de acordo com a descrição abaixo.

Deve ser escolhido um nome que identifique o TAD (aqui referenciado como *tad*).
Cada TAD será implementado em dois arquivos que só serão usados para esse fim.
A interface pública do TAD (a declaração do tipo, a declaração das funções que implementam as operações públicas sobre dados desse tipo, constantes necessárias para uso desse tipo etc) será colocada em um arquivo de cabeçalho chamado "*tad*.h".

Todo TAD será implementado como uma struct, de conteúdo só conhecido pela implementação do TAD. Os clientes do TAD (programas que usam o TAD) sempre acessam o TAD através de ponteiros.
Para que o tipo de dados seja opaco aos clientes (eles não tenham acesso aos dados internos que constituem o TAD), nosso TAD será declarado no .h como
```c
   typedef struct _tad *Tad;
```
Os clientes usarão o tipo *Tad* (e não a *struct _tad*) para declarar suas variáveis.

Todos os nomes de funções que implementam as operações do tipo *tad* serão prefixadas com *tad_*.
Com excessão de funções de construção (com nome *tad_cria\**), todas têm um primeiro argumento do tipo *Tad*, que é um ponteiro para o TAD que será operado pela função.

A implementação do TAD será feita em um arquivo chamado "*tad*.c". Esse arquivo deve incluir o arquivo "*tad*.h" no seu início.
Esse arquivo deve definir a *struct _tad* e implementar todas as funções declaradas em *tad*.h.
Caso implemente alguma outra função (em geral alguma função auxiliar), devem ser declaradas como *static*, para que não seja exportada e usada em outro arquivo.

Em um .c que seja cliente (que use) o TAD, deve haver a inclusão do arquivo *tad*.h, e o cliente só tem o direito de usar os tipos e funções ali declarados para operar sobre um dado desse tipo.

Todos os TADs terão funções para a criação e destruição (construtores e destrutores) de uma variável do tipo, com os nomes *tad_cria()* e *tad_destroi()*.
A primeira recebe argumentos necessários à criação de uma variável, aloca memória para essa variável, inicializa essa memória e retorna um ponteiro para a variável criada.
A segunda recebe um ponteiro para a variável a ser destruída, realiza alguma operação de finalização do dado se for necessário, libera a memória ocupada pelo dado.

Todo TAD deve ter uma função *tad_teste*, para testar o funcionamento do TAD. Essa função deve ser a primeira a ser escrita.

### Exemplo de implementação de um TAD

Para exemplificar, vamos ver como poderia ser a implementação de um TAD para representar datas, de acordo com essas considerações.
O nome escolhido para o TAD é *data*.
As operações a realizar sobre dados desse tipo (é só um exemplo, poderia ter sido escolhido outro conjunto de operações):
- criação de uma data, fornecendo dia, mês, ano
- destruição de uma data
- obtenção do dia da data
- obtenção do mês da data
- obtenção do ano da data
- cálculo do número de dias entre duas datas
- obtenção de uma data a tantos dias de distância da outra

O arquivo `data.h`:
```c
   #ifndef _DATA_H_    // para evitar a inclusão múltipla deste arquivo
   #define _DATA_H_
   
   // o tipo de dados Data
   typedef struct _data *Data;
   
   // criação de uma data
   Data data_cria(int dia, int mes, int ano);
   
   // destruição de uma data
   void data_destroi(Data d);
   
   // obtenção do dia da data
   int data_dia(Data d);
   
   // obtenção do mês da data
   int data_mes(Data d);
   
   // obtenção do ano da data
   int data_ano(Data d);
   
   // cálculo do número de dias entre duas datas
   int data_dias_ate(Data d1, Data d2);
   
   // obtenção de uma data a tantos dias de distância da outra
   Data data_apos_dias(Data d, int dias);
   
   #ifdef TESTE
   void data_teste(void);
   #endif
   
   #endif  // _DATA_H_
```
O arquivo `data.c`
```c
   #include "data.h"
   #include <stdbool.h>
   #include <stdlib.h>
   
   typedef struct _data {
     int dia;
     int mes;
     int ano;
   } data;
   
   #ifdef TESTE
   #include <assert.h>
   void data_teste(void)
   {
     Data d1;
     d1 = data_cria(29,2,2023);
     assert(d1 == NULL);
     d1 = data_cria(27,3,2023);
     assert(d1 != NULL);
     assert(data_dia(d1) == 27);
     assert(data_mes(d1) == 3);
     assert(data_ano(d1) == 2023);
     Data d2;
     d2 = data_apos_dias(d1,10);
     assert(d2 != NULL);
     assert(data_dia(d2) == 6);
     assert(data_mes(d2) == 4);
     assert(data_ano(d2) == 2023);
     assert(data_dias_ate(d1, d2) == 10);
     assert(data_dias_ate(d2, d1) == -10);
     data_destroi(d1);
     data_destroi(d2);
     // ... outros testes
   }
   #endif // TESTE
   
   // função auxiliar que retorna true se for data válida
   static bool data_ok(int d, int m, int a)
   {
     // testes para ver se d/m/a constituem uma data válida
     // ...
     return true; // ou nao
   }
   
   Data data_cria(int dia, int mes, int ano)
   {
     if (!data_ok(dia, mes, ano)) return NULL;
     Data d = malloc(sizeof(data));
     if (d != NULL) {
       d->dia = dia;
       d->mes = mes;
       d->ano = ano;
     }
     return d;
   }
   
   // destruição de uma data
   void data_destroi(Data d)
   {
     free(d);
   }
   
   // obtenção do dia da data
   int data_dia(Data d)
   {
     return d->dia;
   }
   
   // obtenção do mês da data
   int data_mes(Data d)
   {
     return d->mes;
   }
   
   // obtenção do ano da data
   int data_ano(Data d)
   {
     return d->ano;
   }
   
   // cálculo do número de dias entre duas datas
   int data_dias_ate(Data d1, Data d2)
   {
     if (d1->mes == d2->mes && d1->ano == d2->ano) {
       return d2->dia - d1->dia;
     }
     // ...
   }
   
   // obtenção de uma data a tantos dias de distância da outra
   Data data_apos_dias(Data d, int dias)
   {
     return data_cria(d->dia + dias, d->mes, d->ano); // talvez necessite refinamentos
   }
```
Um possível cliente:
```c
   #include "data.h"
   #include <stdio.h>
   
   int main()
   {
     Data hoje, amanha;
     
     hoje = data_cria(6, 5, 2022);
     amanha = data_apos_dias(hoje, 1);
     printf("Amanhã será %02d/%02d/%04d.\n", data_dia(amanha), data_mes(amanha), data_ano(amanha));
     
     data_destroi(hoje);
     data_destroi(amanha);
     
     return 0;
   }
```

Um programa de teste do TAD (teste de unidade) poderia ser:
```c
   #include "data.h"
   
   int main()
   {
     data_teste();
   }
```
Compila esse programa com (o *-DTESTE* serve para definir a palavra TESTE durante a compilação para incluir o código opcional delimitado por *#ifdef TESTE*):
```
   gcc -DTESTE -o teste teste.c data.c
```
