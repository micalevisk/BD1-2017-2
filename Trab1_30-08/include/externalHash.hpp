//
//  [externalHash.hpp]
//  Created by Micael Levi and Victor Meireles on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//
//	Funções e definições específicas para o uso da hash
//  que indexará o arquivo de dados.
//


#ifndef EXTERNAL_HASH_HPP
#define EXTERNAL_HASH_HPP



#include "artigo.h"
#include <iostream>
#include <fstream>

#ifdef DEBUG
  #include <sys/stat.h>
#endif


// ============= PARÂMETROS PARA A HASH EXTERNA ============= //
#define Bloco struct __Bloco
#define BLOCO_SIZE (sizeof(Bloco)) // B

#ifndef QTD_BUCKETS
  #define QTD_BUCKETS 1549147 // M
#endif
#define QTD_BLOCOS_POR_BUCKET 1 // m
#define BUCKET_SIZE (QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE) // m * B
// #define FATOR_BLOCO (BLOCO_SIZE / ARTIGO_SIZE) // bfr = quantidade máxima de registros por bloco


/**
 * Descreve como será tratado um bloco
 * lido (ou escrito) do disco para a memória principal.
 * Guarda a quantidade de registros que possui (até bfr registros),
 * seguidos dos mesmos.
 */
struct __Bloco {// 4096 bytes
  unsigned qtdRegistros;// (entre 0 e FATOR_BLOCO - 1) 8 bytes
  char dados[4092];// para completar os 4KB
};



/**
 *
 */
template<typename TypeRecord, typename F>
class ExternalHash {

private:
  const size_t RECORD_SIZE = sizeof(TypeRecord); // tamanho de um registro
  const unsigned long BLOCKING_FACTOR = (BLOCO_SIZE / RECORD_SIZE); // fator de bloco
  const char* pathToHashFile_; // caminho para o arquivo de dados que será criado
  F getKey_; // função que retorna a chave de um registro para ser usado na função de espalhamento

public:
  std::fstream streamHashFile;


  /**
   * Construtor utilizado para inicializar
   * os parâmetros da hash estática.
   *
   * @param pathToHashFile
   * @param getKey
   *
   * @author Micael Levi
   * @date 2017-08-30
   */
  ExternalHash(const char* pathToHashFile, const F& getKey) : pathToHashFile_(pathToHashFile), getKey_(getKey) {
    #ifdef DEBUG
      fprintf(stderr, "- externalHash[%d] Hash externa inicializada com os parâmetros\n", __LINE__);
      fprintf(stderr, "\tRECORD_SIZE: %ld\n", RECORD_SIZE);
      fprintf(stderr, "\tBLOCKING_FACTOR: %lu\n", BLOCKING_FACTOR);
      fprintf(stderr, "\tpathToHashFile_: '%s'\n", pathToHashFile_);
    #endif
  }

  /**
   * Cria o arquivo de dados organizado em hash estático.
   * Deixa o arquivo aberto para leitura ou escrita.
   *
   * @author Micael Levi
   * @date 2017-08-30
   */
  void create(){
    // ===================== alocação do arquivo de dados ===================== //
    streamHashFile.open(pathToHashFile_, std::fstream::in | std::fstream::out | std::fstream::trunc | std::ios::binary);
    if (!streamHashFile.is_open()) {
      fprintf(stderr, "{ERROR at %s, code line %d}\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);// TODO tornar verboso
    }

    Bloco bufferPage = { 0 }; // buffer pra Bloco (ou página, se estiver na MP) com 0 registros
    // escrevendo os buckets no arquivo
    for (unsigned long i=0; i < QTD_BUCKETS; ++i) {
      // quando estão na memoria principal, blocos são chamados de "páginas"
      for (unsigned long numPagina=0; numPagina < QTD_BLOCOS_POR_BUCKET; ++numPagina) {
        // escrever 1 bloco, i.e., alocar espaço para os registros
        streamHashFile.write((char*)&bufferPage, BLOCO_SIZE);
      }
    }

    #ifdef DEBUG
      struct stat results;
      if ( !stat(pathToHashFile_, &results) ) {
       fprintf(stderr, "- externalHash[%d] Análise do arquivo criado\n", __LINE__);
       fprintf(stderr, "\tThe file size in GB: %f\n", results.st_size / 1000000000.0);
       fprintf(stderr, "\tNumber of blocks allocated: %ld\n", results.st_blocks);
       fprintf(stderr, "\tBlock size in bytes: %ld\n" , results.st_blksize);
      }
    #endif
  }

  /**
   * Fechar a stream do arquivo de dados
   * que foi criado (e aberto) pelo método #create,
   * se ele estiver aberto.
   *
   * @author Micael Levi
   * @date 2017-08-30
   */    
  void closeStream(){
    streamHashFile.close();
  }

  /**
   * Fecha a stream do arquivo de dados e o apaga.
   *
   * @see #closeStream
   * @author Micael Levi
   * @date 2017-08-30
   */    
  void deleteHashfile(){
    closeStream();
    remove(pathToHashFile_);
    #ifdef DEBUG
      fprintf(stderr, "- externalHash[%d] Arquivo '%s' foi removido!\n", __LINE__, pathToHashFile_);
    #endif
  }

  /**
   * Aplica a função de hash escolhida
   * que mapeia um registro para o seu bucket na hash externa.
   * A função escolhida foi
   * h(k) = k MOD M
   * Onde 'k' é a chave e 'M' é a quantidade de buckets na hash.
   * A implementação abaixo está totalmente neutra.
   *
   * @param data O dado sobre o qual a hash será aplicada.
   * @param getKey Função que retorna (número) a chave que será usada pela função hash.
   * @return O número do bucket que o dado deve estar.
   *
   * @author Micael Levi
   * @date 2017-08-30
   */
  unsigned long applyHashOn(TypeRecord data){
    return getKey_(data) % QTD_BUCKETS;
  }

  /**
   * Insere um registro (que será passado) no arquivo de dados.
   *
   * @param record O registro que será inserido.
   * @return True se o registro foi inserido. False se não há espaço no bucket.
   *
   * @author Micael Levi
   * @date 2017-08-30
   */
  bool insertRecordOnHashFile(const TypeRecord& record){

    // calcular o bucket que será inserido
    unsigned long bucketNumber = applyHashOn(record);

    // mover cursor do arquivo de dados para o bucket correspondente, i.e., byte: bucketNumber * (m * B)
    streamHashFile.seekg(bucketNumber * QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE, std::ios::beg);

    // carregar bloco(s) do disco enquanto não houver espaço para inserir, i.e., encontrar bloco com espaço
    Bloco bufferPage = { 0 }; // quando o bloco está na memória, ele é chamado de "página"
    unsigned currPage;

    for (currPage = 0; currPage < QTD_BLOCOS_POR_BUCKET; ++currPage) {
      // ler bloco encontrado ao mover cursor
      streamHashFile.read((char*)&bufferPage, BLOCO_SIZE);

      // bloco válido encontrado;
      // os próximos sempre estarão livres (se existirem) mas o primeiro livre encontrado é o escolhido
      // ocasionando um crescimento top-down do bucket
      if (bufferPage.qtdRegistros < BLOCKING_FACTOR) break; // para quebrar o loop e evitar o incremento de currPage
    }

    // currPage sempre será menor que QTD_BLOCOS_POR_BUCKET se algum bloco válido foi encontrado
    if (currPage >= QTD_BLOCOS_POR_BUCKET) return false;

    // atualizar o bloco encontrado, i.e., inserir o novo registro (ainda em memória principal)
    bufferPage.qtdRegistros++;
    std::memcpy(&bufferPage.dados[currPage * RECORD_SIZE], (char*)&record, RECORD_SIZE);

    // reescrevendo no arquivo de dados o bloco encontrado que foi atualizado
    streamHashFile.seekp(- BLOCO_SIZE, std::ios::cur); // voltar para o primeiro byte do bloco encontrado
    streamHashFile.write((char*)&bufferPage, BLOCO_SIZE); // escrever a página no arquivo

    return true;
  }

};



#endif