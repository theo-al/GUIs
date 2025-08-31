- `03-buffer.c`:
    - https://github.com/fsantanna-uerj/GUIs/blob/master/01-sdl/03-buffer.c
- Note que há duas chamadas a SDL_RenderPresent.
- Assuma que o correto seria apenas 1 chamada.
- Desconsidere o problema de a animação ser instantânea.

1. Visualmente, qual o bug do programa?
    - Na maioria das vezes que rodei, só dava pra ver um retângulo azul parado. Nas vezes que aparece mais alguma coisa, só consegui ver 2 frames, com duas imagens diferentes aparecendo em sequência.
    - Aumentando o limite de `r.x`, vejo só uma tela branca, às vezes também com o _flash_ de um retângulo.
    - Adicionando um Delay depois de cada `RenderPresent`, vejo a tela alternando entre uma tela branca e um retângulo.
    - Deixando o Delay depois só um dos `RenderPresent`, é como se só uma das metades do loop estivesse rodando.

2. Como fica evidente o uso de "double buffering"?
    - Fora da experimentação descrita no número 1, a presença de um buffer que é mostrado enquanto um segundo buffer é alterado é mais bem demonstrada desenhando o retângulo também no primeiro desenho, com uma cor diferente. É possível ver que aparecem dois retângulos sobrepostos, com o original mais avançado e por cima.
    - O efeito se pronuncia mais retirando o `RenderClear` de dentro do loop. Assim, podemos ver um par de sequências alternadas do retângulo novo e original, com os componentes trocando de lugar de forma clara a cada frame mas continuando "contínuos" individualmente.
    - Uma coisa que me confundiu no exemplo original era que ambos os frames adjacentes tinham o mesmo fundo branco. No exemplo que deixei na pasta desse exercício, os dois tem fundo preto, mesmo limpando a tela com branco no início. Eu esperaria que, com 2 buffers, considerando que o padrão parece ser preto, o primeiro frame fosse branco e o segundo preto. Minha hipótese é que `RenderClear` limpe os dois buffers, mas isso não explicaria tudo, só o exemplo original.
