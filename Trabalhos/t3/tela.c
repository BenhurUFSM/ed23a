#include "tela.h"

// implementado usando a biblioteca curses
#include <curses.h>
#include <locale.h>
#include <stdarg.h>

enum {
  cor_normal = 1,
  cor_selecao,
};

#define CTRL(c) (c&31)   // CTRL('a') produz o código de ctrl-a

static void tela_inicializa_cores(void)
{
  if (has_colors() == FALSE) return;
  start_color();
  init_pair(cor_normal, COLOR_BLACK, COLOR_WHITE);
  init_pair(cor_selecao, COLOR_CYAN, COLOR_BLUE);
}

void tela_cria(void)
{
  setlocale(LC_ALL, "");   // seleciona o locale (espera-se que tenha UTF8)
	initscr();               // inicializa o curses
	noecho();                // não ecoa os caracteres quando são digitados
	cbreak();	               // não espera 'enter' para retornar digitados
  keypad(stdscr, TRUE);    // processa caracteres especiais (setas etc)
  tela_inicializa_cores();
	tela_limpa();
	tela_atualiza();
}

void tela_destroi(void)
{
	endwin();                // finaliza o curses
}

void tela_limpa(void)
{
  clear();
}

void tela_atualiza(void)
{
  refresh();
}

void tela_lincol(int lin, int col)
{
  move(lin, col);
}

int tela_nlin(void)
{
  return LINES;
}

int tela_ncol(void)
{
  return COLS;
}

void tela_cor_normal(void)
{
  if (has_colors() == TRUE) {
    attroff(COLOR_PAIR(cor_selecao));
  } else {
    attroff(A_REVERSE);
  }

}

void tela_cor_selecao(void)
{
  if (has_colors() == TRUE) {
    attron(COLOR_PAIR(cor_selecao));
  } else {
    attron(A_REVERSE);
  }

}

void tela_texto(char *txt)
{
  addstr(txt);
}

void tela_textof(char *txt, ...)
{
  va_list ap;
  va_start(ap, txt);
  const int n = 100;
  char s[n];
  vsnprintf(s, n, txt, ap);
  tela_texto(s);
}

int tela_le_char(void)
{
  static char pendentes[5];  // caracteres utf8 sendo juntados
  static int npendentes = 0; // quantos já foram juntados
  tela_atualiza();
  int c = getch();
  if (c == ERR) return c_none;
  if (npendentes > 0) {
    // estamos acumulando caracteres UTF8, vê se 'c' serve, 
    //   ou esquece os pendentes
    if (utf8_cont_valido(c)) {
      pendentes[npendentes++] = c;
      if (npendentes < utf8_num_bytes(pendentes[0])) {
        return c_none;
      }
      chu ch = chu_de_utf8(pendentes);
      npendentes = 0;
      return ch;
    } else {
      // caractere não pode fazer parte de uma sequência UTF8
      //   detona os pendentes, trata o caractere normalmente
      npendentes = 0;
    }  
  }
  if (c >= 32 && c <= 126) {
    // caractere imprimível ASCII
    return c;
  } else if (utf8_prim_valido(c)) {
   // início de uma sequência de caracteres UTF8, tem que esperar os outros
    pendentes[npendentes++] = c;
    return c_none;
  } else {
    // caractere especial
    switch (c) {
      case KEY_UP:          return c_up;
      case KEY_DOWN:        return c_down;
      case KEY_LEFT:        return c_left;
      case KEY_RIGHT:       return c_right;
      case KEY_HOME:        return c_home;
      case KEY_END:         return c_end;
      case KEY_BACKSPACE:
      case '\b':            return c_back;
      case KEY_DC:          return c_del;
      case KEY_NPAGE:       return c_pgdn;
      case KEY_PPAGE:       return c_pgup;
      case KEY_ENTER:
      case '\n':
      case '\r':            return c_enter;
      case CTRL('s'):       return c_save;
      case CTRL('o'):       return c_open;
      case CTRL('x'):       return c_close;
      case KEY_MOUSE:
      case KEY_RESIZE:
      default:
                            return c_none;
    }
  }
} 
