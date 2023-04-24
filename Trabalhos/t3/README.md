### Trabalho 3 - lista duplamente encadeada

Reimplemente o TAD `lista.c` para que o editor de texto anexo funcione corretamente.
Deve ser uma lista duplamente encadeada com alocação individual dos nós.
A lista é usada para implementar o texto do editor (uma lista de Str), a lista de textos em edição e a lista de janelas que editam esses textos.
Os dados que a lista gerencia são ponteiros genéricos (`void *`).

Substitua `str.c` pelo de seu t2 para que funcionem os caracteres acentuados.

Nenhum outro arquivo (além de lista.c e str.c) precisa ser alterado (a não ser que queira incrementar o editor).

As funções que lidam com utf8 estão em chu.c, com os nomes um pouco diferentes.

Para compilar o editor, compile todos os .c e ligue com a biblioteca `curses`. Por exemplo:
```
gcc -g -Wall -o main *.c -lcurses
```

Para executar, passe nomes de arquivos a editar na linha de comando:
```
./main a.txt texto_de_teste
```

Na edição, dá para usar ctrl-o para abrir outro arquivo, ctrl-w para gravar, ctrl-x para fechar (só fecha se tiver gravado), shift-home e shift-end para trocar de arquivo se tiver editando mais de um. O programa termina quando fechar todos os arquivos.

Foi utilizada a biblioteca [ncurses](https://invisible-island.net/ncurses/ncurses.html).
O código está rodando no [replit](https://replit.com/@BenhurStein/ed-t3), pode fazer um fork e editar ali, se tiver com problemas de instalação local.
