# ed23a
Estruturas de Dados, UFSM, 1º semestre 2023

ELC1066 DLSC/CT/UFSM\
professor: Benhur Stein ([benhur+ed23a@inf.ufsm.br](mailto:benhur%2bed23a@inf.ufsm.br))\
segundas e quartas, 10h30, sala 334, prédio 7 (principal do CT)

## Notas

As notas estão na [planilha](https://docs.google.com/spreadsheets/d/1afGkzsOsM4dB0aYUoUyLZtTgvNbERXJFxhRhry5xpqc/edit?usp=sharing).
O exame será dia quarta 19jul, no horário e local das aulas.


## Desenvolvimento da disciplina

Os canais de comunicação entre o professor e os alunos são as aulas presenciais, esta página (<https://github.com/BenhurUFSM/ed23a>), e-mail (ver acima). Caso queira conversar fora desses canais, manda um mail que marcamos.

O e-mail pode ser usado para o envio de perguntas, exercícios, reclamações, sugestões, críticas e o que mais for, durante todo o período em que durar a disciplina.

Esta página concentrará os assuntos vistos em aula, a descrição de exercícios e trabalhos, avaliações, etc. Ela estará sendo aumentada/atualizada durante todo o semestre. Ela é o canal mais oficial de informações sobre a disciplina.

A disciplina será desenvolvida de uma forma bastante prática, os alunos devem desenvolver pequenos programas de computador, a serem enviados para o professor em datas definidas. 
Esses programas serão revisados e comentados pelo professor, individualmente ou de forma coletiva. 
Os programas podem ser corrigidos e reenviados quantas vezes o aluno quiser, para resolver dúvidas ou pedir sugestões.
A avaliação será feita no final do semestre, quando cada aluno deve entregar a versão final de um subconjunto dos trabalhos desenvolvidos durante o semestre e uma auto-avaliação justificada. Esse material final será usado pelo professor para gerar a nota final de cada aluno.
Para quem precisar, o exame será uma prova escrita.

##  Aulas

Os assuntos vistos em aula serão listados aqui.

|    N |  data | assunto
| ---: | :---: | :--------
|    1 | 20mar | introdução / revisão
|    2 | 22mar | revisão de C
|    3 | 27mar | [tipos abstratos de dados](Aulas/tad.md)
|    4 | 29mar | TAD, cont.
|    5 |  3abr | [listas lineares](Aulas/listas.md)
|    6 |  5abr | [listas encadeadas](Aulas/listas_encadeadas.md)
|    7 | 10abr | [listas encadeadas, alocação individual](Aulas/lista_encadeada_com_ponteiros.md)
|    8 | 12abr | [listas duplamente encadeadas, complementos](Aulas/lista_duplamente_encadeada.md)
|    9 | 17abr | [matrizes](Aulas/matrizes.md)
|   10 | 19abr | t2
|   11 | 24abr | Conjunto, Dicionário, Tabela de Dispersão (Hash)
|   12 | 26abr | t2
|   13 |  3mai | t2
|   14 |  8mai | [árvores](Aulas/arvores.md)
|   15 | 10mai | árvores
|   16 | 15mai | [árvores binárias de busca](Aulas/arvore_binaria_de_busca.md)
|   17 | 22mai | [arvores AVL](Aulas/arvore_avl.md)
|   18 | 24mai | [fila de prioridade, heap](Aulas/heap.md)
|   19 | 29mai | [grafos](Aulas/grafo.md)
|   20 | 31mai | [implementação de grafos, deteção de ciclos](Aulas/grafo2.md)
|   21 |  5jun | [ordem topológica](Aulas/grafo3.md)
|   22 |  7jun | [árvore geradora](Aulas/grafo4.md)
|   23 | 12jun | [caminho mais curto](Aulas/grafo5.md)
|   24 | 14jun | t4
|   25 | 19jun | t4
|   26 | 21jun | t4

## Exercícios e trabalhos

A descrição dos trabalhos a entregar será colocada aqui.

|     N |    prazo | descrição
| ----: | -------: | :-----------
|    t1 |      --- | [cálculo de notas](Trabalhos/t1.md) [[exemplo de implementação](Trabalhos/t1-ed23a.c)]
|    t2 |          | [TAD string](Trabalhos/t2.md)
|    t3 |     3mai | [TAD lista](Trabalhos/t3)
|    t4 |   **antes de** 10jul | [passeio na UFSM](Trabalhos/t4)<br>21jun - alteração do formato de saída

## Entrega Final

Para ser avaliado, o aluno deve realizar a entrega final de seus trabalhos e auto-avaliação, e comparecer a uma conversa com o professor, conforme descrito abaixo.

Devem ser entregues:
- a versão final dos trabalhos t3 e t4
- um texto com a auto-avaliação do aluno.

Crie um diretório (*folder*) chamado "ed-nome" ("nome" é uma string que permita identificar o aluno na chamada), coloque nele os arquivos a entregar (pode usar subdiretórios), faz um zip (ou tgz ou 7z etc). Anexa esse arquivo a um e-mail endereçado ao professor, enviado antes de 10 de julho. Se tiver preferência ou restrições sobre o horário da conversa, coloque no mail. Serão consideradas por ordem de chegada.

A conversa vai ser dia 10 ou 12 de julho, no horário da aula. Dia 10 antes da aula envio um mail com os horários.

A auto-avaliação deve conter uma avaliação (0 a 10) sobre o desenvolvimento (o quanto evoluiu no conteúdo, como foi no desenvolvimento dos trabalhos) do aluno durante a disciplina e outra (0 a 10 também) sobre o quanto o aluno considera que domina do conteúdo da disciplina. Deve ter um (pequeno) texto justificando esses números.
Pode também conter avaliação sobre como a disciplina foi conduzida pelo professor, sugestões, e o que mais o aluno considerar pertinente.

Para facilitar na quantificação do segundo número, você pode considerar como está o seu conhecimento nos assuntos abaixo (coloquei 10 itens, pontue cada um entre 0 e 1 e some):
1. tipos abstratos de dados (TAD)
1. como implementar TADs em C
2. uso de alocação dinâmica de memória
3. estruturas lineares (listas, filas, pilhas, deques)
4. implementação de TADs de estruturas lineares (alocação contígua, alocação encadeada simples ou dupla)
5. árvores
6. árvore binária de busca
7. implementação de árvore (encadeada, em heap)
8. grafos e sua implementação
9. algoritmos de grafos (ordem topológica, custo mínimo, caminho mais curto)


## Ementa

Ver [aqui](https://www.ufsm.br/ementario/disciplinas/ELC1066/).

## Bibliografia

Os livros abaixo estão disponíveis no [minha biblioteca](https://www.ufsm.br/orgaos-suplementares/biblioteca/e-books-2/), acessível aos alunos da UFSM.

Estruturas de Dados e Seus Algoritmos - Jayme Szwarcfiter e Lilian Markenzon

Estruturas de Dados - Nina Edelweiss e Renata Galante

Estruturas de Dados em C, Uma Abordagem Didática - Sílvio Pereira

Introdução a Estruturas de Dados - Com Técnicas de Programação em C - Waldemar Celes

Estrutura de Dados - Adriana de S. Vetorazzo, Mauício de O. Saraiva, Jeanine dos S. Barreto, Ramiro S. Córdova Jr.
