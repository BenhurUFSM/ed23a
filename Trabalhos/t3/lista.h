#ifndef _LISTA_H_
#define _LISTA_H_

// lista
// -----
// TAD para uma lista linear simples, para armazenar ponteiros
//   genéricos (os dados mantidos pela lista são void *)
// 

typedef struct _lista *Lista;

// cria uma lista vazia
Lista lista_cria(void);

// libera a memória ocupada pela lista
void lista_destroi(Lista self);

// retorna o número de elementos armazenados na lista
int lista_nelem(Lista self);

// retorna o elemento que está na posição 'pos' da lista
// o primeiro elemento está na posição 0
// posições negativas são relativas ao final da lista (-1 é o último)
// retorna NULL se não houver dado na posição
void *lista_elem_pos(Lista self, int pos);

// insere um novo dado na lista, na posição 'pos'
// os dados que estavam nessa posição e adiante avançam uma posição
// 'pos' pode ser negativa (-1 corresponde a inserção no final)
// não faz nada se posição não for válida
void lista_insere_pos(Lista self, void *dado, int pos);

// insere dado no início da lista
void lista_insere_inicio(Lista self, void *dado);

// insere dado no final da lista
void lista_insere_fim(Lista self, void *dado);

// remove o dado na posição 'pos' da lista, e retorna o dado removido
// 'pos' é como em 'lista_elem_pos'
void *lista_remove_pos(Lista self, int pos);

// remove e retorna o primeiro dado da lista
// retorna NULL se lista estiver vazia
void *lista_remove_inicio(Lista self);

// remove e retorna o último dado da lista
// retorna NULL se lista estiver vazia
void *lista_remove_fim(Lista self);

// inicia um percurso na lista e retorna o primeiro elemento
// prepara o que for necessário para que uma chamada a lista_proximo
//   retorne o elemento seguinte
// retorna NULL se a lista estiver vazia
void *lista_percorre(Lista self);

// retorna o elemento seguinte ao retornado na última chamada a
//   lista_percorre ou lista_proximo
// retorna NULL se o último elemento retornado foi o último da lista
// o comportamento desta função só é garantido se não houver alteração
// na lista desde a última chamada a lista_percorre
void *lista_proximo(Lista self);

#endif // _LISTA_H_
