#include "controlador.h"
#include "janela.h"
#include "lista.h"
#include "texto.h"
#include "tela.h"
#include <stdlib.h>

struct _ctrl {
  Lista textos;
  Lista janelas;
  int pos_jan_atual;
  Jan jan_atual;
};

// Funções internas

// Interação com o usuário

// Faz uma pergunta ao usuário, retorna true se a resposta for 'sim'
static bool ctrl_sim_nao(Ctrl self, char *msg);
// mostra mensagem e espera uma string do teclado; retorna a string
static Str ctrl_le_str(Ctrl self, char *msg);
// faz aparecer uma mensagem para o usuário
static void ctrl_msg(Ctrl self, char *msg);

// desenha a tela
static void ctrl_desenha_tela(Ctrl self);
// desenha o nome da janela
static void ctrl_desenha_nome(Ctrl self, Jan jan);
// desenha o nome de todas as janelas no topo da tela
static void ctrl_desenha_nomes(Ctrl self);


// funções de manipulação de arquivos / janelas

// grava em arquivo o texto da janela corrente
static void ctrl_grava(Ctrl self);

// retorna um texto com o conteúdo do arquivo
// se já existir um, retorna ele, senão cria um novo
static Txt ctrl_txt_com_nome(Ctrl self, Str nome);

// retorna o número de janelas abertas no mesmo texto
static int ctrl_num_jan_com_txt(Ctrl self, Txt txt);

// libera o texto (retira da lista, libera memória)
static void ctrl_libera_txt(Ctrl self, Txt txt);

// fecha a janela atual
static void ctrl_fecha(Ctrl self);

// torna corrente a próxima janela
static void ctrl_janela_seguinte(Ctrl self);

// torna corrente a janela anterior
static void ctrl_janela_anterior(Ctrl self);



// Funções da API (visíveis externamente)

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
  // pede um nome para o arquivo e abre ele
  Str nome = ctrl_le_str(self, "Nome do arquivo a abrir: ");
  ctrl_abre_nome(self, nome);
  str_destroi(nome);
}

void ctrl_laco_principal(Ctrl self)
{
  while(lista_nelem(self->janelas) > 0) {
    ctrl_desenha_tela(self);
    // lê um caractere, trata se for o caso, repassa pra janela se não
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


// Implementação das funções internas

static void ctrl_grava(Ctrl self)
{
  txt_grava(jan_texto(self->jan_atual));
}

static Txt ctrl_txt_com_nome(Ctrl self, Str nome)
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


static Str ctrl_le_str(Ctrl self, char *msg)
{
  // TODO
  return str_cria("teste");
}

static void ctrl_msg(Ctrl self, char *msg)
{
  // TODO
}

static bool ctrl_sim_nao(Ctrl self, char *msg)
{
  Str sn = ctrl_le_str(self, msg);
  Str sim = str_cria("sim");
  bool resposta = !str_igual(sn, sim);
  str_destroi(sn);
  str_destroi(sim);
  return resposta;
}



static int ctrl_num_jan_com_txt(Ctrl self, Txt txt)
{
  int nj = 0;
  Jan jan;
  // percorre a lista de janelas procurando as que têm o texto
  for (jan = lista_percorre(self->janelas);
       jan != NULL;
       jan = lista_proximo(self->janelas)) {
    if (jan_texto(jan) == txt) nj++;
  }
  return nj;
}

static void ctrl_libera_txt(Ctrl self, Txt txt)
{
  int nt = 0;
  Txt t;
  // nossa lista não tem remoção por conteúdo nem busca :o(
  for (t = lista_percorre(self->textos);
       t != NULL;
       t = lista_proximo(self->textos)) {
    if (t == txt) {
      lista_remove_pos(self->textos, nt);
      txt_destroi(txt);
      break;
    }
    nt++;
  }
}

static void ctrl_fecha(Ctrl self)
{
  // não fecha a janela se for a única com esse texto,
  //   o texto tiver sido alterado e o usuário não confirmar o descarte
  Txt txt = jan_texto(self->jan_atual);
  int njan = ctrl_num_jan_com_txt(self, txt);
  if (txt_alterado(txt) && njan == 1) {
    if (ctrl_sim_nao(self, "Descarta alterações (responda 'sim')? ")) {
      return;
    }
  }
  // salva a posição da janela atual e altera a atual para a seguinte
  int pj = self->pos_jan_atual;
  ctrl_janela_seguinte(self);
  // remove a janela da lista e destroi ela
  jan_destroi(lista_remove_pos(self->janelas, pj));
  if (self->pos_jan_atual >= pj) self->pos_jan_atual--;
  // se só tinha essa janela com esse texto, libera o texto
  if (njan == 1) ctrl_libera_txt(self, txt);
}

static void ctrl_janela_seguinte(Ctrl self)
{
  self->pos_jan_atual++;
  if (self->pos_jan_atual >= lista_nelem(self->janelas)) {
    self->pos_jan_atual = 0;
  }
  self->jan_atual = lista_elem_pos(self->janelas, self->pos_jan_atual);
}

static void ctrl_janela_anterior(Ctrl self)
{
  self->pos_jan_atual--;
  if (self->pos_jan_atual < 0) {
    self->pos_jan_atual = lista_nelem(self->janelas) - 1;
  }
  self->jan_atual = lista_elem_pos(self->janelas, self->pos_jan_atual);
}

static void ctrl_desenha_nome(Ctrl self, Jan jan)
{
  char snome[200];
  Txt txt = jan_texto(jan);
  str_cstring(txt_nome(txt), snome);
  tela_textof(" %s%s ", snome, txt_alterado(txt)?"+":"");
}

static void ctrl_desenha_nomes(Ctrl self)
{
  // desenha o nome da janela corrente, em destaque
  tela_cor_selecao();
  tela_lincol(0, 0);
  ctrl_desenha_nome(self, self->jan_atual);
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
    ctrl_desenha_nome(self, jan);
  }
  // imprime os nomes nas janelas antes da atual
  for (jan = lista_percorre(self->janelas);
       jan != self->jan_atual;
       jan = lista_proximo(self->janelas)) {
    ctrl_desenha_nome(self, jan);
  }
}

static void ctrl_desenha_tela(Ctrl self)
{
  tela_limpa();
  ctrl_desenha_nomes(self);
  // desenha o texto da janela corrente
  jan_desenha_txt(self->jan_atual);
}

