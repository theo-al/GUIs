O programa segue o que essa máquina de estados descreve:
![autômato](autômato.png)

Um autômato finito propriamente dito teria estados demais para eu desenhar, a não ser que eu limitasse o número máximo de cliques (limitando a quantidade de cliques a 1 por milissegundo nos 250ms até o fim, seriam 251 estados).

Legenda:
- formato das transições: {símbolo: ação[, ação]\*} 
- símbolos:
    - timeout: 250ms passaram desde o primeiro clique
    - clique: um clique numa certa posição
    - clique fora: um clique numa outra posição
- ações:
    - count: a variável que conta os cliques
    - start: salva a posição e horário do clique, count = 1
    - emit: põe um evento com a quantidade atual de cliques na fila de eventos

O jogo consiste em uma mesa de sinuca com uma bola só, em que a posição do mouse em relação à bola e o número de cliques em sequência determinam a força e direção para onde a bola é movida.

