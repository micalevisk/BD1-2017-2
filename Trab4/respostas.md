<img src="http://icomp.ufam.edu.br/professores/images/ufam.png" width="150" align="right">

### BD1 &horbar; Trabalho Prático 4
1. **Micael Levi** &horbar; 21554923
2. **Moisés Gomes** &horbar; 21550188

----------------------------------------

#### Questão 1
> Enriqueça o texto identificando possíveis lacunas de informação. Faça hipóteses razoáveis quando for o caso para preenchê-las. Para as questões seguintes, estas suas hipóteses devem também serem consideradas.

- o número do INSS de um segurado é único e de seus dependentes devem "herdar" esse identificador
- um segurado pode ter vários dependentes
- certos diagnósticos não podem serem dados à pessoas de qualquer idade
- cada diagnóstico deve ter especificado o gênero e a faixa de idade do paciente que pode receber tal diagnóstico
- dado um diagnóstico, nem todos os tratamentos necessariamente serão aplicados
- dado um tratamento, nem todos os materiais e serviços serão utilizados/aplicados
- cada tratamendo deve especificar os serviços necessários e a quantidade máxima (útil) de cada material
- um mesmo serviço pode ser aplicado em diferentes tratamentos
- um mesmo material pode ser aplicado em diferentes tratamentos
- o pagamento do INSS é feito pelo próprio segurado se ele for autônomo ou aposentado
- o INSS não precisa pagar o valor do tratamento de pacientes em clínicas e hospitais da própria rede
- o valor do tratamento é dado pelo custo dos serviços realizados no tratamento

### Questão 2
> Projete um esquema entidade-relacionamento para este sistema. Desenhe um diagrama ER, descreva os atributos e também restrições semânticas adicionais que não puderem ser descritas em termos do Modelo ER estendido. Este diagrama deve usar a notação do Modelo ER descrita no livro de _Elmasri-Navathe_, vista em sala aula.

<!-- problema: não representa um trabalhor autônomo e nem uma rede privada -->
![diagrama_er](resposta_questao2.png)
