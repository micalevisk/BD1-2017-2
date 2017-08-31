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
  // #define SHOW_VARIABLE(x) (std::cerr << #x " = " << (x) << '\n')
#endif


using namespace std;


int main(const int argc, const char* argv[]){
  if (argc != 2) return 1;// TODO mostrar help (como usar este programa)

  const char* PATH_ARQUIVO_COM_DADOS = argv[1];
  ifstream arqComDados; // arquivo que contém a hash e arquivo de entrada, respectivamente

  ExternalHash<Artigo, getArtigo_pfn> hashExterna(PATH_HASH_FILE, getArtigoId);
  hashExterna.create();


  // ============ ler CSV e inserir registros no arquivo de dados ============ //
  #ifdef DEBUG
    unsigned long qtdRegistrosLidos = 0;
  #endif

  arqComDados.open(PATH_ARQUIVO_COM_DADOS);
  if (!arqComDados.is_open()) {
    fprintf(stderr, "{ERROR at %s, code line %d}\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);// TODO tornar verboso
  }

  while (!arqComDados.eof()) {
    Artigo* artigo = getRecordArtigoFrom(arqComDados);
    if (!artigo) continue;

    try { // tentar inserir registro lido no arquivo de dados
      if ( !hashExterna.insertRecordOnHashFile(*artigo) )
        throw "ERROR ao inserir na hash";
    } catch (const char* msg) {
      fprintf(stderr, "{%s: at %s, code line %d}\n", msg, __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }

    #ifdef DEBUG
      qtdRegistrosLidos++;
    #endif
  }


  hashExterna.closeStream();
  arqComDados.close();


  #ifdef TEST
    hashExterna.deleteHashfile();
  #endif


  #ifdef DEBUG
    fprintf(stderr, "- upload:: qtdRegistrosLidos = %lu\n", qtdRegistrosLidos);
  #endif


  return 0;
}