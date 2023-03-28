### Implemente o TAD string

Você deve implementar um TAD para a manipulação de strings em C, com alocação dinâmica de memória, seguindo as convenções da disciplina, com a interface definida em [str.h](str.h).

Exemplos de testes:
```c
  Str s1, s2, s3, s4;
  s1 = str_cria("aba");
  s2 = str_cria("caxi");
  s3 = str_cria("abacaxi");
  str_altera(s1, -1, 0, s2);
  assert(str_igual(s1, s3));
  s4 = str_substr(s2, 20, 10);
  assert(str_tam(s4) == 0);
  str_altera(s3, 0, 3, s4);
  assert(str_igual(s2, s3));
```
