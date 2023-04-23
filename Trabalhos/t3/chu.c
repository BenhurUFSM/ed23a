#include "chu.h"
#include <stddef.h> // para NULL

int utf8_num_bytes(char b)
{
  // o primeiro byte permite identificar o número de bytes usados para 
  //   codificar um caractere em UTF8

  // se o byte for 0xxxxxxx, o caractere ocupa só um byte
  // (7 bits, código entre 0000 e 007F)
  if ((b & 0b10000000) == 0b00000000) return 1;
  // se o byte for 110xxxxx, o caractere ocupa 2 bytes,
  // 5 bits no primeiro e 6 no segundo (11 bits, códigos até 07FF)
  if ((b & 0b11100000) == 0b11000000) return 2;
  // se o byte for 1110xxxx, o caractere ocupa 3 bytes,
  // 4 bits no primeiro e 6 nos outros 2 (16 bits, códigos até FFFF)
  if ((b & 0b11110000) == 0b11100000) return 3;
  // se o byte for 11110xxx, o caractere ocupa 4 bytes,
  // 3 bits no primeiro e 6 nos outros 3 (21 bits, códigos válidos até 10FFFF)
  if ((b & 0b11111000) == 0b11110000) return 4;
  // se começar com outra coisa, temos um problema
  return 0;
}

int chu_num_bytes(chu c)
{
  if (c < 0 || c > 0x10FFFF) return 0;
  else if (c <= 0x007F) return 1;
  else if (c <= 0x07FF) return 2;
  else if (c <= 0xFFFF) return 3;
  return 4;
}

chu chu_de_utf8(char *p)
{
  switch (utf8_num_bytes(p[0])) {
    case 1:
      return   p[0];
    case 2:
      return ((p[0] & 0b00011111) <<  6) 
           |  (p[1] & 0b00111111);
    case 3:
      return ((p[0] & 0b00001111) << 12)
           | ((p[1] & 0b00111111) <<  6)
           |  (p[2] & 0b00111111);
    case 4:
      return ((p[0] & 0b00000111) << 18)
           | ((p[1] & 0b00111111) << 12)
           | ((p[2] & 0b00111111) << 6)
           |  (p[3] & 0b00111111);
    default:
      return 0; // erro!
  }
}

void chu_para_utf8(chu c, char s[5])
{
  switch (chu_num_bytes(c)) {
    case 1:
      s[0] =   c;
      s[1] = '\0';
      break;
    case 2:
      s[0] =  (c >>  6)               | 0b11000000;
      s[1] = ( c        & 0b00111111) | 0b10000000;
      s[2] = '\0';
      break;
    case 3:
      s[0] =  (c >> 12)               | 0b11100000;
      s[1] = ((c >>  6) & 0b00111111) | 0b10000000;
      s[2] = ( c        & 0b00111111) | 0b10000000;
      s[3] = '\0';
      break;
    case 4:
      s[0] =  (c >> 18)               | 0b11110000;
      s[1] = ((c >> 12) & 0b00111111) | 0b10000000;
      s[2] = ((c >>  6) & 0b00111111) | 0b10000000;
      s[3] = ( c        & 0b00111111) | 0b10000000;
      s[4] = '\0';
      break;
    default:
      s[0] = '\0';
  }
}

bool utf8_prim_valido(char b)
{
  return utf8_num_bytes(b) > 1;
}

bool utf8_cont_valido(char b)
{
  // bytes de continuação UTF8 iniciam com 10 em binário
  return (b & 0b11000000) == 0b10000000;
}

char *utf8_nesimo_chu(char *s, int n)
{
  int ct = 0;
  while (*s != '\0') {
    // se chegou no n-ésimo caractere, retorna o ponteiro
    if (ct == n) return s;
    // avança o número de bytes necessários para o caractere
    s += utf8_num_bytes(*s);
    // um caractere a mais
    ct++;
  }
  return NULL;
}
