O programa segue o que essa máquina de estados descreve:
![autômato](autômato.png)

Legenda:
- formato das transições: {símbolo: ação[, ação]\*} ou {símbolo[, símbolo]\*}
- símbolos:
    - esc: apertar ESC no teclado
    - movimento: mover o mouse
    - clique: iniciar um clique (dentro do retângulo em questão)
    - clique fora: iniciar um clique (fora do retângulo em questão)
    - soltar: terminar um clique
- ações:
    - salvar: salva os dados do clique e a diferença entre a origem do retângulo e o clique
    - reset: coloca o retângulo de volta na posição salva
    - emit: dispara um evento de clique completo no retângulo

O "jogo" consiste em algumas cartas empilháveis, inicialmente pretas.
Enquanto se segura alguma, ela é trazida ao topo e fica azul.
Enquanto se arrasta, ela fica vermelha.

Ao fazer um clique numa carta, ela muda de cor permanentemente.
Clicando mais vezes, a cor muda em ciclo, mas sem voltar a cor preta inicial.

Pode-se cancelar o clique completo ou o arrasto apertando ESC.

