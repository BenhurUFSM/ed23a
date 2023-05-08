#include "str.h"
#include <string.h>
#include <stdlib.h>

// Implementação parcial do TAD Str, visto em aula
// Não implementa suporte a UTF8

// uma Str está sendo implementada como somente um ponteiro para a
//   área que contém a string. Poderia manter outros dados aqui,
//   para simplificar ou acelerar a implementação de alguma operação
//   (tamanho da string, por exemplo)
struct _str {
  char *bytes;
};

// função auxiliar que retorna o valor de 'a', ajustado entre min e max
static int ajusta(int a, int min, int max)
{
  if (a<min) return min;
  if (a>max) return max;
  return a;
}

Str str_cria(char *s)
{
  Str novo;
  novo = malloc(sizeof(struct _str));
  if (novo != NULL) {
    novo->bytes = malloc(strlen(s)+1);
    if (novo->bytes != NULL) {
      strcpy(novo->bytes, s);
    } else {
      free(novo);
      novo = NULL;
    }
  }
  return novo;
}

Str str_cria_linha(FILE *arq)
{
  Str res = str_cria("");
  const int tam = 20;
  char s[tam];
  bool achei_o_fim_da_linha = false;
  while (!achei_o_fim_da_linha && fgets(s, tam, arq) != NULL) {
    int n = strlen(s);
    if (s[n-1] == '\n') {
      s[n-1] = '\0';
      achei_o_fim_da_linha = true;
    }
    Str tmp = str_cria(s);
    str_altera(res, -1, 0, tmp);
    str_destroi(tmp);
  }
  if (!achei_o_fim_da_linha && str_tam(res) == 0) {
    str_destroi(res);
    return NULL;
  }
  return res;
}

void str_destroi(Str s)
{
  if (s != NULL) {
    free(s->bytes);
    free(s);
  }
}

int str_tam(Str s)
{
  if (s == NULL) return 0;
  int ct = 0;
  char *p = s->bytes;
  while (*p != '\0') {
    // avança um caractere e o número de bytes que ele ocupa
    p += utf8_num_bytes(*p);
    ct++;
  }
  return ct;
}

int str_numbytes(Str s)
{
  if (s == NULL) return 0;
  return strlen(s->bytes);
}

chu str_char(Str s, int i)
{
  int t = str_tam(s);
  // converte posição para um valor positivo
  if (i < 0) {
    i += t;
  }
  if (i >= t || i < 0) return -1;
  return chu_de_utf8(utf8_nesimo_chu(s->bytes, i));
}

// função auxiliar, conta o número de bytes ocupados por n chu em s
static int bytes_em_n_chu(char *s, int n)
{
  int nby_t = 0;
  while (n > 0) {
    int nby_1 = utf8_num_bytes(*s);
    s += nby_1;
    nby_t += nby_1;
    n--;
  }
  return nby_t;
}

// p é uma posição em s, podendo ser positiva (medida a partir do início)
//   ou negativa (medida a partir do final)
// n é o tamanho de uma substring a partir de p
// esta função corrige p para ser positivo e estar em uma posição
//   existente de s, e n para representar um tamanho válido em s
static void ajeita_pos_e_tam_de_substr(Str s, int *p, int *n)
{
  int t = str_tam(s);
  if (*p < 0) {
    // converte posições negativas no equivalente positivo
    //   se p for -1, vira t; se for -2, vira t-1 etc
    *p = *p + t + 1;
  }
  // faz p ficar nos limites da string (entre 0 e t)
  *p = ajusta(*p, 0, t);  // este era o bug em aula, faltava ajustar p
  // faz n ficar nos limites da string
  *n = ajusta(*n, 0, t - *p);
}

Str str_substr(Str s, int p, int n)
{
  // cria uma nova Str para conter a substring
  Str sub = malloc(sizeof(struct _str));
  if (sub == NULL) return NULL;

  // corrige p e n para ficarem nos limites de s
  ajeita_pos_e_tam_de_substr(s, &p, &n);

  // ini aponta para o primeiro byte da substring;
  // nby é o número de bytes da substring
  char *ini = s->bytes + bytes_em_n_chu(s->bytes, p);
  int nby = bytes_em_n_chu(ini, n);

  // aloca memória para os nby caracteres mais o '\0'
  sub->bytes = malloc(sizeof(char)*(nby+1));
  if (sub->bytes == NULL) {
    free(sub);
    return NULL;
  }

  // copia nby bytes, a partir da posição ini para sub
  strncpy(sub->bytes, ini, nby);
  // põe o terminador no final de sub
  sub->bytes[nby] = '\0';

  return sub;
}


// retorna a posição da primeira ocorrência do caractere 'c' em 's' ou -1
int str_poschar(Str s, chu c)
{
  for (int i=0; ; i++) {
    int ci = str_char(s, i);
    if (ci == '\0') break;
    if (ci == c) return i;
  }
  return -1;
}

// retorna 'true' se as strings em 's' e 'o' forem iguais
bool str_igual(Str s, Str o)
{
  if (s == o) return true;
  if (s == NULL || o == NULL) return false;
  return strcmp(s->bytes, o->bytes) == 0;
}

// altera 's', substituindo os 'n' caracteres a partir de 'p' pelo conteúdo de 'o'
// valores negativos de 'n' são tratados como 0
// se 'p' além do final de 's', deve ser tratado como logo após o final
// se 'p' antes do início de 's', deve ser tratado cono logo antes do início
// valores negativos de 'p' referem-se ao final de 's' (-1 é logo após o final de 's',
//  -2 logo antes do último caractere, etc.)
void str_altera(Str s, int p, int n, Str o)
{
  // corrige p e n para ficarem nos limites de s
  ajeita_pos_e_tam_de_substr(s, &p, &n);

  // calcula algumas posições e tamanhos em bytes
  char *i_ini = s->bytes;
  int nby_ini = bytes_em_n_chu(i_ini, p);
  char *i_meio = o->bytes;
  int nby_meio = str_numbytes(o);
  char *i_del = i_ini + nby_ini;
  int nby_del = bytes_em_n_chu(i_del, n);
  char *i_fim = i_del + nby_del;
  int nby_fim = str_numbytes(s) - nby_ini - nby_del;
  int nby_novo = nby_ini + nby_meio + nby_fim;

  // aloca memória para a string resultante
  char *b = malloc(nby_novo+1);
  if (b == NULL) {
    return;  // xi, quem chama não tem como saber que deu errado
  }
  // copia tam_novo bytes para a nova região:
  //   tam_ini bytes do início de s
  //   tam_ins bytes de o e
  //   tam_fim bytes do final de s
  strncpy(b, i_ini, nby_ini);
  strncpy(b+nby_ini, i_meio, nby_meio);
  strncpy(b+nby_ini+nby_meio, i_fim, nby_fim);
  b[nby_novo] = '\0';

  // substitui a string original
  free(s->bytes);
  s->bytes = b;
}

void str_cstring(Str s, char *p)
{
  if (s == NULL) {
    *p = '\0';
  } else {
    strcpy(p, s->bytes);
  }
}

void str_grava(Str s, FILE *arq)
{
  fputs(s->bytes, arq);
}

#ifdef TESTE
// testa o TAD

#include <assert.h>

void str_teste(void)
{
  Str s1, s2, s3, s4;
  s1 = str_cria("aba");
  s2 = str_cria("caxi");
  s3 = str_cria("abacaxi");
  str_altera(s1, -1, 0, s2);
  assert(str_igual(s1, s3));
  s4 = str_substr(s2, 20, 10);
  assert(str_tam(s4) == 0);
  str_altera(s3, 0, 3, s4);
  assert(str_igual(s2, s3));
}
#endif
