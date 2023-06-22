## t4 - percurso no câmpus

Você deve fazer um programa que encontra o caminho mais curto entre locais selecionados no câmpus da UFSM.

O programa tem como entrada 3 arquivos:
- um com locais do câmpus (vértices)
- um com caminhos entre esses locais (arestas)
- um com os locais que se quer visitar.

A saída do programa é um arquivo contendo uma sequência de locais pelos quais se deve passar, de forma a visitar todos os locais desejados. 
Essa sequência deve ter custo mínimo.

### Formato dos arquivos

Os arquivos são arquivos texto, organizados por linhas.
O caractere `#` e os que o seguem em uma linha devem ser ignorados.
O programa deve ignorar linhas em branco e vazias.

O **arquivo de locais** descreve um local por linha válida não vazia. Um local é composto por 3 ou 4 valores, separados por vírgula (e podendo ter espaços):
- a identificação do local (entre 1 e 4 caracteres);
- a latitude do local (um número double, em graus);
- a longitude do local (um número double, em graus);
- o descrição do local (uma string, até o final da linha (ou #)), opcional.

O **arquivo de caminhos** tem um caminho válido por linha não vazia.
Um caminho é composto por duas identificações de local, separadas por vírgula (e podendo ter espaços).

O custo de um caminho é a distância entre os dois locais que ele interliga (calculada à partir da latitude e longitude desses locais).

O **arquivo de locais a visitar** deve conter a identificação de um local em cada linha não vazia. O primeiro desses locais é o local de partida, os demais são locais a visitar, sem ordem específica.

O **arquivo de saída** deve estar no formato [[map]bbcode](http://mapbbcode.org/). Esse formato começa por `\[map]` e termina por `\[/map]`. Dentro, pode conter locais ou caminhos, separados por `;`. Um local contém latitude e longitude separados por vírguna e nome entre parênteses (por exemplo, `-29.72074,-53.7149(Reitoria da UFSM)`). Um caminho contém uma sequência de coordenadas separadas por espaço, uma coordenada é latitude e longitude separados por vírgula (por exemplo, `-29.72074,-53.7149 -29.720763,-53.715030 -29.720522,-53.715022`).

O arquivo deve conter o local de partida, seguido pelo caminho até o segundo local, seguido pelo segundo local, seguido pelo caminho até o terceiro local, seguido pelo terceiro local etc.

O primeiro local é o primeiro do arquivo de locais a visitar; o segundo é um outro local desse arquivo, aquele que tem o caminho mais curto a partir do primeiro; o terceiro é um outro local desse arquivo, aquele que tem o caminho mais curto a partir do segundo etc.

Por exemplo, o arquivo de saída para um caminho entre a reitoria e o CPD poderia conter:
```
[map]
-29.72074,-53.7149(Reitoria da UFSM);
-29.72074,-53.7149 -29.72076,-53.71503 -29.72058,-53.71504 -29.7204,-53.71373 -29.72083,-53.71365;
-29.72083,-53.71365(CPD)
[/map]
```

Essa saída pode ser visualizada em (http://share.mapbbcode.org/), colando o conteúdo do arquivo na janela que abre quando clica em "Edit Raw".


### TADs

O grafo deve ser implementado conforme [grafo.h](grafo.h).
Outras estruturas (lista, fila, de busca, etc) devem ser implementadas como TADs (um arquivo .h e um .c para cada TAD, com acesso aos dados internos do TAD limitados ao .c, só com funções relativas ao TAD).

### Dicas

#### Distância entre 2 coordenadas

Não necessitamos de uma precisão muito grande.
Podemos aproximar o cálculo supondo que o planeta é uma esfera, e que o câmpus é um espaço 2d.
Só queremos calcular a distância entre duas coordenadas.
Dá para dividir em uma distância norte-sul (diferença entre latitudes) e uma distância leste-oeste (diferença entre longitudes), e depois considerar que essas distâncias são catetos de um triângulo retângulo cuja hipotenusa é o valor da distância que queremos.

90 graus de diferença de latitude correspondem a um quarto da circunferência da terra ou 10000km.

90 graus de diferença de longitude correspondem a um quarto da circunferência de um círculo que corta a terra horizontalmente na latitude considerada. Um quarto da circunferência desse círculo é cos(latitude)*10000km.
