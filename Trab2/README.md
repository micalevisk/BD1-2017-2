<!--
remover todos os diretórios __pycache__
no src, executar:
$ find . -name \*.pyc -delete -o -type d -name __pycache__ -delete
-->

# Esquema do Banco de Dados relacional
![relational_schema](https://user-images.githubusercontent.com/13461315/31106037-5cb72f34-a7b6-11e7-98ed-27cf14e58794.png)

## Dicionário de Dados (DD)


--
## Dependência Funcional (DF)
Tipo de Restrição de Integridade que generaliza o conceito de _chave_. <br>
Seja `R` um esquema de relação e `X` e `Y` conjuntos não-vazios de atributos em R.
Dizemos que uma instância `r` de `R` satisfaz a **DF X &rarr; Y** (X determina funcionalmente Y)
se o seguinte vale para todo par de tuplas <`t1`,`t2`> em `r`: <br>
Se `t1.X = t2.X`, então `t1.Y = t2.Y` <br>
onde ^~ significa "projeção da tupla `t1` nos atributos em `X`" <br>
Basicamente diz que se duas tuplas concordam nos valores dos atributos `X`,
elas também devem concordar nos valores dos atributos `Y`

- _união:_        Se **X &rarr; Y** e **X &rarr; Z**, então **X &rarr; YZ**
- _decomposição:_ Se **X &rarr; YZ**, então **X &rarr; Y** e **X &rarr; Z**
> - DF trivial = o lado direito contém apenas atributos que também aparecem no lado esquerdo
> - uma restrição de chave primária é um caso especial de DF, onde os atributos na chave desempenham o papel de _X_ e o conjunto de todos os atributos na relação desempenha o papel de _Y_


## Formas Normais

### FBNC (&sub; 3FN)
seja `R` um esquema de relação,<br>
seja `F` o conjunto de DFs dadas como válidas sobre `R`,<br>
seja `X` um subconjunto dos atributos de `R` e <br>
seja `A` um atributo de `R` <br>
Então, &forall; <kbd>DF X -> A</kbd>  &isin; `F`, uma das seguintes declarações é verdadeira:
- `A` &isin; `X`; i.e., trata-se de uma DF trivial, ou
- `X` é uma superchave

> Assim, as únicas dependências não-triviais são aquelas nas quais uma chave determina algum(ns) atributo(s). <br>
> Portanto, cada tupla pode ser considerada como uma entidade ou relacionamento, identificado por uma chave e descrito pelos atributos restantes.

e.g.:
| X | Y | A |
|:-:|:-:|:-:|
| x | u | a |
| x | v | a |
satisfaz **`DF X -> A`**, como `A` é diferente de `X`, uma relação assim só pode estar em FBNC se `X` for uma chave, caso contrário, seria um exemplo de redundância. Sendo assim, _u_ é igual a _v_, ou seja, as duas tuplas acima são idênticas. Além disso, como uma relação é definida como **conjunto** de tuplas, a situação acima não pode surgir.

> Todo campo de toda tupla registra uma informação que não pode ser inferida (usando-se apenas DFs) a partir dos valores presentes em todos os outros campos em todas as tuplas da instância da relação.


### 3FN (&sub; 2FN) <small><small>(adotada no projeto)</small></small>
seja `R` um esquema de relação,<br>
seja `F` o conjunto de DFs dadas como válidas sobre `R`,<br>
seja `X` um subconjunto dos atributos de `R` e <br>
seja `A` um atributo de `R` <br>
Então, &forall; <kbd>DF X -> A</kbd> &isin; `F`, uma das seguintes declarações é verdadeira:
- `A` &isin; `X`; i.e., trata-se de uma DF trivial, ou
- `X` é uma superchave, ou
- `A` faz parte de alguma chave de `R`

> chave = conjunto mínimo de atributos que determina exclusivamente todos os outros atributos <br>
> dependência parcial = `X` é um subconjunto de alguma chave `K` <br>
> dependência transitiva = `X` não é um subconjunto de nenhuma chave <br>
> As dependências parciais e transitivas não são permitidas. Em suma, `R` não deve ter um atributo não-chave determinado funcionalmente por outro atributo não-chave (ou por um conjunto de atributos não-chave)


### 2FN (&sub; 1FN)
A única diferença em relação a 3FN é que as dependências transitivas são permitidas.

### 1FN
- o domínio de um atributo deve incluir apenas _valores atômicos_ e que o valor de qualquer atributo em uma tupla deve ser _único_; i.e., reprova relações dentro de relações ou relações como valores de atributo dentro de tuplas.
