#ifndef _LISTA_H_
#define _LISTA_H_

typedef struct _lista *Lista;

Lista lista_cria(void);

void lista_destroi(Lista self);

int lista_nelem(Lista self);

void *lista_elem_pos(Lista self, int pos);

void lista_insere_pos(Lista self, void *dado, int pos);

void lista_insere_inicio(Lista self, void *dado);

void lista_insere_fim(Lista self, void *dado);

void *lista_remove_pos(Lista self, int pos);

void *lista_remove_inicio(Lista self);

void *lista_remove_fim(Lista self);

void *lista_percorre(Lista self);

void *lista_proximo(Lista self);

#endif // _LISTA_H_
