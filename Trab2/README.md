<img src="http://icomp.ufam.edu.br/professores/images/ufam.png" width="150" align="right">

### Trabalho Prático 2 - Banco de Dados Relacional
> 11/09/2017 ⇒ 03/10/2017

----------------------------------------

**Aluno1:** Micael Levi &nbsp;&nbsp; &horbar; 21554923 <br>
**Aluno2:** Victor Roque &horbar; 21555174 <br>

----------------------------------------

```
src
├── create_database
│   ├── __init__.py
│   ├── db_connection.py
│   ├── models
│   │   ├── __init__.py
│   │   ├── categories.py
│   │   ├── product_categories.py
│   │   ├── products.py
│   │   ├── reviews.py
│   │   └── similars.py
│   └── parser
│       ├── __init__.py
│       ├── parse.py
│       └── utils.py
└── createDatabase.py
```

# Execução

## pré-requisitos
- [PostgreSQL](https://www.postgresql.org/)
- [Python 3.x](https://www.python.org/download/releases/3.0/)
  - módulo <kbd>psycopg2</kbd>
  - módulo <kbd>getopt</kbd>
- deve existir um _schema_ de nome **"public"** e o usuário (a ser passado) deve ter permissões para realizar um CRUD sobre ele

---

No diretório `src`, execute em na linha de comandos:
```bash
python3 createDatabase.py -i <caminho/para/a/entrada.txt> -d <nome_do_banco_existente> -u <usuário> -p <senha>
```

Para apagar os diretórios `__pycache__` (gerados após a execução), basta executar:
```bash
find . -name \*.pyc -delete -o -type d -name __pycache__ -delete
```

# Esquema do Banco de Dados relacional
<img src="https://user-images.githubusercontent.com/13461315/31156622-9495c5d2-a883-11e7-9e18-fbf1541a0af5.png" alt="relational schema" width="800" align="center">

<!-- ![relational_schema](https://user-images.githubusercontent.com/13461315/31156622-9495c5d2-a883-11e7-9e18-fbf1541a0af5.png) -->

<details>
<summary>Dicionário de Dados (DD)</summary>
<!-- TOC ->

<!-- fonte http://www.luis.blog.br/dicionario-de-dados.aspx -->

<table>
  <caption>products</caption>
  <tr>
    <th>Atributo</th>
    <th>Classe</th>
    <th>Domínio</th>
    <th>Tamanho</th>
    <th>Descrição</th>
  </tr>

  <tr>
  <td>asin</td>
  <td>determinante</td>
  <td>texto</td>
  <td>10</td>
  <td>Número de identificação padrão da Amazon</td>
  </tr>

  <tr>
  <td>id</td>
  <td>simples</td>
  <td>numérico</td>
  <td></td>
  <td>Identificador único do produto</td>
  </tr>

  <tr>
  <td>title</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>500</td>
  <td>Título ou nome do produto</td>
  </tr>

  <tr>
  <td>name_group</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>5</td>
  <td>Grupo do produto (Book, DVD, Video ou Music)</td>
  </tr>

  <tr>
  <td>salesrank</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>5</td>
  <td>Amazon Salesrank</td>
  </tr>

  <tr>
  <td>count_similars</td>
  <td>multivalorado</td>
  <td>númerico</td>
  <td></td>
  <td>Quantidade de produtos semelhantes</td>
  </tr>

  <tr>
  <td>count_categories</td>
  <td>multivalorado</td>
  <td>númerico</td>
  <td></td>
  <td>Quantidade de categorias em que ele pertence</td>
  </tr>

  <tr>
  <td>count_total_reviews</td>
  <td>multivalorado</td>
  <td>numérico</td>
  <td></td>
  <td>Quantidade total de comentários no produto</td>
  </tr>
</table>

<table>
  <caption>similars</caption>
  <tr>
    <th>Atributo</th>
    <th>Classe</th>
    <th>Domínio</th>
    <th>Tamanho</th>
    <th>Descrição</th>
  </tr>

  <tr>
  <td>asin</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>10</td>
  <td>Número de identificação padrão da Amazon</td>
  </tr>

  <tr>
  <td>asin_similar</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>10</td>
  <td>ASIN do produto semelhante</td>
  </tr>
</table>

<table>
  <caption>reviews</caption>
  <tr>
    <th>Atributo</th>
    <th>Classe</th>
    <th>Domínio</th>
    <th>Tamanho</th>
    <th>Descrição</th>
  </tr>

  <tr>
  <td>asin</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>10</td>
  <td>Número de identificação padrão da Amazon</td>
  </tr>

  <tr>
  <td>time</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>10</td>
  <td>Data em que o comentário foi realizado</td>
  </tr>

  <tr>
  <td>cutomer</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td>15</td>
  <td>Identificador do usuário que comentou</td>
  </tr>

  <tr>
  <td>rating</td>
  <td>multivalorado</td>
  <td>numérico</td>
  <td></td>
  <td>Avaliação dada pelo usuário</td>
  </tr>

  <tr>
  <td>votes</td>
  <td>multivalorado</td>
  <td>numérico</td>
  <td></td>
  <td>Número total de votos no comentário</td>
  </tr>

  <tr>
  <td>helpful</td>
  <td>multivalorado</td>
  <td>numérico</td>
  <td></td>
  <td>Avaliação de utilidade do comentário</td>
  </tr>
</table>

<table>
  <caption>categories</caption>
  <tr>
    <th>Atributo</th>
    <th>Classe</th>
    <th>Domínio</th>
    <th>Tamanho</th>
    <th>Descrição</th>
  </tr>

  <tr>
  <td>cid</td>
  <td>determinante</td>
  <td>numérico</td>
  <td></td>
  <td>Identificador único da categoria</td>
  </tr>

  <tr>
  <td>cname</td>
  <td>simples</td>
  <td>texto</td>
  <td>500</td>
  <td>Nome da categoria</td>
  </tr>

  <tr>
  <td>super_cid</td>
  <td>multivalorado</td>
  <td>texto</td>
  <td></td>
  <td>Identificador da categoria que está no nível acima</td>
  </tr>
</table>

<table>
  <caption>product_categories</caption>
  <tr>
    <th>Atributo</th>
    <th>Classe</th>
    <th>Domínio</th>
    <th>Tamanho</th>
    <th>Descrição</th>
  </tr>

  <tr>
  <td>asin</td>
  <td>simples</td>
  <td>texto</td>
  <td>10</td>
  <td>Número de identificação padrão da Amazon</td>
  </tr>

  <tr>
  <td>leaf_cid</td>
  <td>simples</td>
  <td>numérico</td>
  <td></td>
  <td>Identificador da categoria em que o produto com o "asin" acima está</td>
  </tr>
</table>

<!-- /TOC -->
</details>

---

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
A forma normal utilizada no desenvolvimento do esquema de relação do BD foi a **Terceira**. Abaixo segue uma visão geral sobre as principais.

### FBNC (&sub; 3FN)
seja `R` um esquema de relação,<br>
seja `F` o conjunto de DFs dadas como válidas sobre `R`,<br>
seja `X` um subconjunto dos atributos de `R` e <br>
seja `A` um atributo de `R` <br>
Então, &forall; `DF X -> A`  &isin; `F`, uma das seguintes declarações é verdadeira:
- `A` &isin; `X`; i.e., trata-se de uma DF trivial, ou
- `X` é uma superchave

> Assim, as únicas dependências não-triviais são aquelas nas quais uma chave determina algum(ns) atributo(s). <br>
> Portanto, cada tupla pode ser considerada como uma entidade ou relacionamento, identificado por uma chave e descrito pelos atributos restantes.

e.g.:
| X | Y | A |
|---|---|---|
| x | u | a |
| x | v | a |
satisfaz **`DF X -> A`**, como `A` é diferente de `X`, uma relação assim só pode estar em FBNC se `X` for uma chave, caso contrário, seria um exemplo de redundância. Sendo assim, _u_ é igual a _v_, ou seja, as duas tuplas acima são idênticas. Além disso, como uma relação é definida como **conjunto** de tuplas, a situação acima não pode surgir.

> Todo campo de toda tupla registra uma informação que não pode ser inferida (usando-se apenas DFs) a partir dos valores presentes em todos os outros campos em todas as tuplas da instância da relação.


### 3FN (&sub; 2FN) <small><small>(adotada no projeto)</small></small>
seja `R` um esquema de relação,<br>
seja `F` o conjunto de DFs dadas como válidas sobre `R`,<br>
seja `X` um subconjunto dos atributos de `R` e <br>
seja `A` um atributo de `R` <br>
Então, &forall; `DF X -> A` &isin; `F`, uma das seguintes declarações é verdadeira:
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
