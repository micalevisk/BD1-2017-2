//
//  [upload.cpp]
//  Created by Micael Levi on 2017-08-26
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
//
//  A priori, recebe o caminho para o arquivo CSV
//  para criar um arquivo binário,
//  o arquivo de dados, usando hashing estática.
//  Dessa forma ele fará a carga inicial da massa de dados de testes.
//


// ---------------------------------------------------------------------------------------- //

#define PATH_HASH_FILE "__hashfile"

// parâmetros para a hash externa
#define QTD_BUCKETS 1549147 // M
#define QTD_BLOCOS_POR_BUCKET 1 // m
#define BUCKET_SIZE (QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE) // m * B
#define FATOR_BLOCO (BLOCO_SIZE / ARTIGO_SIZE) // bfr = quantidade máxima de registros por bloco

// definição dos tamanhos
#define ARTIGO_SIZE (sizeof(Artigo)) // R
#define BLOCO_SIZE (sizeof(Bloco)) // B


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



// tipo dos registros que serão lidos
struct Artigo {//2384 bytes
  int id;//4
  int ano;//4
  int citacoes;//4

  char atualizacao[ARTIGO_ATUALIZACAO_MAX_SIZE+1];//19 (AAAA-MM-DD hh:mm:ss)
  char titulo[ARTIGO_TITULO_MAX_SIZE+1];//alfa 300
  char autores[ARTIGO_ATORES_MAX_SIZE+1];//alfa 1024
  char snippet[ARTIGO_SNIPPET_MAX_SIZE+1];//alfa 1024
};

// estrutura do bloco que será lido (de uma vez) do disco, armazena até bfr registros
struct Bloco {//4096 bytes
  unsigned qtdRegistros;//(entre 0 e FATOR_BLOCO - 1) 8 bytes
  char dados[4092];//para completar os 4KB
};

// ---------------------------------------------------------------------------------------- //


#include "../../include/stringUtils.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/stat.h>


// ---------------------------------------------------------------------------------------- //
template<const char delimiter = ';'> std::string getFieldFrom(std::ifstream& stream, bool& validField);
auto getRecordFrom(std::ifstream& inputStream) -> Artigo*;
// ---------------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------------- //
unsigned bucketPara(const Artigo& artigo){
  // aplicar função hash
  return artigo.id % QTD_BUCKETS;
}
// ---------------------------------------------------------------------------------------- //


using namespace std;

int main(const int argc, const char *argv[])
{
  if (argc != 2) return 1;// TODO mostrar help (como usar este programa)

  const char* PATH_ARQUIVO_COM_DADOS = argv[1];
  fstream arqDados;
  ifstream arqComDados; // arquivo que contém a hash e arquivo de entrada, respectivamente

  /*
  cout << "tamanho do Artigo = " << ARTIGO_SIZE << endl;
  cout << "bfr = " << FATOR_BLOCO << endl;
  cout << "tamanho do arquivo = " << BUCKET_SIZE * QTD_BUCKETS << endl;
  */


  /* ===================== alocação do arquivo de dados ===================== */
  arqDados.open(PATH_HASH_FILE, fstream::in | fstream::out | fstream::trunc | ios::binary);
  if (!arqDados.is_open()) exit(EXIT_FAILURE);// TODO tornar verboso

  Bloco bufferBloco = { 0 }; // buffer pra Bloco (ou página, se estiver na MP) com 0 registros
  // escrevendo os buckets no arquivo
  for(unsigned i=0; i < QTD_BUCKETS; ++i){
    for(unsigned numBloco=0; numBloco < QTD_BLOCOS_POR_BUCKET; ++numBloco){
      // escrever 1 bloco, i.e., alocar espaço para os registros
      arqDados.write((char*)&bufferBloco, BLOCO_SIZE);
    }
  }

  #ifdef DEBUG
  struct stat results;
  if ( !stat(PATH_HASH_FILE, &results) ) {
   fprintf(stderr, "Análise do arquivo:\n");
   fprintf(stderr, "\tThe file size in GB: %f\n", results.st_size / 1000000000.0);
   fprintf(stderr, "\tNumber of blocks allocated: %ld\n", results.st_blocks);
   fprintf(stderr, "\tBlock size in bytes: %ld\n" , results.st_blksize);
  }
  #endif

  /* ============ ler CSV e inserir registros no arquivo de dados ============ */
  arqComDados.open(PATH_ARQUIVO_COM_DADOS);
  if (!arqComDados.is_open()) exit(EXIT_FAILURE);// TODO tornar verboso

  unsigned artigos = 0;
  while (!arqComDados.eof()) {
    artigos++;
    Artigo* artigo = getRecordFrom(arqComDados);
    cout << bucketPara(*artigo) << endl;

  }

  cout << "LIDOS = " << artigos << endl;


  arqDados.close();
  arqComDados.close();

  #ifdef DEBUG
  remove(PATH_HASH_FILE);
  #endif
}





///////////////////////////////////////////////////////////////////////////////


template<const char delimiter = ';'>
auto getFieldFrom(std::ifstream& stream, bool& validField) -> std::string { // XXX consegue lê apenas os 100 primeiros
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


auto getRecordFrom(std::ifstream& inputStream) -> Artigo* { // TODO neutralizar retorno
  Artigo* record = new Artigo;
  bool algumRegistroValido = false;

  for (unsigned qtdCamposLidos=0; qtdCamposLidos < 7; ) {
    bool campoValido = false;
    string campoCurr = getFieldFrom(inputStream, campoValido);

    if (campoValido) {
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
    } else if (inputStream.eof()) break;
  }

  return algumRegistroValido ? record : NULL;
}
