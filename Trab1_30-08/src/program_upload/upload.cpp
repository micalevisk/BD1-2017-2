//
//  [upload.cpp]
//  Created by Micael Levi on 2017-08-26
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
//
//  Recebe o caminho para o arquivo CSV
//  para criar três arquivos binários
//  o arquivo de dados, usando hashing estática;
//  o arquivo de índice primário, usando B-Tree indexada pelo ID do artigo;
//  o arquivo de índice secundário, usando B-Tree indexada pelo TITULO do artigo;
//  Dessa forma ele fará a carga inicial da massa de dados de testes.
//
//  Macros disponíveis:
//  DEBUG - mostra informações (na saída de erro) para a depuração.
//  TEST  - no final da execução, os arquivos criados serão apagados.
//
//  Para usar este programa, compile:
//  $ g++ -std=c++11 -I../../include upload.cpp -o upload
//  E execute:
//  upload <path/to/data.csv>
//


#include "stringUtils.hpp"
#include "externalHash.hpp"
#include <vector>
#include <iostream>

#ifdef DEBUG
  #include <sys/stat.h>
  #define SHOW_VARIABLE(x) (std::cerr << #x " = " << (x) << '\n')
#endif


using namespace std;


int main(const int argc, const char* argv[]){
  if (argc != 2) return 1;// TODO mostrar help (como usar este programa)

  const char* PATH_ARQUIVO_COM_DADOS = argv[1];
  fstream arqDados;
  ifstream arqComDados; // arquivo que contém a hash e arquivo de entrada, respectivamente


  // ===================== alocação do arquivo de dados ===================== //
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


  // ============ ler CSV e inserir registros no arquivo de dados ============ //
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


