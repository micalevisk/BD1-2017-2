//
//  [artigo.hpp]
//  Created by Micael Levi and Victor Meireles on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//
//  Contém definições e estruturas relacionadas
//  ao tipo de registro que será lido da massa de dados.
//  Além de constantes comuns aos programas.
//


#ifndef ARTIGO_HPP
#define ARTIGO_HPP



#include "parametros.h"
#include "stringUtils.hpp"
#include <fstream>


// ============= PARÂMETROS DE ACORDO COM O TIPO DE REGISTRO ============= //
#define Artigo struct __Artigo
#define ARTIGO_SIZE (sizeof(Artigo)) // R
#define QTD_CAMPOS_ARTIGO 7

#ifndef TEST // caso seja o cenário real (especificado)
  #define ARTIGO_TITULO_MAX_SIZE 300
  #define ARTIGO_ATUALIZACAO_MAX_SIZE 19
  #define ARTIGO_ATORES_MAX_SIZE 1024
  #define ARTIGO_SNIPPET_MAX_SIZE 1024
#else // cenário para fins de teste
  // ajustando os tamanhos para que o bfr seja 12
  #define ARTIGO_TITULO_MAX_SIZE 100
  #define ARTIGO_ATUALIZACAO_MAX_SIZE 19
  #define ARTIGO_ATORES_MAX_SIZE 100
  #define ARTIGO_SNIPPET_MAX_SIZE 100
#endif

#define INVALID_FIELD "-1" // marcar que um campo não contém dados válidos, i.e., só lixo

typedef int(*getArtigoId_pfn)(const Artigo&);


/**
 * Descreve o tipo dos registros que serão lidos.
 * Está organizado de maneira a reduzir o "structure padding".
 */
struct __Artigo {// 2384 bytes no cenário real
  int id;
  int ano;
  int citacoes;

  char atualizacao[ARTIGO_ATUALIZACAO_MAX_SIZE+1];
  char titulo[ARTIGO_TITULO_MAX_SIZE+1];
  char autores[ARTIGO_ATORES_MAX_SIZE+1];
  char snippet[ARTIGO_SNIPPET_MAX_SIZE+1];
};



/**
 * Para printar os valores de um Artigo.
 *
 * @author Micael Levi
 * @date 2017-08-31
 */
std::ostream& operator <<(std::ostream& out, const Artigo& artigo){
  out << "ID: " << artigo.id << '\n'
      << "TITULO: " << artigo.titulo << '\n'
      << "ANO: " << artigo.ano << '\n'
      << "AUTORES: " << artigo.autores << '\n'
      << "CITACOES: " << artigo.citacoes << '\n'
      << "ATUALIZACAO: " << artigo.atualizacao << '\n'
      << "SNIPPET: " << artigo.snippet;
  return out;
}


/**
 * Retorna o id de um dado artigo (getter id).
 *
 * @param artigo O objeto alvo.
 * @return O valor do atributo 'id' do objeto alvo.
 *
 * @author Micael Levi
 * @date 2017-08-31
 */
int getArtigoId(const Artigo& artigo){
  return artigo.id;
}


/**
 * Função específica para o formato do CSV que será lido.
 * Retorna o valor de um campo lido,
 * i.e., contéudo sem as aspas duplas no início e fim (se tiver).
 *
 * @param inputStream A stream (aberta) pro arquivo CSV cujo o campo será lido.
 * @param delimiter O caractere que separa/delimita os campos.
 * @return O campo lido como uma sequência de caracteres; será igual a "-1" se não foi lido.
 *
 * @author Victor Meireles
 * @date 2017-09-01
 */
static char* getNextFieldFrom(std::istream& inputStream, const char& delimiter = ';'){
  char *field = new char[ARTIGO_SNIPPET_MAX_SIZE+1]; // o tamanho do maior campo é o do snippet
  bool isEvenQuotes = false; // flag para marcar se o número de aspas duplas é par
  unsigned long i = 0;

  for (char c; (c = inputStream.get())!=EOF; ) {
    if (!isEvenQuotes && c == delimiter) break;
    if (c != '\n' && c != '\r' && c != '"') field[i++] = c;
    if (c == '"') isEvenQuotes = !isEvenQuotes;
  }

  if (!i) strcpy(field, INVALID_FIELD); // registro inválido
  else field[i] = 0; // definir caractere nulo

  return field;
}


/**
 * Função específica para o formato do CSV que será lido.
 * Lê 1 registro, do tipo Artigo, de um arquivo texto CSV.
 *
 * @param inputStream A stream (aberta) pro arquivo CSV cujo o campo será lido.
 * @return O endereço do registro alocado, ou nullptr caso não tenha sido lido.
 *
 * @author Victor Meireles
 * @date 2017-09-01
 */
Artigo* getRecordArtigoFrom(std::istream& inputStream){
  char* firstField = getNextFieldFrom(inputStream);
  if (!strcmp(firstField, INVALID_FIELD)) return nullptr;

  Artigo* record = new Artigo;

  try {
    record->id = StringUtils::stringToInt(firstField);
  } catch (std::exception const& e) {
    return nullptr;
  }

  // strcpy(record->titulo, getNextFieldFrom(inputStream));
  StringUtils::stringToCharArray(getNextFieldFrom(inputStream), record->titulo, ARTIGO_TITULO_MAX_SIZE);

  // record->ano = atoi(getNextFieldFrom(inputStream));
  try {
    record->ano = StringUtils::stringToInt( getNextFieldFrom(inputStream) );
  } catch (std::exception const& e) {
    return nullptr;
  }

  // strcpy(record->autores, getNextFieldFrom(inputStream));
  StringUtils::stringToCharArray(getNextFieldFrom(inputStream), record->autores, ARTIGO_ATORES_MAX_SIZE);

  // record->citacoes = atoi(getNextFieldFrom(inputStream));
  try {
    record->citacoes = StringUtils::stringToInt( getNextFieldFrom(inputStream) );
  } catch (std::exception const& e) {
    return nullptr;
  }

  // strcpy(record->atualizacao, getNextFieldFrom(inputStream));
  StringUtils::stringToCharArray(getNextFieldFrom(inputStream), record->atualizacao, ARTIGO_ATUALIZACAO_MAX_SIZE);

  // strcpy(record->snippet, getNextFieldFrom(inputStream, '\n'));
  StringUtils::stringToCharArray(getNextFieldFrom(inputStream, '\n'), record->snippet, ARTIGO_SNIPPET_MAX_SIZE);

  return record;
}



#endif
