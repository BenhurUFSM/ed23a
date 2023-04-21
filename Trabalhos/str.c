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
int ajusta(int a, int min, int max)
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
  return strlen(s->bytes);
}

int str_numbytes(Str s)
{
  return strlen(s->bytes);
}

int str_char(Str s, int i)
{
  int t = str_tam(s);
  if (i < 0) {
    i += t;
  }
  if (i >= t || i < 0) return -1;
  return s->bytes[i];
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

  // aloca memória para os n caracteres mais o '\0'
  sub->bytes = malloc(sizeof(char)*(n+1));
  if (sub->bytes == NULL) {
    free(sub);
    return NULL;
  }

  // copia n bytes, a partir da posição p de s para sub
  strncpy(sub->bytes, s->bytes + p, n);
  // põe o terminador no final de sub
  sub->bytes[n] = '\0';

  return sub;
}


// retorna a posição da primeira ocorrência do caractere 'c' em 's' ou -1
int str_poschar(Str s, int c)
{
  for (int i=0; ; i++) {
    int ci = str_char(s, i);
    if (ci == '\0') break;
    if (ci == c) return i;
  }
  return -1;
}

// retorna 'true' se 's' e 'o' forem iguais
bool str_igual(Str s, Str o)
{
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

  // calcula alguns tamanhos
  int tam_org = str_tam(s);
  int tam_ini = p;           // caracteres a copiar do início de s
  int tam_ins = str_tam(o);  // caracteres a copiar de o
  int tam_fim = tam_org - tam_ini - n; // caracteres do final de s
  int tam_novo = tam_ini + tam_ins + tam_fim;

  // aloca memória para a string resultante
  char *b = malloc(tam_novo+1);
  if (b == NULL) {
    return;  // xi, quem chama não tem como saber que deu errado
  }
  // copia tam_novo bytes para a nova região:
  //   tam_ini bytes do início de s
  //   tam_ins bytes de o e
  //   tam_fim bytes do final de s
  strncpy(b, s->bytes, tam_ini);
  strncpy(b+tam_ini, o->bytes, tam_ins);
  strncpy(b+tam_ini+tam_ins, s->bytes+tam_ini+n, tam_fim);
  b[tam_novo] = '\0';

  // substitui a string original
  free(s->bytes);
  s->bytes = b;
}

