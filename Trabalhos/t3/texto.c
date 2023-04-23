#include "texto.h"
#include "lista.h"
#include <stdlib.h>

struct _txt {
  Lista linhas;   // lista com strings com cada linha do texto
  bool alterado;  // se o texto foi alterado depois de lido ou gravado
  Str nome;       // nome do arquivo que contém o texto
};

Txt txt_cria(void)
{
  Txt self = malloc(sizeof(*self));
  if (self == NULL) return NULL;
  self->linhas = lista_cria();
  self->alterado = false;
  self->nome = str_cria("sem_nome");
  return self;
}

void txt_destroi(Txt self)
{
  if (self == NULL) return;
  lista_destroi(self->linhas);
  str_destroi(self->nome);
  free(self);
}

Txt txt_cria_arq(Str nome)
{
  Txt self = txt_cria();
  if (self == NULL) return NULL;
  str_destroi(self->nome);
  self->nome = str_substr(nome, 0, str_tam(nome)); // nao tem str_copia
  char snome[str_numbytes(nome)+1];
  str_cstring(nome, snome);
  FILE *arq = fopen(snome, "r");
  if (arq != NULL) {
    Str linha;
    while ((linha = str_cria_linha(arq)) != NULL) {
      lista_insere_fim(self->linhas, linha);
    }
    fclose(arq);
  }
  return self;
  /* tem uma função padrão em stdlib (getline) que faz exatamente
   * o que é necessário nesta função...
   */
}

bool txt_alterado(Txt self)
{
  return self->alterado;
}

Str txt_nome(Txt self)
{
  return self->nome;
}

void txt_grava(Txt self)
{
  if (self == NULL) return;
  // abre o arquivo para escrita
  char snome[str_numbytes(self->nome)+1];
  str_cstring(self->nome, snome);
  FILE *arq = fopen(snome, "w");
  // pega cada string da lista e escreve em uma linha do arquivo
  Str linha;
  for (linha = lista_percorre(self->linhas);
       linha != NULL;
       linha = lista_proximo(self->linhas)) {
    str_grava(linha, arq);
    fputc('\n', arq);
  }
  fclose(arq);
  self->alterado = false;
}

int txt_nlin(Txt self)
{
  return lista_nelem(self->linhas);
}

Str txt_linha(Txt self, int lin)
{
  return lista_elem_pos(self->linhas, lin);
}

int txt_tamlin(Txt self, int lin)
{
  return str_tam(txt_linha(self, lin));
}

void txt_insere_char(Txt self, int lin, int col, chu ch)
{
  // cria uma str com a versão utf8 do caractere
  char chs[5];
  chu_para_utf8(ch, chs);
  Str ins = str_cria(chs);

  Str s = txt_linha(self, lin);
  if (s != NULL) {
    str_altera(s, col, 0, ins);
    str_destroi(ins);
    self->alterado = true;
  } else if (lin == txt_nlin(self)) {
    // inserção após o final do texto -- cria uma nova linha
    lista_insere_fim(self->linhas, ins);
    self->alterado = true;
  } else {
    // não deveria acontecer...
    str_destroi(ins);
  }
}

void txt_remove_char(Txt self, int lin, int col)
{
  Str s = txt_linha(self, lin);
  if (s == NULL) return;
  if (col < 0 || col > str_tam(s)) return;
  if (col < str_tam(s)) {
    // remoção no meio da linha -- troca o caractere por nada
    Str nada = str_cria("");
    str_altera(s, col, 1, nada);
    str_destroi(nada);
    self->alterado = true;
  } else {
    // remove além do final da linha -- concatena a próxima nesta
    // e apaga a próxima linha, se existir
    Str prox = txt_linha(self, lin+1);
    if (prox == NULL) return;
    str_altera(s, -1, 0, prox);
    lista_remove_pos(self->linhas, lin+1);
    str_destroi(prox);
    self->alterado = true;
  }
}

void txt_quebra_linha(Txt self, int lin, int col)
{
  Str s = txt_linha(self, lin);
  if (s != NULL) {
    // pega uma subrtring a partir de col
    Str sobra = str_substr(s, col, str_tam(s));
    // apaga o que foi pego da string original
    Str nada = str_cria("");
    str_altera(s, col, str_tam(s), nada);
    str_destroi(nada);
    // insere a nova linha após lin
    lista_insere_pos(self->linhas, sobra, lin+1);
    self->alterado = true;
  } else if (lin == txt_nlin(self)) {
    // inserção após o final -- insere uma linha em branco
    lista_insere_fim(self->linhas, str_cria(""));
    self->alterado = true;
  }
}
