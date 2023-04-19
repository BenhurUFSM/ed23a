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
    if (s[n] == '\n') {
      s[n] = '\0';
      achei_o_fim_da_linha = true;
    }
    Str tmp = str_cria(s);
    str_altera(res, -1, 0, tmp);
    str_destroi(tmp);
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

static void ajeita_pos_e_tam_de_substr(Str s, int *p, int *n)
{
  // corrige os valores de 'p' e 'n' de acordo com a descrição
  int t = str_tam(s);
  if (*p < 0) {
    // converte posições negativas no equivalente positivo
    *p = *p + t + 1;
  }
  // faz p ficar nos limites da string
  *p = ajusta(*p, 0, t);  // este era o bug em aula, faltava ajustar p
  // faz n ficar nos limites da string
  *n = ajusta(*n, 0, t-*p);
}

Str str_substr(Str s, int p, int n)
{
  Str sub = malloc(sizeof(struct _str));
  if (sub == NULL) return NULL;

  ajeita_pos_e_tam_de_substr(s, &p, &n);

  // aloca memória para os caracteres e copia
  sub->bytes = malloc(sizeof(char)*(n+1));
  if (sub->bytes == 0) {
    free(sub);
    return NULL;
  }
  strncpy(sub->bytes, s->bytes + p, n);
  sub->bytes[n] = '\0';

  return sub;
}


// retorna a posição da primeira ocorrência do caractere 'c' em 's' ou -1
int str_poschar(Str s, int c)
{
  for (int i=0; s->bytes[i] != '\0'; i++) {
    if (s->bytes[i] == c) return i;
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
  ajeita_pos_e_tam_de_substr(s, &p, &n);

  // calcula alguns tamanhos
  int tam_org = str_tam(s);
  int tam_ini = p;           // caracteres a copiar do início de s
  int tam_ins = str_tam(o);  // caracteres de o
  int tam_fim = tam_org - tam_ini - n; // caracteres do final de s
  int tam_novo = tam_ini + tam_ins + tam_fim;

  // aloca memória e copia
  char *b = malloc(tam_novo+1);
  if (b == NULL) {
    return;  // xi, quem chama não tem como saber que deu errado
  }
  strncpy(b, s->bytes, tam_ini);
  strncpy(b+tam_ini, o->bytes, tam_ins);
  strncpy(b+tam_ini+tam_ins, s->bytes+tam_ini+n, tam_fim);
  b[tam_novo] = '\0';

  // substitui a string original
  free(s->bytes);
  s->bytes = b;
}

