# Trabalho Prático 1: Implementação para Armazenamento e Pesquisa de dados
> 37/07/2017 ⇒ 30/08/2017

<details>
<summary>Tópicos</summary>
<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:0 orderedList:0 -->

* [Apresentação](#apresentação)
* [Descrição do trabalho](#descrição-do-trabalho)
* [O que entregar](#o-que-entregar)
* [Como entregar](#como-entregar)

<!-- /TOC -->
</details>


## Apresentação

Este trabalho consiste na implementação para armazenamento e pesquisa de dados a partir de uma massa de dados a ser fornecido em um arquivo texto formatado. <br>
Os programas devem fornecer suporte para inserção dos dados que serão fornecidos, assim como diferentes meios de pesquisa, seguindo as técnicas apresentadas nas aulas de organização e indexação de arquivos. <br>
O trabalho deve ser implementado em linguagem C++ utilizando as bibliotecas padrão de chamadas de sistemas disponíveis no Linux.

## Descrição do trabalho
O arquivo de entrada contém registros de dados sobre artigos científicos publicados em conferências. A estrutura deste arquivo é a seguinte:

| Campo         | Tipo          | Descrição  |
|:-------------:|:--------------|:-----------|
| ID            | `inteiro`     | código identificador do artigo                         |
| Título        | `alfa 300`    | título do artigo                                       |
| Ano           | `inteiro`     | ano de publicação do artigo                            |
| Autores       | `alfa 1024`   | lista dos autores do artigo (nomes separados por `\|`)  |
| Citações      | `inteiro`     | número de vezes que o artigo foi citado                |
| Atualização   | `data e hora` | data e hora da última atualização dos dados (`AAAA-MM-DD hh:mm:ss`)           |
| Snippet       | `alfa 1024`   | resumo textual dos dados do artigo                     |

#### Exemplo (ignore os `\n`)
```
"1";
"Poster: 3D sketching and flexible input for surface design: A case study.";
"2013";
"Anamary Leal|Doug A. Bowman";
"0";
"2016-07-28 09:36:29";
"Poster: 3D sketching and flexible input for surface design: A case study. A Leal, DA Bowman -  Interfaces (3DUI), 2013 IEEE Symposium , 2013 - ieeexplore.ieee.org. ABSTRACT Designing three-dimensional (3D) surfaces is difficult in both the physical world  and in 3D modeling software, requiring background knowledge and skill. The goal of this  work is to make 3D surface design easier and more accessible through natural and  .."
```

Os seguintes programas devem ser implementados: <br>
- `upload` _<file\>_ <br>
Programa que fará a carga inicial da massa de testes para seu banco de dados que irá criar:
  + Arquivo de dados organizado por hashing
  + Arquivo de índice primário usando B-Tree
  + Arquivo de índice secundário usando B-Tree

- `findrec` _<ID\>_ <br>
Busca no arquivo de dados por um registro com o ID informado, se existir, e retorna os campos do registro, a quantidade de blocos lidos para encontrá-lo e a quantidade total de bloco do arquivo de dados;

- `seek1` _<ID\>_ <br>
Devolve o registro com ID igual ao informado, se existir, pesquisando através do arquivo de índice primário, mostrando todos os campos, a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de bloco do arquivo de índice primário;

- `seek2` _<Titulo\>_ <br>
Mostra os dados do registro que possua o Título igual ao informado, se existir, pesquisando através do arquivo de índice secundário, informando a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de bloco do arquivo de índice secundário;

## O que entregar
- Os arquivos-fonte dos programas comentados, juntamente com o arquivo `Makefile` para que possam ser compilados e testados. <br>
Devem ser entregues em um único arquivo **ZIP** nomeado como `TP1_1_MicaelLevi_VictorRoque.zip`
- A documentação do projeto dos programas, deve ser entregue em um único arquivo **PDF** nomeado como `TP1_2_MicaelLevi_VictorRoque.pdf` <br>
Registrando todas as decisões de projeto tomada, incluindo:
  + A estrutura de cada arquivo de dados e índices
  + Quais fontes formam cada programa
  + As funções que cada fonte contém
  + Quem desenvolveu cada fonte/função
  + Qual o papel de cada função


## Como entregar
Os dois arquivos acima devem ser copiados em uma pasta do Google Drive que ficará aberta até **23:59** do dia **30/08/2017**.


<!--
## method_name ⇒ `return`
> description to use

`app.method_name(arg1[, arg2], callback)`

| Param     | Type                | Description                   |
|:---------:|:-------------------:|:-----------------------------:|
| arg1      | `String`\|`Integer` | how&when use & example        |
| arg2      | `String[]`          | how&when use & example 2      |
-->

---

## Exemplo de código usando o [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) e o [JavaDoc Style](http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html#docexamples)

```c++
/**
 *  A test class. A more elaborate class description.
 *  @author Micael Levi, Victor Roque
 *  @date 07/31/2017
 */
class Javadoc_Test
{
  public:

    /**
     * An enum.
     * More detailed enum description.
     */
    enum TEnum {
          TVal1, /**< enum value TVal1. */
          TVal2, /**< enum value TVal2. */
          TVal3  /**< enum value TVal3. */
         }
       *enumPtr, /**< enum pointer. Details. */
       enumVar;  /**< enum variable. Details. */

      /**
       * A constructor.
       * A more elaborate description of the constructor.
       */
      Javadoc_Test();

      /**
       * A destructor.
       * A more elaborate description of the destructor.
       */
     ~Javadoc_Test();

      /**
       * a normal member taking two arguments and returning an integer value.
       * @author Micael Levi
       * @date 07/31/2017
       * @param a an int argument.
       * @param s a constant character pointer.
       * @see Javadoc_Test()
       * @see ~Javadoc_Test()
       * @see testMeToo()
       * @see publicVar()
       * @return The test results as int.
       */
       int testMe(int a, const char *s) {
         if (a) {
           return a;
         } else if (!a) {
           return 23;
         }
         return 0;
       }

      /**
       * A pure virtual member.
       * @author Victor Roque
       * @date 07/31/2017
       * @see testMe()
       * @param c1 the first argument.
       * @param c2 the second argument.
       */
       virtual void testMeToo(char c1, char c2) = 0;

      /**
       * a public variable.
       * Details.
       */
       int publicVar;

      /**
       * a function variable.
       * Details.
       */
       int (*handler)(int a,int b);
};
```
