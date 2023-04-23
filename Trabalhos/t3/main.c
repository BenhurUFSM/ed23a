#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "tela.h"
#include "str.h"
#include "controlador.h"


int main(int argc, char *argv[argc])
{
  // inicializa a tela e cria um controlador
  tela_cria();
  Ctrl controlador = ctrl_cria();

  // abre os arquivos passados pelo usuário na linha de comando
  // ou um sem nome se o usuário não passou nenhum
  if (argc == 1) {
    Str nome = str_cria("sem_nome");
    ctrl_abre_nome(controlador, nome);
    str_destroi(nome);
  }
  for (int i=1; i<argc; i++) {
    Str nome = str_cria(argv[i]);
    ctrl_abre_nome(controlador, nome);
    str_destroi(nome);
  }

  // faz a edição
  ctrl_laco_principal(controlador);

  // libera o que foi alocado
  ctrl_destroi(controlador);
  tela_destroi();
   
	return 0;
}


