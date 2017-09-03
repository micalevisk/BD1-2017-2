/*
 *  [upload.cpp]
 *  Created by Micael Levi and Victor Meireles on 2017-08-26
 *  Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
 *
 *  Recebe o caminho para o arquivo CSV
 *  para criar três arquivos binários
 *  o arquivo de dados, usando hashing estática;
 *  o arquivo de índice primário, organizado por B-Tree indexada pelo ID do artigo;
 *  o arquivo de índice secundário, organizado por B-Tree indexada pelo TITULO do artigo;
 *  Dessa forma ele fará a carga inicial da massa de dados de testes.
 *
 *  Macros disponíveis:
 *  DEBUG - mostra informações (na saída de erro) para a depuração.
 *  TEST  - no final da execução, os arquivos criados serão apagados.
 *
 *  Para usar este programa, compile:
 *  $ g++ -std=c++11 -I../../include upload.cpp -o ../../upload
 *  E execute:
 *  (no diretório raiz do trabalho)
 *  upload <path/to/data.csv>
 */


#include "artigo.hpp"
#include "externalHash.hpp"
#include <vector>
#include <iostream>


using namespace std;


int main(const int argc, const char* argv[]){
  if (argc != 2) Log::errorMessageExit("use:", argv[0], "<path/to/data.csv>");

  const char* PATH_ARQUIVO_COM_DADOS = argv[1];
  ifstream arqComDados; // arquivo de entrada
  ExternalHash<Artigo, getArtigoId_pfn> hashExterna(PATH_HASH_FILE, getArtigoId);

  arqComDados.open(PATH_ARQUIVO_COM_DADOS);
  if (!arqComDados.is_open()) Log::errorMessageExit("ao abrir o arquivo:", PATH_ARQUIVO_COM_DADOS);

  // ======================== criar arquivo de dados ======================== //
  Log::basicMessage("iniciando criacao do arquivo de dados...");
  hashExterna.create();

  // ============ ler CSV e inserir registros no arquivo de dados =========== //
  #ifdef DEBUG
    unsigned long qtdRegistrosLidos = 0;
  #endif

  Log::basicMessage("\tiniciando leitura do arquivo texto", PATH_ARQUIVO_COM_DADOS);
  while (!arqComDados.eof()) {
    Artigo* artigo = getRecordArtigoFrom(arqComDados);
    if (!artigo) continue;
    if (!hashExterna.insertRecordOnHashFile(*artigo)) Log::errorMessageExit("ao inserir registro lido na hash externa");

    #ifdef DEBUG
      qtdRegistrosLidos++;
    #endif
  }
  Log::basicMessage("\tfim da indexacao dos registros lidos. hash externa finalizada");

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
