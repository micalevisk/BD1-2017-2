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
//  $ g++ -std=c++11 -I../../include upload.cpp -o ../../upload
//  E execute:
//  (no diretório raiz do trabalho)
//  upload <path/to/data.csv>
//


#include "externalHash.hpp"
#include "log.hpp"
#include <vector>
#include <iostream>

#define showErrorAndExit(msg) (Log::errorMessageExit(__FILE__, __LINE__, msg))


using namespace std;


int main(const int argc, const char* argv[]){
  if (argc != 2) return 1;// TODO mostrar help (como usar este programa)

  const char* PATH_ARQUIVO_COM_DADOS = argv[1];
  ifstream arqComDados; // arquivo que contém a hash e arquivo de entrada, respectivamente
  ExternalHash<Artigo, getArtigoId_pfn> hashExterna(PATH_HASH_FILE, getArtigoId);

  arqComDados.open(PATH_ARQUIVO_COM_DADOS);
  if (!arqComDados.is_open()) showErrorAndExit("ao abrir o arquivo com os dados");

  // ======================== criar arquivo de dados ======================== //
  hashExterna.create();

  // ============ ler CSV e inserir registros no arquivo de dados =========== //
  #ifdef DEBUG
    unsigned long qtdRegistrosLidos = 0;
  #endif

  while (!arqComDados.eof()) {
    Artigo* artigo = getRecordArtigoFrom(arqComDados);
    if (!artigo) continue;
    if (!hashExterna.insertRecordOnHashFile(*artigo)) showErrorAndExit("ao inserir registro lido na hash externa");

    #ifdef DEBUG
      qtdRegistrosLidos++;
    #endif
  }

  // ==================== fim da manipulação dos arquivos ==================== //
  hashExterna.closeStream();
  arqComDados.close();

  #ifdef TEST
    hashExterna.deleteHashfile();
  #endif

  #ifdef DEBUG
    fprintf(stdout, "- upload[%u] qtdRegistrosLidos = %lu\n", __LINE__, qtdRegistrosLidos);
  #endif


  return 0;
}
