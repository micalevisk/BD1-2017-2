//
//  [upload.cpp]
//  Created by Micael Levi on 2017-08-26
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
//
//  Recebe o caminho para o arquivo CSV
//  para criar três arquivos binários
//  o arquivo de dados, usando hashing estática;
//  o arquivo de índice primário, usando B-Tree indexada pelo ID;
//  o arquivo de índice secundário, usando B-Tree indexada pelo TITULO;
//  Dessa forma ele fará a carga inicial da massa de dados de testes.
//
//  Para usar este programa, compile:
//  $ g++ -std=c++11 upload.cpp -o upload
//  E execute:
//  upload <path/to/data.csv>
//


#define SHOW_VARIABLE(x) (std::cerr << #x " = " << (x) << '\n')

// ---------------------------------------------------------------------------------------- //

#define DIR_ARQUIVOS_CRIADOS "../../arquivos_criados/"

#define PATH_HASH_FILE DIR_ARQUIVOS_CRIADOS"__hashfile"

// parâmetros para a hash externa
#define QTD_BUCKETS 1549147 // M
#define QTD_BLOCOS_POR_BUCKET 1 // m
#define BUCKET_SIZE (QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE) // m * B
#define FATOR_BLOCO (BLOCO_SIZE / ARTIGO_SIZE) // bfr = quantidade máxima de registros por bloco

// definição dos tamanhos
#define ARTIGO_SIZE (sizeof(Artigo)) // R
#define BLOCO_SIZE (sizeof(Bloco)) // B
#define QTD_CAMPOS_ARTIGO 7


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
template<const char delimiter> std::string getFieldFrom(std::ifstream&, bool&);
template<typename F> bool insertRecordOnHashFile(std::fstream&, const Artigo&, F);
// ---------------------------------------------------------------------------------------- //

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
// ---------------------------------------------------------------------------------------- //


using namespace std;



// O campo de hash escolhido deve ser retornado pela função abaixo.
int getArtigoId(const Artigo& artigo){
  return artigo.id;
}

int main(const int argc, const char* argv[])
{
  if (argc != 2) return 1;// TODO mostrar help (como usar este programa)

  const char* PATH_ARQUIVO_COM_DADOS = argv[1];
  fstream arqDados;
  ifstream arqComDados; // arquivo que contém a hash e arquivo de entrada, respectivamente


  /* ===================== alocação do arquivo de dados ===================== */
  arqDados.open(PATH_HASH_FILE, fstream::in | fstream::out | fstream::trunc | ios::binary);
  if (!arqDados.is_open()) {
    fprintf(stderr, "{ERROR at %s, code line %d}\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);// TODO tornar verboso
  }

  Bloco bufferPage = { 0 }; // buffer pra Bloco (ou página, se estiver na MP) com 0 registros
  // escrevendo os buckets no arquivo
  for (unsigned i=0; i < QTD_BUCKETS; ++i) {
    // quando estão na memoria principal, blocos são chamados de "páginas"
    for (unsigned numPagina=0; numPagina < QTD_BLOCOS_POR_BUCKET; ++numPagina) {
      // escrever 1 bloco, i.e., alocar espaço para os registros
      arqDados.write((char*)&bufferPage, BLOCO_SIZE);
    }
  }

  #ifdef DEBUG
  unsigned long qtdRegistrosLidos = 0;

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
  if (!arqComDados.is_open()) {
    fprintf(stderr, "{ERROR at %s, code line %d}\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);// TODO tornar verboso
  }

  while (!arqComDados.eof()) {
    Artigo* artigo = getRecordArtigoFrom(arqComDados);
    if (!artigo) continue;

    try { // tentar inserir registro lido no arquivo de dados
      if ( !insertRecordOnHashFile(arqDados, *artigo, getArtigoId) )
        throw "ERROR ao inserir na hash";
    } catch (const char* msg) {
      fprintf(stderr, "{%s: at %s, code line %d}\n", msg, __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }

    #ifdef DEBUG
    qtdRegistrosLidos++;
    #endif
  }


  #ifdef DEBUG
  SHOW_VARIABLE(qtdRegistrosLidos);
  #endif


  arqDados.close();
  arqComDados.close();


  #ifdef TEST
  remove(PATH_HASH_FILE);
  #endif
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



////////////////////////////////////////////////////////////////////////////////
///////////////////////// CLASSE "ExternalHash" ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template<typename F, typename T>
unsigned long hashOf(T data, F getKeyFrom){
  // aplicar função hash
  return getKeyFrom(data) % QTD_BUCKETS;
}


template<typename F>
bool insertRecordOnHashFile(std::fstream& hashFile, const Artigo& artigo, F getKey){

  // calcular o bucket que será inserido
  unsigned long bucketNumber = hashOf(artigo, getKey);

  // mover cursor do arquivo de dados para o bucket correspondente, i.e., byte: bucketNumber * (m * B)
  hashFile.seekg(bucketNumber * QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE, std::ios::beg);

  // carregar bloco(s) do disco enquanto não houver espaço para inserir, i.e., encontrar bloco com espaço
  Bloco bufferPage = { 0 }; // quando o bloco está na memória, ele é chamado de "página"
  unsigned currPage;

  for (currPage = 0; currPage < QTD_BLOCOS_POR_BUCKET; ++currPage) {
    // ler bloco encontrado ao mover cursor
    hashFile.read((char*)&bufferPage, BLOCO_SIZE);

    // bloco válido encontrado;
    // os próximos sempre estarão livres (se existirem) mas o primeiro livre encontrado é o escolhido
    // ocasionando um crescimento top-down do bucket
    if (bufferPage.qtdRegistros < FATOR_BLOCO) break; // para quebrar o loop e evitar o incremento de currPage
  }

  // currPage sempre será menor que QTD_BLOCOS_POR_BUCKET se algum bloco válido foi encontrado
  if (currPage >= QTD_BLOCOS_POR_BUCKET) return false;

  // atualizar o bloco encontrado, i.e., inserir o novo registro (ainda em memória principal)
  bufferPage.qtdRegistros++;
  std::memcpy(&bufferPage.dados[currPage * ARTIGO_SIZE], (char*)&artigo, ARTIGO_SIZE);

  // reescrevendo no arquivo de dados o bloco encontrado que foi atualizado
  hashFile.seekp(- ((currPage+1) * BLOCO_SIZE), std::ios::cur); // voltar para o primeiro byte do bloco encontrado
  hashFile.write((char*)&bufferPage, BLOCO_SIZE);


  return true;
}
