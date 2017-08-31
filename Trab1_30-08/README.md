<!--
documentar
estruturas
e mais
-->


```
.
│
├── __tests__
├── docs
├── include
│   ├── artigo.h
│   ├── externalHash.hpp
│   └── stringUtils.hpp
└── src
    ├── program_findrec
    │   └── findrec.cpp
    ├── program_seek
    │   └── seek1.cpp
    └── program_upload
        └── upload.cpp
```

# Explicação Sobre o Projeto



## Compilar



### Programa `upload`
> **upload** `<path/to/all_data.csv>`

Fará a carga inicial da massa de testes para seu banco de dados que irá criar:
  + Arquivo de dados organizado por hashing
  + Arquivo de índice primário indexado por B-Tree
  + Arquivo de índice secundário indexado por B-Tree



### Programa `findrec`
> **findrec** `<ID>` `[path/to/data_file]`

Busca no arquivo de dados por um registro com o ID informado, se existir, e retorna os campos do registro, a quantidade de blocos lidos para encontrá-lo e a quantidade total de bloco do arquivo de dados.


### Programas `seek1` e `seek2`
> **seek1** `<ID>` `[path/to/data_file]`

Devolve o registro com ID igual ao informado, se existir, pesquisando através do arquivo de índice primário, mostrando todos os campos, a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de bloco do arquivo de índice primário.



> **seek2** `<TITULO>` `[path/to/data_file]`

Mostra os dados do registro que possua o Título igual ao informado, se existir, pesquisando através do arquivo de índice secundário, informando a quantidade de blocos lidos para encontrá-lo no arquivo de índice e a quantidade total de bloco do arquivo de índice secundário.



## Convenções de Código


### Template de cabeçalho de qualquer arquivo código
```java
//
//  [<%= filename >]
//  Created by <%= author_name > on <%= year >-<%= month >-<%= day >
//  Copyright (c) <%= year > <%= author_email >; All rights reserved.
//
//  <%= file_description >
//
```

### Template de cabeçalho de documentação _in-code_
```java
/**
 * {{descrição}}
 *
 * @param {{nomeParam}} {{descrição}}.
 * @return {{descrição}}.
 *
 * @author {{nome criador}}
 * @date {{ano}}-{{mês}}-{{dia}}
 */
```