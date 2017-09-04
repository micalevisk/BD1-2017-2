/*
 * Copyright (c) 2017 All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Created by Micael Levi and Victor Meireles on 2017-08-11
 *
 * Contém definições e estruturas relacionadas
 * ao tipo de registro que será lido da massa de dados.
 * Além de constantes comuns aos programas.
 */


#ifndef ARTIGO_HPP
#define ARTIGO_HPP



#include "parametros.h"
#include "stringUtils.hpp"
#include <fstream>


// ============= PARÂMETROS DE ACORDO COM O TIPO DE REGISTRO ============= //
#define Artigo struct __Artigo
#define ARTIGO_SIZE (sizeof(Artigo)) // R

#ifndef TEST // caso seja o cenário real (especificado)
  #define ARTIGO_TITULO_MAX_SIZE 300
  #define ARTIGO_ATUALIZACAO_MAX_SIZE 19
  #define ARTIGO_AUTORES_MAX_SIZE 1024
  #define ARTIGO_SNIPPET_MAX_SIZE 1024
#else // cenário para fins de teste
  // ajustando os tamanhos para que o bfr seja 12
  #define ARTIGO_TITULO_MAX_SIZE 100
  #define ARTIGO_ATUALIZACAO_MAX_SIZE 19
  #define ARTIGO_AUTORES_MAX_SIZE 100
  #define ARTIGO_SNIPPET_MAX_SIZE 100
#endif

#define INVALID_FIELD "-1" // marcar que um campo não contém dados válidos, i.e., só lixo

typedef int(*getArtigoId_pfn)(const Artigo&);


/**
 * Descreve o tipo dos registros que serão lidos.
 * Está organizado de maneira a reduzir o "structure padding".
 */
struct __Artigo {// 2384 bytes no cenário real
  int id, ano, citacoes;
  char atualizacao[ARTIGO_ATUALIZACAO_MAX_SIZE+1];
  char titulo[ARTIGO_TITULO_MAX_SIZE+1];
  char autores[ARTIGO_AUTORES_MAX_SIZE+1];
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
 * Apaga o conteúdo de uma cadeia de caracteres se tiver valor inválido,
 * i.e., adiciona o caractere nulo no primeiro byte da cadeia.
 *
 * @param str A string alvo
 *
 * @author Micael Levi
 * @date 2017-09-03
 */
static void clearFieldIfInvalid(char* field){
  if (!field || std::strlen(field) < 0) return;
  if (!strcmp(field, INVALID_FIELD)) field[0] = '\0';
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
Artigo* getRecordArtigoFrom(std::istream& inputStream){ // FIXME alguns registros ficam com titulo misturado com autores
  char* firstField = getNextFieldFrom(inputStream);
  if (!strcmp(firstField, INVALID_FIELD)) return nullptr;

  Artigo* record = new Artigo;

  try {
    record->id = StringUtils::stringToInt(firstField);
  } catch (std::exception const& e) {
    return nullptr;
  }

  StringUtils::strcpy(record->titulo, getNextFieldFrom(inputStream), ARTIGO_TITULO_MAX_SIZE);
  clearFieldIfInvalid(record->titulo);

  try {
    record->ano = StringUtils::stringToInt( getNextFieldFrom(inputStream) );
  } catch (std::exception const& e) {
    return nullptr;
  }

  StringUtils::strcpy(record->autores, getNextFieldFrom(inputStream), ARTIGO_AUTORES_MAX_SIZE);
  clearFieldIfInvalid(record->autores);

  try {
    record->citacoes = StringUtils::stringToInt( getNextFieldFrom(inputStream) );
  } catch (std::exception const& e) {
    return nullptr;
  }

  StringUtils::strcpy(record->atualizacao, getNextFieldFrom(inputStream), ARTIGO_ATUALIZACAO_MAX_SIZE);
  clearFieldIfInvalid(record->atualizacao);

  StringUtils::strcpy(record->snippet, getNextFieldFrom(inputStream, '\n'), ARTIGO_SNIPPET_MAX_SIZE);
  clearFieldIfInvalid(record->snippet);


  return record;
}



#endif
