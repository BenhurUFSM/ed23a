// chu.h
// -----
// suporte mínimo para caracteres unicode/UTF8 (para a disciplina ed23a)
//
// alterações:
//   16abr: criação

// não altere este arquivo

// evita inclusões múltiplas
#ifndef _CHU_H_
#define _CHU_H_

#include <stdbool.h>

// um caractere unicode é representado por um inteiro entre 0 e 0x10FFFF
// valores negativos são usados para representar caracteres especiais,
//   de controle
typedef enum {
  c_none = -1000,
  c_enter,
  c_back,
  c_del,
  c_pgup,
  c_pgdn,
  c_home,
  c_end,
  c_left,
  c_right,
  c_up,
  c_down,
  c_save,
  c_open,
  c_close,
  c_primeiro_unicode = 0,
  c_ultimo_unicode = 0x10FFFF,
} chu;


// retorna o número de bytes usados para codificar um caractere unicode
//   em UTF8 quando o primeiro caractere codificado é 'b'
int utf8_num_bytes(char b);

// retorna o número de bytes necessários para codificar o caractere unicode
//   'c' em UTF8
int chu_num_bytes(chu c);

// retorna o código do caractere unicode codificado em UTF-8 a partir de 'p'
chu chu_de_utf8(char *p);

// converte o código do caractere 'c' para string UTF-8 a partir de 's'
// tem que ter espaço suficiente em 's', a conversão pode ocupar até 4
//   caracteres além do '\0' de finalização da string
void chu_para_utf8(chu c, char s[5]);

// retorna true se 'b' pode iniciar uma sequência UTF8
bool utf8_prim_valido(char b);

// retorna true se 'b' pode ser sequência UTF8
bool utf8_cont_valido(char b);

#endif // _CHU_H_
