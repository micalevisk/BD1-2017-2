<!-- https://gist.github.com/rxaviers/7360908 -->
# Parte V

Experimentar estratégias para utilização de transações e níveis de isolamento em SGBDs relacionais.
As tarefas envolvem uma simulação de um sistema de reservas de passagem aéreas.

Considere a seguinte tabela que registra os assentos reservados em um vôo:
```
Assentos(num_voo, disp)
```
> **num** é um `inteiro`  de 1 a 200 <br>
> **disp** é `booleano` onde se _true_ então o assento está vago (valor inicial = _true_)

A reserva de um assento é feita em três passos:
1. O sistema recupera a lista dos assentos disponíveis.
2. O cliente escolhe o assento. Esse passo deve ser simulado pela escolha aleatória de um dos assentos disponíveis, levando para isso um tempo de escolha de 1 segundo.
3. O sistema registra a reserva do assento escolhido, atualizando o valor de **disp** para _false_

Cada assento é reservado individualmente.
Duas versões diferentes do processo de reserva devem ser implementadas.

- **(versão a)** A reserva é implementada como uma única transação que inclui os três passos acima
- **(versão b)** A reserva inclui uma transação para o passo `1` e outra para o passo `3`. O passo `2` não faz parte das transações, mas deve ser executado.

Agentes de viagens são responsáveis por realizar as reserva de 200 clientes no total.
A atividade de um agente de viagens é simulada por uma _thread_.

Experimentos devem ser realizados simulando a atuação de _k_ agentes de viagem trabalhando simulataneamente, <br>
onde _k = 1,2,4,6,8_ e _10_. Cada agente/_thread_ faz uma reserva de cada vez. As _threads_ devem ser reiniciadas até que todos os 200 clientes tenham seus assentos reservados.

Dois conjuntos de experimentos devem ser feitos usando dois níveis de isolamento: _"read commited"_ e _"serializable"_.
Nos dois casos, o sistema deve ser configurado para realizar bloqueios a nível de tupla (linha).

<small>As implementações devem ser feitas em C++ ou Python 3.x usando o SGBD PostgreSQL</small>

| Número da Tarefa | Autor             | Finalizada?
|:----------------:|:-----------------:|:----------:|
| 24               | :tiger:           | :negative_squared_cross_mark:
| 25               | :tiger:           | :negative_squared_cross_mark:
| 26               | :bear:            | :negative_squared_cross_mark:
| 27               | :bear:            | :negative_squared_cross_mark:
