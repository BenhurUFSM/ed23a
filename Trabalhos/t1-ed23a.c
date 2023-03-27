// programa visto em aula, com alguns comentários a mais
// não compilei, tem coisas faltando, nomes a trocar, certamente não roda
#include <stdio.h>
#include <stdbool.h>

typedef struct {
  long matr;
  float n1;
  float n2;
  float media;
} aluno_t_p1;

typedef struct {
  long matr;
  char nome[30];
  float n1;
  float n2;
  float media;
} aluno_t_p2;

typedef struct {
  long matr;
  char *nome;
  float n1;
  float n2;
  float media;
} aluno_t_p3;

// lê uma string do arquivo arq
// a string deve estar delimitada por aspas
// deve ser alocado espaço para conter a string
// retorna um ponteiro para o espaço que foi alocado para a string
// retorna NULL em caso de erro
char *le_str_entre_aspas_p3(FILE *arq)
{
  char *s;   // ponteiro para a string
  int aloc;  // número de bytes alocados
  int tam;   // número de bytes ocupados pela string
  char c;
  // lê um caractere do arquivo, após pular espaços. deve ser aspas, ou é erro.
  if (fscanf(arq, " %c", &c) != 1) return NULL;
  if (c != '"') return NULL;
  // aloca espaço para a string. chuta tamanho 30, ajusta depois se não for isso.
  tam = 0;
  aloc = 30;
  s = malloc(aloc);
  if (s == NULL) return NULL;
  for (;;) {
    // lê um char, se der erro ou chegar no final da linha, é erro (tem que liberar a memória nesse caso)
    if (fscanf(arq, "%c", &c) != 1) { free(s); return NULL; }
    if (c == '\n') { free(s); return NULL; }
    // se for aspas, a string terminou
    if (c == '"') break;
    // caractere é bom, coloca no fim da string
    if (tam >= aloc) {
      // o espaço alocado tá cheio, aloca mais
      aloc += 30;
      char *ns = realloc(s, aloc);
      if (ns == NULL) { free(s); return NULL; }
      s = ns;
    }
    s[tam] = c;
    tam++;
  }
  // terminou a string, realoca para o tamanho justo
  if (aloc != tam+1) {
    char *ns = realloc(s, tam+1);
    if (ns == NULL) { free(s); return NULL; }
    s = ns;
  }
  // toda string em C tem que ter um \0 no fim
  s[tam] = '\0';
  return s;
}

// lê uma string do arquivo arq
// a string deve estar delimitada por aspas
// a string deve ser colocada no vetor de tamanho n
// retorna true se OK, false se não
bool le_str_entre_aspas_p2(FILE *arq, int n, char s[n])
{
  int tam = 0;   // número de bytes ocupados pela string
  char c;
  // lê um caractere do arquivo, após pular espaços. deve ser aspas, ou é erro.
  if (fscanf(arq, " %c", &c) != 1) return false;
  if (c != '"') return false;
  // lê cada char, até aspas ou erro (problema na leitura, fim da linha, str muito grande)
  for (;;) {
    if (fscanf(arq, "%c", &c) != 1) return false;
    if (c == '\n') return false;
    if (c == '"') break;
    if (tam >= n-1) return false;
    // caractere é bom, coloca no fim da string
    s[tam] = c;
    tam++;
  }
  // terminou a string
  // toda string em C tem que ter um \0 no fim
  s[tam] = '\0';
  return true;
}

bool le_1_aluno_p1(FILE *arq, aluno_t *a)
{
  if (fscanf(arq, "%ld", &a->matr) != 1) return false;
  a->nome = le_str_entre_aspas(arq);
  if (fscanf(arq, "%f", &a->n1) != 1) return false;
  if (fscanf(arq, "%f", &a->n2) != 1) return false;
  return true;
}

bool le_1_aluno_p2(FILE *arq, aluno_t *a)
{
  if (fscanf(arq, "%ld", &a->matr) != 1) return false;
  if (!le_str_entre_aspas_p2(arq, 30, a->nome)) return false;
  if (fscanf(arq, "%f", &a->n1) != 1) return false;
  if (fscanf(arq, "%f", &a->n2) != 1) return false;
  // ignora o resto da linha
  le_ate_fim_da_linha(arq);
  return true;
}

bool le_1_aluno_p3(FILE *arq, aluno_t *a)
{
  if (fscanf(arq, "%ld", &a->matr) != 1) return false;
  a->nome = le_str_entre_aspas(arq);
  if (a->nome == NULL) return false;
  if (fscanf(arq, "%f", &a->n1) != 1) return false;
  if (fscanf(arq, "%f", &a->n2) != 1) return false;
  // ignora o resto da linha
  le_ate_fim_da_linha(arq);
  return true;
}


// preenche o vetor "alunos" de tamanho "n" com dados lidos do arquivo "nome".
// retorna o número de registros lidos.
// se o arquivo contiver mais de n alunos ou se houver erro, deve retornar -1.
// não esqueça de fechar o arquivo.
aluno_t *le_arq_p3(char *nome, int *n)
{
  aluno_t *vet;
  int nalu;
  FILE *arq;
  arq = fopen(nome, "r");
  if (arq == NULL) return NULL;

  if (fscanf(arq, "%d", &nalu) != 1) { fclose(arq); return NULL; }
  vet = malloc(nalu * sizeof(aluno_t));
  if (vet == NULL) { fclose(arq); return NULL; }
	  
  for (int i=0; i<nalu; i++) {
    if (!le_1_aluno(arq, &vet[i])) { fclose(arq); free(vet); return NULL; }
  }

  fclose(arq);
  *n = nalu;
  return vet;
}

// grava "n" dados do vetor no arquivo
// mesmo formato, mas com uma terceira nota (a média)
// esta função grava, não calcula médias
void grava_arq(int n, aluno_t alunos[n], char *nome);

void calcula_1_media(aluno_t *a)
{
  a->media = (a->n1 + a->n2) / 2;
}
// calcula a media dos n alunos do vetor
void calcula_medias(int n, aluno_t alunos[n])
{
  for (int i=0; i<n; i++) {
    calcula_1_media(&aluno[i]);
  }

int main()
{
  aluno_t *vet;
  int n;
  vet = le_arq("alunos", &n);
  if (n < 0) {
    fprintf(stderr, "erro na leitura do arquivo.\n");
    return 1;
  }
  calcula_medias(n, vet);
  grava_arq(n, vet, "alunos");
  return 0;
}
