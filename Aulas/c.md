### Revisão de C

A parte I do livro do Celes (por exemplo) cobre bem esses assuntos.

#### Tipos básicos de dados

Os tipos de dados básicos de C podem representar valores inteiros (char, short, int, long), de ponto flutuante (float, double), lógico (bool, só disponível com inclusão de stdbool.h).

#### Variáveis

Uma variável representa uma região de memória. Antes de se usar, deve ser declarada, com um nome e um tipo.
```c
int a;  // declara a variável chamada 'a', que pode receber valores do tipo 'int'
```
Uma variável pode ser o destino de um valor calculado em uma expressão, em uma atribuição. Em uma expressão, o nome de uma variável é substituído por seu valor no momento que a expressão é avaliada.
```c
a = 10 * b;  // altera o valor da variável 'a' para 10 vezes o valor atual da variável 'b'
```

#### Entrada e saída no terminal

As principais funções de entrada e saída são *scanf* e *printf*. 
O primeiro argumento dessas funções é uma string de formato, que diz que conversões devem ser realizadas; os demais argumentos devem estar de acordo. A função `printf` imprime uma string no terminal.
```c
// pede para printf formatar o valor de 'a' como uma string de dígitos decimais (no lugar do '%d') 
//   e a metade do valor de 'x' como um valor decimal com 2 casas após o ponto (no lugar do '%.2f'),
//   e imprimir a string resultante no terminal ('\n' representa mudança para a próxima linha)
printf("Resultado 1: %d\nResultado 2:%.2f\n", a, x/2);
```
A função `scanf` converte valores a partir dos caracteres digitados no terminal.
```c
// pede para scanf ler caracteres do terminal:
// - ignorar os espaços iniciais (' '),
// - colocar o próximo caractere na variável 'letra' ('%c'),
// - ignorar espaços e converter a sequência de dígitos que segue em um valor int
//   e colocá-lo na variável 'a' (%d)
scanf(" %c%d", &letra, &a);
```

#### Comandos de seleção

C tem 2 comandos que permitem controlar de a execução de outros comandos será ou não realizada.

O comando `if` permite escolher um entre dois grupos de comandos, baseado no resultado de uma expressão lógica.
Se a expressão tiver o valor `true`, os primeiro grupo de comandos será executado e o segundo será ignorado.
Se o valor for `false`, o primeiro grupo será ignorado e o segundo (cláusula `else`) será executado. A cláusula `else` é opcional.
```c
// seleciona os comandos a executar, dependendo do valor de 'delta'
if (delta >= 0) {
  x = 10 * sqrt(delta);
} else {
  x = 0;
  naocalc = true;
}
```
O comando `switch` permite escolher em qual comando de uma sequência a execução iniciará.
A execução do comando `break` dentro do `switch` encerra a execução dos comandos controlados pelo `switch`.
Os comandos dentro do `switch` onde a execução pode iniciar são precedidos por cláusulas `case` associadas a uma valor inteiro. O comando calcula o valor de uma expressão inteira e inicia a execução no primeiro comando após a cláusula `case` associada ao valor calculado. Caso não exista tal cláusula, inicia no primeiro comando após a cláusula `default`.
```c
switch (opcao) {
  case 'A':
  case 'a':
    abertura();
    break;
  case 'f':
    fechamento();
    break;
  case 'F':
    fim();
    break;
  default:
    erro();
}
```

#### Laços de repetição

Os comandos `while`, `for` e `do-while` permitem executar os comandos controlados por eles repetidas vezes, baseado no resultado de uma expressão lógica. A cada vez, o valor da expressão é calculado e se for `true` os comandos controlados são executados e a expressão é recalculada. Quando a expressão resultar em `false`, o comando de repetição termina.
Entre os comandos controlados podem estar os comandos especiais `break`, que encerra a execução do comando de repetição e `continue`, que interrompe a repetição corrente (como se tivesse executado o último dos comandos controlados).
```c
while (tem_coisa_pra_fazer()) {
  faz_alguma_coisa();
}
```
O `do-while` é equivalente ao `while` sem executar a expressão na primeira vez.
```c
do {
  d = le_dado();
} while (dado_invalido(d));
```
O `for` adiciona 2 comandos, um para ser executado uma vez antes de iniciar as repetições e outro para executar após cada repetição.
```
for (A; B; C) {
  D;
}
// é quase equivalente a
A;
while (B) {
  D;
  C;
}
// a diferença é que a execução de um 'continue' em D desvia para a execução de C

// imprime os valores de 0 a 9 exceto 4 e 7:
for (int i = 0; i < 10; i++) {
  if (i == 4 || i == 7) continue;
  printf("%d ", i);
}
```

- funções (modularização, passagem de parâmetros por valor e referência)
- vetores e strings
- registros, ponteiros para registros
- arquivos
- alocação dinâmica de memória
