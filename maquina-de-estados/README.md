Tabela de Transições — Máquina de Estados do Jogador
| Estado Atual | Próximo Estado | Condição de Transição       |
|--------------|----------------|----------------------------|
| Parado       | Movendo        | Pressiona tecla direcional |
| Movendo      | Parado         | Solta as teclas direcionais|
| Movendo      | Movendo        | Pressiona tecla direcional |
| Movendo      | Coletando      | Coleta as joias            |
| Movendo      | Vitória        | Coleta o cálice            |
| Parado       | Coletando      | Começa a coletar           |
| Coletando    | Parado         | Termina de coletar         |
| Vitória      | Vitória        | —                          |


