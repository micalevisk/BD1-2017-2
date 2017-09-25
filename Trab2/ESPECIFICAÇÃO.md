# Trabalho Prático 2: Banco de Dados Relacional
> 11/09/2017 ⇒ 02/10/2017

## Apresentação
O objetivo deste trabalho prático é <u>projetar</u> e <u>implementar</u> um banco de dados sobre produtos vendidos em uma loja de comércio eletrônico (_e-commerce_), incluindo avaliações e comentários de usuários sobre estes produtos.

O trabalho consiste em:
- criar um [Banco de Dados Relacional][wiki-bdr] contendo dados sobre compras de produtos
- elaboração de um [Dashboard][wiki-dashboard] (painel de monitoramento dos dados de compra, gerando um série de relatórios e gráficos)

Os dados para o banco de dados serão fornecidos de um arquivo (texto) de entrada, descrito abaixo.

## Arquivo de Entrada
O arquivo de onde serão extraídos os dados de entrada será o "[Amazon product co-purchasing network metadata][dados-entrada]".
Os dados foram coletados em 2006 do site Amazon e informações de produto e comentários de clientes sobre 548.552 produtosd diferentes.
Para cada produto, a seguinte informação PODE estar disponível:

+ Título [_title_]
+ Grupo [_group_]
+ Posição no ranking de vendas [_salesrank_]
+ Lista de produtos "similares" [_similiar_] (que foram adquiridos junto com o produto; a quantidade seguida pelos ASINs)
+ Informação de categorização do produto [_categories_] - Categorias e subcategorias ao qual o produto pertence
+ Comentários sobre os produtos [_reviews_]:
  - Total de comentários [_total_]
  - Total de downloads [_downloaded_]
  - Média dos votos [_avg rating_]
  - Informação data (início da linha)
  - id do cliente [_cutomer_]
  - classificação [_rating_]
  - números de votos [_votes_]
  - o número de pessoas que acharam a avaliação útil [_helpful_]

```
Id:   15
ASIN: 1559362022
  title: Wake Up and Smell the Coffee
  group: Book
  salesrank: 518927
  similar: 5  1559360968  1559361247  1559360828  1559361018  0743214552
  categories: 3
   |Books[283155]|Subjects[1000]|Literature & Fiction[17]|Drama[2159]|United States[2160]
   |Books[283155]|Subjects[1000]|Arts & Photography[1]|Performing Arts[521000]|Theater[2154]|General[2218]
   |Books[283155]|Subjects[1000]|Literature & Fiction[17]|Authors, A-Z[70021]|( B )[70023]|Bogosian, Eric[70116]
  reviews: total: 8  downloaded: 8  avg rating: 4
    2002-5-13  cutomer: A2IGOA66Y6O8TQ  rating: 5  votes:   3  helpful:   2
    2002-6-17  cutomer: A2OIN4AUH84KNE  rating: 5  votes:   2  helpful:   1
    2003-1-2  cutomer: A2HN382JNT1CIU  rating: 1  votes:   6  helpful:   1
    2003-6-7  cutomer: A2FDJ79LDU4O18  rating: 4  votes:   1  helpful:   1
    2003-6-27  cutomer: A39QMV9ZKRJXO5  rating: 4  votes:   1  helpful:   1
    2004-2-17  cutomer:  AUUVMSTQ1TXDI  rating: 1  votes:   2  helpful:   0
    2004-2-24  cutomer: A2C5K0QTLL9UAT  rating: 5  votes:   2  helpful:   2
    2004-10-13  cutomer:  A5XYF0Z3UH4HB  rating: 5  votes:   1  helpful:   1
```
> Data format:

+ **Id**: Product id (number 0, ..., 548551)
+ **ASIN**: Amazon Standard Identification Number (postive)
+ **title**: Name/title of the product
+ **group**: Product group (Book, DVD, Video or Music)
+ **salesrank**: Amazon Salesrank
+ **similar**: ASINs of co-purchased products (people who buy X also buy Y)
+ **categories**: Location in product category hierarchy to which the product belongs (separated by |, category id in [])
+ **reviews**: Product review information: time, user id, rating, total number of votes on the review, total number of helpfulness votes (how many people found the review to be helpful)
---


## Sobre o Esquema do Banco de Dados
O esquema de bancos de dados a ser desenvolvido deverá seguir o modelo relacional.
Seu desenvolvimento deverá seguir a técnica ascendente (_bottom-up_) de projeto de banco de dados relacionais e deve necessariamente observar as regras de uma das formas normais de alto nível tais como a **Forma Normal de Boyce-Codd**, **Terceira Forma Normal** ou **Quarta Forma Normal**.

## Sobre o Dashboard
O painel de bordo a ser implementado deve dar suporte a pelo menos as seguintes consultas, as quais devem todas ser implementadas com consultadas em linguagem SQL:

<ol type="a">
 <li>Dado produto, listar os 5 comentários mais úteis e com maior avaliação e os 5 comentários mais úteis e com menor avaliação</li>
 <li>Dado um produto, listar os produtos similiares com maiores vendas do que ele</li>
 <li>Dado um produto, mostrar a evolução diária das médias de avaliação ao longo do intervalo de tempo coberto no arquivo de entrada</li>
 <li>Listar os 10 produtos líderes de venda em cada grupo de produtos</li>
 <li>Listar os 10 produtos com a maior média de avaliações úteis positivas por produto</li>
 <li>Listar as 5 categorias de produto com a maior média de avaliações úteis positivas por produto</li>
 <li>Listar os 10 clientes que mais fizeram comentários por grupo de produto</li>
</ol>

## O que Entregar
Documentação apresentando um diagrama correspondendo ao esquema do Banco de Dados relacional;
além de um dicionário de dados descrevendo cada relação, atributo, restrição de integridade referencial (ou outro tipo de restrição que fizer parte do esquema do banco de dados).
Deve ser entregue em um único arquivo PDF nomeado `TP2_2.1_MicaelLevi_VictorRoque.pdf`

Código fonte dos programas desenvolvidos para extração de dados do arquivo de entrada, criação do esquema de dados e povoamento das relações com estes dados.
Deve ser entregue em um único arquivo ZIP nomeado `TP2_2.2_MicaelLevi_VictorRoque.zip`

Código fonte do Dashboard implementando no mínimo as consultas descritas na seção acima. Podendo conter gráficos relativos à elas.
Deve ser entregue em um único arquivo ZIP nomeado `TP2_2.3_MicaelLevi_VictorRoque.zip`

Os códigos devem ser implementados em C++/Python e os executáveis gerados devem receber como parâmetros de entrada os dados de conexão a um banco de dados *PostgreSQL*,
ou seja, o endereço do servidor (e porta), usuário e senha, nome do banco de dados.
Drivers disponíveis para a conexão: JDBC e ODBC.

## Como Entregar
Cada um dos 3 arquivos acima deve ser copiado em uma pasta do Google Drive que ficará aberta até às 23:59 do dia 02/10/2017


<!-- REFERENCES -->
[wiki-bdr]: https://pt.wikipedia.org/wiki/Banco_de_dados_relacional  "Definição de Banco de Dados Relacional"
[wiki-dashboard]: https://pt.wikipedia.org/wiki/Painel_de_bordo "Definição de Dashboard"
[dados-entrada]: https://snap.stanford.edu/data/amazon-meta.html "Stanford Network Analysis Project"
