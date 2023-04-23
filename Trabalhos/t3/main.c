#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "tela.h"
#include "str.h"
#include "controlador.h"


int main(int argc, char *argv[argc])
{
  tela_cria();
  Ctrl controlador = ctrl_cria();

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

  ctrl_laco_principal(controlador);

  ctrl_destroi(controlador);
  tela_destroi();
   
	return 0;
}


