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

O **arquivo de locais** descreve um local por linha válida não vazia. Um local é composto por 3 ou 4 valores, separados por espaços:
- a identificação do local (entre 1 e 4 caracteres);
- a latitude do local (um número double, em graus);
- a longitude do local (um número double, em graus);
- o descrição do local (uma string, até o final da linha (ou #)), opcional.

O **arquivo de caminhos** tem um caminho válido por linha não vazia.
Um caminho é composto por duas identificações de local, separadas por espaços.

O custo de um caminho é a distância entre os dois locais que ele interliga (calculada à partir da latitude e longitude desses locais).

O **arquivo de locais a visitar** deve conter a identificação de um local em cada linha não vazia. O primeiro desses locais é o local de partida, os demais são locais a visitar, sem ordem específica.

O **arquivo de saída** contém um local por linha, que é o caminho a ser percorrido para se passar por todos os locais que se quer visitar (provavelmente incluindo também outros locais de passagem).
Cada linha contém a latitude, longitude, identificação e descrição de um local.
Esse arquivo deve estar em formato csv, com os valores separados por vírgula, e strings entre aspas.
