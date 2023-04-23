### Trabalho 3 - lista duplamente encadeada

Reimplemente o TAD `lista.c` para que o editor de texto anexo funcione corretamente.
Deve ser uma lista duplamente encadeada com alocação individual dos nós.
A lista é usada para implementar o texto do editor (uma lista de Str), a lista de textos em edição e a lista de janelas que editam esses textos.
Os dados que a lista gerencia são ponteiros genéricos (`void *`).

Substitua `str.c` pelo de seu t2 para que funcionem os caracteres acentuados.

Para compilar o editor, compile todos os .c e ligue com a biblioteca `curses`. Por exemplo:
```
gcc -g -Wall -o main *.c -lcurses
```
