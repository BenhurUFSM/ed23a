// tela.h
// ------
// implementa uma abstração do terminal de texto
// trata caracteres lidos do teclado e controla a impressão na tela
//

#ifndef TELA_H
#define TELA_H

#include "chu.h"

// inicializa a tela e o teclado
void tela_cria(void);

// volta a tela ao normal
void tela_destroi(void);

// efetivamente processa os comandos de tela anteriores
void tela_atualiza(void);

// limpa a tela
void tela_limpa(void);

// posiciona o cursor (0,0 é o canto superior esquerdo)
void tela_lincol(int lin, int col);

// retorna a altura da tela (número de linhas)
int tela_nlin(void);

// retorna a largura da tela (número de colunas)
int tela_ncol(void);

// imprime a string 'txt' a partir da posição atual do cursor
void tela_texto(char *txt);

// imprime uma string formatada (como printf)
void tela_textof(char *txt, ...);

// cor normal para as próximas impressões
void tela_cor_normal(void);

// cor destacada para as próximas impressões
void tela_cor_selecao(void);

// retorna o próximo caractere da entrada, que pode ser um caractere unicode
//   ou um caractere especial de controle (com código negativo)
//   ou 'c_none' se não houver caractere disponível na entrada
chu tela_le_char(void);

#endif // TELA_H
