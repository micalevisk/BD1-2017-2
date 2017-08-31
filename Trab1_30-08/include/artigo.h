//
//  [artigo.hpp]
//  Created by Micael Levi and Victor Meireles on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//
//  Contém definições e estruturas relacionadas
//  ao tipo de registro que será lido da massa de dados.
//


#ifndef ARTIGO_HPP
#define ARTIGO_HPP



#include <fstream>

#define DIR_ARQUIVOS_CRIADOS "../../docs/"

#define ARTIGO_SIZE (sizeof(Artigo)) // R
#define QTD_CAMPOS_ARTIGO 7

// ============= PARÂMETROS DE ACORDO COM O TIPO DE REGISTRO ============= //
#ifdef _REAL_ // caso seja o cenário real (especificado)
  #define ARTIGO_TITULO_MAX_SIZE 300
  #define ARTIGO_ATUALIZACAO_MAX_SIZE 19
  #define ARTIGO_ATORES_MAX_SIZE 1024
  #define ARTIGO_SNIPPET_MAX_SIZE 1024
#else // cenário para fins de teste
  #undef QTD_BUCKETS
  #define QTD_BUCKETS 128000 // causará 11 colisões, i.e., 1 bloco precisa ter espaço pra 12 registros

  // ajustando os tamanhos para que o bfr seja 12
  #define ARTIGO_TITULO_MAX_SIZE 100
  #define ARTIGO_ATUALIZACAO_MAX_SIZE 19
  #define ARTIGO_ATORES_MAX_SIZE 100
  #define ARTIGO_SNIPPET_MAX_SIZE 100
#endif


/**
 * Descreve o tipo dos registros que serão lidos.
 * Está organizado de maneira a reduzir o "structure padding".
 */
struct Artigo {//2384 bytes
  int id;//4
  int ano;//4
  int citacoes;//4

  char atualizacao[ARTIGO_ATUALIZACAO_MAX_SIZE+1];//19 (AAAA-MM-DD hh:mm:ss)
  char titulo[ARTIGO_TITULO_MAX_SIZE+1];//alfa 300
  char autores[ARTIGO_ATORES_MAX_SIZE+1];//alfa 1024
  char snippet[ARTIGO_SNIPPET_MAX_SIZE+1];//alfa 1024
};



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


// ---------------------------------------------------------------------------------------- //
template<const char delimiter> std::string getFieldFrom(std::ifstream&, bool&);
// ---------------------------------------------------------------------------------------- //


Artigo* getRecordArtigoFrom(std::ifstream& inputStream){ // leitura específica para o tipo Artigo
  bool algumRegistroValido = false;
  Artigo* record = new Artigo;

  for (unsigned qtdCamposLidos=0; qtdCamposLidos < QTD_CAMPOS_ARTIGO; ) {
    bool campoValido = false;
    std::string campoCurr = getFieldFrom<';'>(inputStream, campoValido);

    if (!campoValido ) {
      if (inputStream.eof()) break;
      continue;
    }

    algumRegistroValido = true;
    switch (qtdCamposLidos) {
      case 0:
        record->id = StringUtils::removeDoubleQuotesAndParseInt(campoCurr);
        break;

      case 1:
        StringUtils::stringToCharArray(campoCurr, record->titulo, ARTIGO_TITULO_MAX_SIZE);
        break;

      case 2:
        record->ano = StringUtils::removeDoubleQuotesAndParseInt(campoCurr);
        break;

      case 3:
        StringUtils::stringToCharArray(campoCurr, record->autores, ARTIGO_ATORES_MAX_SIZE);
        break;

      case 4:
        record->citacoes = StringUtils::removeDoubleQuotesAndParseInt(campoCurr);
        break;

      case 5:
        StringUtils::stringToCharArray(campoCurr, record->atualizacao, ARTIGO_ATUALIZACAO_MAX_SIZE);
        break;

      case 6:
        StringUtils::stringToCharArray(campoCurr, record->snippet, ARTIGO_SNIPPET_MAX_SIZE);

      default:;
      }

      qtdCamposLidos++;
  }

  if (!algumRegistroValido) {
    delete[] record;
    record = NULL;
  }

  return record;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////// CLASSE "CSVReader" //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<const char delimiter>
std::string getFieldFrom(std::ifstream& stream, bool& validField){ // XXX consegue ler apenas os 100 primeiros
  std::string field;
  char lastChar, currChar;
  unsigned quotesAmount = 0;
  bool end = false;

  auto endOfField = [&](){
    if (   !quotesAmount // é um campo sem as aspas
        || (lastChar == '"' && !(quotesAmount&1)) // OU o último caractere lido foi aspas e a quantidade de aspas é par
       ) end = true;
  };

  do{
    if (!stream.get(currChar)) return "";

    switch (currChar) {
      case '"':
        quotesAmount++;
        break;

      case '\r':
        stream.get(); // tratar EOL com CRLF
        break;

      case '\n':
        endOfField();
        goto _end_of_loop;

      case delimiter:
        if (field == "NULL") return field;
        endOfField();

      default:;
    }

    field += currChar;
    _end_of_loop:
    lastChar = currChar;
  }while(!end);

  validField = (field.length() > 0); // caso tenha sido apenas uma linha em branco
  return StringUtils::removeLastCharFrom(field, delimiter);
}



#endif