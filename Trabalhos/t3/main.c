#include <stdio.h>
#include <string.h>
#include "tela.h"

int min(int a, int b)
{
  return a<b ? a : b;
}

int max(int a, int b)
{
  return a>b ? a : b;
}

int limita(int a, int inf, int sup)
{
  return max(inf, min(a, sup));
}

typedef struct _txt *Txt;
struct _txt {

};

int txt_nlin(Txt self)
{
  return 50;
}

int txt_tamlin(Txt self, int lin)
{
  return 70+lin;
}


typedef struct _jan *Jan;
struct _jan {
  int curlin;
  int curcol;
  int lin_topo;
  int col_esq;
  Txt txt;
};

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
  return 3;
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

void jan_ajusta_cursor(Jan self)
{
  // limita o cursor a regiões que contenham texto
  self->curlin = limita(self->curlin, 0, txt_nlin(self->txt)+1);
  self->curcol = limita(self->curcol,
                        0, txt_tamlin(self->txt, self->curlin)+1);
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
  char txt[100];
  int lin_txt = lin - jan_plin_tela(self) + self->lin_topo;
  sprintf(txt, "aqui deveria estar a linha %d", lin_txt);
  int p = self->col_esq;
  if (p > strlen(txt)) p = strlen(txt);
  strcpy(s, txt+p);
}

void jan_desenha_txt(Jan self)
{
  jan_ajusta_cursor(self);
  int tam_lin = (jan_ucol_tela(self)-jan_pcol_tela(self))*4+1;
  char s[tam_lin];
  tela_limpa();
  tela_cor_selecao();
  tela_textof("curlin=%d curcol=%d topo=%d esq=%d", self->curlin, self->curcol, self->lin_topo, self->col_esq);
  tela_cor_normal();
  // desenha_bordas();
  for (int l = jan_plin_tela(self); l <= jan_ulin_tela(self); l++) {
    jan_texto_para_linha(self, l, tam_lin, s);
    tela_lincol(l, jan_pcol_tela(self));
    tela_texto(s);
  }
  tela_lincol(jan_clin_tela(self), jan_ccol_tela(self));
}

int main()
{
  struct _jan j = {0, 0, 2, 0};
  Jan jan = &j;
  
  tela_cria();

	while(1) {	
    int c = tela_le_char();
		switch(c) {	
      case c_up:
        jan->curlin--;
				break;
      case c_down:
        jan->curlin++;
				break;
      case c_left:
        jan->curcol--;
				break;
      case c_right:
        jan->curcol++;
				break;
			case c_enter:
				jan->curlin++;
        jan->curcol = 0;
				break;
    }
    jan_desenha_txt(jan);
	}	
  tela_destroi();
	return 0;
}


