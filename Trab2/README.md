<!--
remover todos os diretórios __pycache__
no src, executar:
$ find . -name \*.pyc -delete -o -type d -name __pycache__ -delete
-->

## Dependência Funcional (DF)
- _união:_        Se **X &rarr; Y** e **X &rarr; Z**, então **X &rarr; YZ**
- _decomposição:_ Se **X &rarr; YZ**, então **X &rarr; Y** e **X &rarr; Z**
> DF trivial = o lado direito contém apenas atributos que também aparecem no lado esquerdo

## FORMAS NORMAIS

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

e.g.
| X | Y | A |
|:-:|:-:|:-:|
| x | u | a |
| x | v | a |
satisfaz **`DF X -> A`**, como `A` é diferente de `X`, uma relação assim só pode estar em FBNC se `X` for uma chave, caso contrário, seria um exemplo de redundância. Sendo assim, _u_ é igual a _v_, ou seja, as duas tuplas acima são idênticas. Além disso, como uma relação é definida como **conjunto** de tuplas, a situação acima não pode surgir.

> Todo campo de toda tupla registra uma informação que não pode ser inferida (usando-se apenas DFs) a partir dos valores presentes em todos os outros campos em todas as tuplas da instância da relação.


### &diams; 3FN (&sub; 2FN)
seja `R` um esquema de relação,<br>
seja `F` o conjunto de DFs dadas como válidas sobre `R`,<br>
seja `X` um subconjunto dos atributos de `R` e <br>
seja `A` um atributo de `R` <br>
Então, &forall; <kbd>DF X -> A</kbd> &isin; `F`, uma das seguintes declarações é verdadeira:
- `A` &isin; `X`; i.e., trata-se de uma DF trivial, ou
- `X` é uma superchave
- `A` faz parte de alguma chave de `R`

> chave = conjunto mínimo de atributos que determina exclusivamente todos os outros atributos


### 2FN (&sub; 1FN)

### 1FN
- o domínio de um atributo deve incluir apenas _valores atômicos_ e que o valor de qualquer atributo em uma tupla deve ser _único_; i.e., reprova relações dentro de relações ou relações como valores de atributo dentro de tuplas.
