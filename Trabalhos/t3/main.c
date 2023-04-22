#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "tela.h"
#include "str.h"

typedef struct _ctrl *Ctrl;
Str ctrl_le_str(Ctrl self, char *msg);
void ctrl_msg(Ctrl self, char *msg);

static int min(int a, int b)
{
  return a<b ? a : b;
}

static int max(int a, int b)
{
  return a>b ? a : b;
}

static int limita(int a, int inf, int sup)
{
  return max(inf, min(a, sup));
}

typedef struct _lista *Lista;
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


typedef struct _txt *Txt;
struct _txt {
  Lista linhas;
  bool alterado;
  Str nome;
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
  char snome[str_numbytes(self->nome)+1];
  str_cstring(self->nome, snome);
  FILE *arq = fopen(snome, "w");
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
  char chs[5];
  chu_para_utf8(ch, chs);
  Str ins = str_cria(chs);
  Str s = txt_linha(self, lin);
  if (s != NULL) {
    str_altera(s, col, 0, ins);
    str_destroi(ins);
    self->alterado = true;
  } else if (lin == txt_nlin(self)) {
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
    // remove além do final da linha -- gruda a próxima
    Str prox = lista_elem_pos(self->linhas, lin+1);
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
    Str sobra = str_substr(s, col, str_tam(s));
    Str nada = str_cria("");
    str_altera(s, col, str_tam(s), nada);
    str_destroi(nada);
    lista_insere_pos(self->linhas, sobra, lin+1);
    self->alterado = true;
  } else if (lin == txt_nlin(self)) {
    lista_insere_fim(self->linhas, str_cria(""));
    self->alterado = true;
  }
}


typedef struct _jan *Jan;
struct _jan {
  int curlin;
  int curcol;
  int lin_topo;
  int col_esq;
  Txt txt;
};

Jan jan_cria(Txt txt)
{
  Jan self = malloc(sizeof(*self));
  if (self != NULL) {
    self->txt = txt;
    self->curlin = 0;
    self->curcol = 0;
    self->lin_topo = 0;
    self->col_esq = 0;
  }
  return self;
}

void jan_destroi(Jan self)
{
  if (self == NULL) return;
  free(self);
}

Txt jan_texto(Jan self)
{
  return self->txt;
}

int jan_plin_tela(Jan self)
{
  return 1;
}

int jan_ulin_tela(Jan self)
{
  return tela_nlin()-2;
}

int jan_nlin_tela(Jan self)
{
  return jan_ulin_tela(self) - jan_plin_tela(self) + 1;
}

int jan_pcol_tela(Jan self)
{
  return 0;
}

int jan_ucol_tela(Jan self)
{
  return tela_ncol()-1;
}

int jan_ncol_tela(Jan self)
{
  return jan_ucol_tela(self) - jan_pcol_tela(self) + 1;
}

int jan_clin_tela(Jan self)
{
  return self->curlin - self->lin_topo + jan_plin_tela(self);
}

int jan_ccol_tela(Jan self)
{
  return self->curcol - self->col_esq + jan_pcol_tela(self);
}

static void jan_ajusta_cursor(Jan self)
{
  // limita o cursor a regiões que contenham texto
  self->curlin = limita(self->curlin, 0, txt_nlin(self->txt));
  self->curcol = limita(self->curcol,
                        0, txt_tamlin(self->txt, self->curlin));
  // faz a janela conter o cursor
  self->lin_topo = limita(self->lin_topo,
                          self->curlin-(jan_nlin_tela(self)-1),
                          self->curlin);
  self->col_esq = limita(self->col_esq,
                         self->curcol-(jan_ncol_tela(self)-1),
                         self->curcol);
}

void jan_texto_para_linha(Jan self, int lin, int n, char s[n])
{
  s[0] = '\0';
  Str linha = txt_linha(self->txt, lin-jan_plin_tela(self)+self->lin_topo);
  if (linha == NULL) return;
  int nchar = jan_ucol_tela(self)-jan_pcol_tela(self);
  Str sub = str_substr(linha, self->col_esq, nchar);
  str_cstring(sub, s);
  str_destroi(sub);
}

void jan_desenha_txt(Jan self)
{
  jan_ajusta_cursor(self);
  int tam_lin = (jan_ucol_tela(self)-jan_pcol_tela(self))*4+1;
  char s[tam_lin];
  tela_cor_normal();
  for (int l = jan_plin_tela(self); l < jan_ulin_tela(self); l++) {
    jan_texto_para_linha(self, l, tam_lin, s);
    tela_lincol(l, jan_pcol_tela(self));
    tela_texto(s);
  }
  tela_lincol(jan_ulin_tela(self), jan_pcol_tela(self));
  tela_cor_selecao();
  tela_textof("L%d C%d", self->curlin+1, self->curcol+1);
  tela_cor_normal();
  tela_lincol(jan_clin_tela(self), jan_ccol_tela(self));
}

static void jan_trata_char(Jan self, chu tecla)
{
  txt_insere_char(self->txt, self->curlin, self->curcol, tecla);
  self->curcol++;
}

static void jan_enter(Jan self)
{
  txt_quebra_linha(self->txt, self->curlin, self->curcol);
  self->curlin++;
  self->curcol = 0;
}
static void jan_delete(Jan self)
{
  txt_remove_char(self->txt, self->curlin, self->curcol);
}

static void jan_backspace(Jan self)
{
  if (self->curcol > 0) {
    self->curcol--;
  } else {
    if (self->curlin > 0) {
      self->curlin--;
      self->curcol = txt_tamlin(self->txt, self->curlin);
    } else {
      return;
    }
  }
  jan_delete(self);
}

static void jan_trata_controle(Jan self, chu tecla)
{
  switch (tecla) {
    case c_up:
      self->curlin--;
      break;
    case c_down:
      self->curlin++;
      break;
    case c_left:
      self->curcol--;
      break;
    case c_right:
      self->curcol++;
      break;
    case c_enter:
      jan_enter(self);
      break;
    case c_home:
      self->curcol = 0;
      break;
    case c_end:
      self->curcol = INT_MAX;
      break;
    case c_pgdn:
      self->curlin += jan_nlin_tela(self) * 4/5;
      break;
    case c_pgup:
      self->curlin -= jan_nlin_tela(self) * 4/5;
      break;
    case c_back:
      jan_backspace(self);
      break;
    case c_del:
      jan_delete(self);
      break;
    default:
      break;
  }
}

void jan_trata_tecla(Jan self, chu tecla)
{
  if (tecla > 0) jan_trata_char(self, tecla);
  else jan_trata_controle(self, tecla);
  jan_ajusta_cursor(self);
}



typedef struct _ctrl *Ctrl;
struct _ctrl {
  Lista textos;
  Lista janelas;
  int pos_jan_atual;
  Jan jan_atual;
};

Ctrl ctrl_cria(void)
{
  Ctrl self = malloc(sizeof(*self));
  if (self != NULL) {
    self->textos = lista_cria();
    self->janelas = lista_cria();
    self->pos_jan_atual = 0;
    self->jan_atual = NULL;
  }
  return self;
}

void ctrl_destroi(Ctrl self)
{
  if (self != NULL) {
    Txt txt;
    while ((txt = lista_remove_inicio(self->textos)) != NULL) {
      txt_destroi(txt);
    }
    lista_destroi(self->textos);
    Jan jan;
    while ((jan = lista_remove_inicio(self->janelas)) != NULL) {
      jan_destroi(jan);
    }
    lista_destroi(self->janelas);
    free(self);
  }
}

void ctrl_grava(Ctrl self)
{
  txt_grava(jan_texto(self->jan_atual));
}

Txt ctrl_txt_com_nome(Ctrl self, Str nome)
{
  Txt t;
  for (t = lista_percorre(self->textos);
       t != NULL;
       t = lista_proximo(self->textos)) {
    if (str_igual(txt_nome(t), nome)) return t;
  }
  t = txt_cria_arq(nome);
  // insere o texto no final da lista
  lista_insere_fim(self->textos, t);
  return t;
}

void ctrl_abre_nome(Ctrl self, Str nome)
{
  Txt txt = ctrl_txt_com_nome(self, nome);
  if (txt == NULL) {
    ctrl_msg(self, "Erro na abertura do arquivo");
  } else {
    // cria uma nova janela para mostrar o texto
    Jan nova_jan = jan_cria(txt);
    if (nova_jan == NULL) {
      ctrl_msg(self, "Erro na abertura da janela");
    } else {
      // insere a nova janela no final da lista e seleciona ela
      lista_insere_fim(self->janelas, nova_jan);
      self->pos_jan_atual = lista_nelem(self->janelas) - 1;
      self->jan_atual = nova_jan;
    }
  }
}

void ctrl_abre(Ctrl self)
{
  // pede um nome para o arquivo e cria um texto com ele
  Str nome = ctrl_le_str(self, "Nome do arquivo a abrir: ");
  ctrl_abre_nome(self, nome);
  str_destroi(nome);
}

Str ctrl_le_str(Ctrl self, char *msg)
{
  // TODO
  return str_cria("teste");
}

void ctrl_msg(Ctrl self, char *msg)
{
  // TODO
}

int ctrl_num_jan_com_txt(Ctrl self, Txt txt)
{
  int nj = 0;
  Jan jan;
  for (jan = lista_percorre(self->janelas);
       jan != NULL;
       jan = lista_proximo(self->janelas)) {
    if (jan_texto(jan) == txt) nj++;
  }
  return nj;
}
void ctrl_fecha(Ctrl self)
{
  Txt txt = jan_texto(self->jan_atual);
  if (txt_alterado(txt) && ctrl_num_jan_com_txt(self, txt) == 1) {
    Str sn = ctrl_le_str(self, "Descarta alterações (responda 'sim')? ");
    Str sim = str_cria("sim");
    bool desiste = !str_igual(sn, sim);
    str_destroi(sn);
    str_destroi(sim);
    if (desiste) return;
  }
  jan_destroi(self->jan_atual);
  lista_remove_pos(self->janelas, self->pos_jan_atual);
  if (self->pos_jan_atual >= lista_nelem(self->janelas)) {
    self->pos_jan_atual = 0;
  }
  self->jan_atual = lista_elem_pos(self->janelas, self->pos_jan_atual);
}

void ctrl_janela_seguinte(Ctrl self)
{
  self->pos_jan_atual++;
  if (self->pos_jan_atual >= lista_nelem(self->janelas)) {
    self->pos_jan_atual = 0;
  }
  self->jan_atual = lista_elem_pos(self->janelas, self->pos_jan_atual);
}

void ctrl_janela_anterior(Ctrl self)
{
  self->pos_jan_atual--;
  if (self->pos_jan_atual < 0) {
    self->pos_jan_atual = lista_nelem(self->janelas) - 1;
  }
  self->jan_atual = lista_elem_pos(self->janelas, self->pos_jan_atual);
}

void ctrl_desenha_tela(Ctrl self)
{
  Txt txt;
  char snome[200];
  tela_limpa();
  tela_cor_selecao();
  tela_lincol(0, 0);
  txt = jan_texto(self->jan_atual);
  str_cstring(txt_nome(txt), snome);
  tela_textof("[%s%s]", snome, txt_alterado(txt)?"+":"");
  tela_cor_normal();
  Jan jan;
  // imprime os nomes nas janelas após a atual
  bool antes = true;
  for (jan = lista_percorre(self->janelas);
       jan != NULL;
       jan = lista_proximo(self->janelas)) {
    if (antes) {
      if (jan != self->jan_atual) continue;
      antes = false;
      continue;
    }
    txt = jan_texto(jan);
    str_cstring(txt_nome(txt), snome);
    tela_textof(" %s%s", snome, txt_alterado(txt)?"+":"");
  }
  // imprime os nomes nas janelas antes da atual
  for (jan = lista_percorre(self->janelas);
       jan != self->jan_atual;
       jan = lista_proximo(self->janelas)) {
    txt = jan_texto(jan);
    str_cstring(txt_nome(txt), snome);
    tela_textof(" %s%s", snome, txt_alterado(txt)?"+":"");
  }
  jan_desenha_txt(self->jan_atual);
}

void ctrl_laco_principal(Ctrl self)
{
  while(lista_nelem(self->janelas) > 0) {	
    ctrl_desenha_tela(self);
    int c = tela_le_char();
		switch(c) {	
      case c_save:
        ctrl_grava(self);
        break;
      case c_close:
        ctrl_fecha(self);
        break;
      case c_open:
        ctrl_abre(self);
        break;
      case c_next:
        ctrl_janela_seguinte(self);
        break;
      case c_previous:
        ctrl_janela_anterior(self);
        break;
      default:
        jan_trata_tecla(self->jan_atual, c);
    }
	}	
}

int main(int argc, char *argv[argc])
{
  tela_cria();
  Ctrl controlador = ctrl_cria();

  if (argc == 1) {
    Str nome = str_cria("sem_nome");
    ctrl_abre_nome(controlador, nome);
    str_destroi(nome);
  }
  for (int i=1; i<argc; i++) {
    Str nome = str_cria(argv[i]);
    ctrl_abre_nome(controlador, nome);
    str_destroi(nome);
  }

  ctrl_laco_principal(controlador);

  ctrl_destroi(controlador);
  tela_destroi();
   
	return 0;
}


