#include "lista.h"
#include <stdlib.h>

struct _lista {
  void *dados[50];
  int ndados;
  int pos_percurso;
};

Lista lista_cria(void)
{
  Lista self = malloc(sizeof(*self));
  if (self == NULL) return NULL;
  self->ndados = 0;
  self->pos_percurso = 0;
  return self;
}

void lista_destroi(Lista self)
{
  if (self != NULL) {
    free(self);
  }
}

int lista_nelem(Lista self)
{
  if (self == NULL) return 0;
  return self->ndados;
}

void *lista_elem_pos(Lista self, int pos)
{
  if (self == NULL) return NULL;
  if (pos < 0) pos = self->ndados + pos;
  if (pos < 0 || pos >= self->ndados) return NULL;
  return self->dados[pos];
}

void lista_insere_pos(Lista self, void *dado, int pos)
{
  if (self == NULL) return;
  if (pos < 0) pos = self->ndados + pos + 1;
  if (pos < 0 || pos > self->ndados) return;
  if (self->ndados >= 50) return;
  for (int i = self->ndados; i > pos; i--) {
    self->dados[i] = self->dados[i-1];
  }
  self->dados[pos] = dado;
  self->ndados++;
}

void lista_insere_inicio(Lista self, void *dado)
{
  lista_insere_pos(self, dado, 0);
}

void lista_insere_fim(Lista self, void *dado)
{
  lista_insere_pos(self, dado, -1);
}

void *lista_remove_pos(Lista self, int pos)
{
  if (self == NULL) return NULL;
  if (pos < 0) pos = self->ndados + pos;
  if (pos < 0 || pos >= self->ndados) return NULL;
  void *dado = self->dados[pos];
  self->ndados--;
  for (int i = pos; i < self->ndados; i++) {
    self->dados[i] = self->dados[i+1];
  }
  return dado;
}

void *lista_remove_inicio(Lista self)
{
  return lista_remove_pos(self, 0);
}

void *lista_remove_fim(Lista self)
{
  return lista_remove_pos(self, -1);
}

void *lista_proximo(Lista self)
{
  if (self == NULL) return NULL;
  if (self->pos_percurso >= self->ndados) return NULL;
  return self->dados[self->pos_percurso++];
}

void *lista_percorre(Lista self)
{
  if (self == NULL) return NULL;
  self->pos_percurso = 0;
  return lista_proximo(self);
}
