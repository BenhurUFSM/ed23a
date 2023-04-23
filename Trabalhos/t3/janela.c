#include "janela.h"
#include "tela.h"
#include <stdlib.h>
#include <limits.h>

struct _jan {
  int curlin;
  int curcol;
  int lin_topo;
  int col_esq;
  Txt txt;
};

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
