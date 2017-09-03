/*
 *  [findrec.cpp]
 *  Created by Micael Levi and Victor Meireles on 2017-09-02
 *  Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
 *
 *  Utiliza o arquivo de dados binário
 *  criado pelo programa 'upload' para procurar em tempo ótimo
 *  um registro do tipo Artigo.
 *  Além de exibir informações sobre a quantidade de blocos lidos e a total.
 *
 *  Macros disponíveis:
 *  DEBUG - mostra informações (na saída de erro) para a depuração.
 *  TEST  - utilizar versão reduzida dos registros.
 *
 *  Para usar este programa, compile:
 *  $ g++ -std=c++11 -I../../include findrec.cpp -o ../../findrec
 *  E execute:
 *  (no diretório raiz do trabalho)
 *  findrec <record_id> [path/to/hashfile]
 */


#include "parametros.h"
#include "artigo.hpp"
#include "externalHash.hpp"
#include <iostream>



using namespace std;


int main(const int argc, const char* argv[]){
  if (argc < 2) Log::errorMessageExit("use:", argv[0], "<record_id> [path/to/hashfile]");
  const char* PATH_ARQUIVO_DE_DADOS = (argc == 3) ? argv[2] : PATH_HASH_FILE;
  int idProcurado;
  unsigned long qtdBlocosCarregados = 0;
  ifstream arqComDados; // arquivo que contém a hash e arquivo de entrada, respectivamente
  Artigo artigo; // armazenará o registro (se encontrado)
  bool registroEncontrado = false;
  ExternalHash<Artigo, getArtigoId_pfn> hashExterna(PATH_ARQUIVO_DE_DADOS, getArtigoId);

  try {
    idProcurado = StringUtils::stringToInt(argv[1]);
  } catch (std::exception const& e) {
    Log::errorMessageExit("id", argv[1], "invalido");
  }

  #ifdef DEBUG
    fprintf(stdout, "- findrec[%u] iniciando busca por %d no arquivo de dados...\n", __LINE__, idProcurado);
  #endif

  artigo.id = idProcurado;
  qtdBlocosCarregados = hashExterna.findRecord(artigo);

  Log::basicMessage("Quantidade de blocos lidos =", qtdBlocosCarregados);
  Log::basicMessage("Quantidade total de blocos no arquivo de dados =", QTD_BLOCOS_POR_BUCKET * QTD_BUCKETS);
  if (qtdBlocosCarregados > 0) cout << artigo << endl;



  return 0;
}
