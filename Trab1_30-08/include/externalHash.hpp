/*
 * Copyright (c) 2017 All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Created by Micael Levi and Victor Meireles on 2017-08-11
 *
 * Funções e definições específicas para o uso da hash
 * que indexará o arquivo de dados.
 */


#ifndef EXTERNAL_HASH_HPP
#define EXTERNAL_HASH_HPP



#include "log.hpp"
#include <iostream>
#include <fstream>

#ifdef DEBUG
  #include <sys/stat.h>
#endif



// ============= PARÂMETROS PARA A HASH EXTERNA ============= //
#define Bloco struct __Bloco
#define BLOCO_SIZE (sizeof(Bloco)) // B

#ifdef TEST
  #define QTD_BUCKETS 128000 // causará 11 colisões, i.e., 1 bloco precisa ter espaço pra 12 registros
#else
  #define QTD_BUCKETS 1549147 // M
#endif
#define QTD_BLOCOS_POR_BUCKET 1 // m
#define BUCKET_SIZE (QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE) // m * B


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
  const unsigned long BLOCKING_FACTOR = (BLOCO_SIZE / RECORD_SIZE); // fator de bloco bfr = quantidade máxima de registros por bloco
  const char* pathToHashFile_; // caminho para o arquivo de dados que será criado
  F getKey_; // função que retorna a chave de um registro para ser usado na função de espalhamento
  unsigned long totalBlocks_; // contabiliza a quantidade de blocos que foram alocados

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
  ExternalHash(const char* pathToHashFile, const F& getKey) : pathToHashFile_(pathToHashFile), getKey_(getKey), totalBlocks_(0) {
    #ifdef DEBUG
      fprintf(stdout, "- externalHash[%u] Hash externa inicializada com os parâmetros\n", __LINE__);
      fprintf(stdout, "\tQTD_BUCKETS = %ld\n", QTD_BUCKETS);
      fprintf(stdout, "\tQTD_BLOCOS_POR_BUCKET = %ld\n", QTD_BLOCOS_POR_BUCKET);
      fprintf(stdout, "\tRECORD_SIZE = %ld\n", RECORD_SIZE);
      fprintf(stdout, "\tBLOCKING_FACTOR = %lu\n", BLOCKING_FACTOR);
      fprintf(stdout, "\tpathToHashFile_ = '%s'\n", pathToHashFile_);
    #endif
  }

  /**
   * Cria o arquivo de dados organizado em hash estático.
   * Deixa o arquivo aberto para leitura ou escrita.
   *
   * @author Micael Levi
   * @date 2017-08-30
   */
  void create(void){
    // ===================== alocação do arquivo de dados ===================== //
    streamHashFile.open(pathToHashFile_, std::fstream::in | std::fstream::out | std::fstream::trunc | std::ios::binary);
    if (!streamHashFile.is_open()) Log::errorMessageExit("ao criar o arquivo de dados com nome", pathToHashFile_);
    Log::basicMessage("\talocando os", QTD_BUCKETS, "buckets na hash externa ...");

    Bloco bufferPage = { 0 }; // buffer pra Bloco (ou página, se estiver na MP) com 0 registros
    // escrevendo os buckets no arquivo
    for (unsigned long i=0; i < QTD_BUCKETS; ++i) {
      // quando estão na memoria principal, blocos são chamados de "páginas"
      for (unsigned long numPagina=0; numPagina < QTD_BLOCOS_POR_BUCKET; ++numPagina) {
        // escrever 1 bloco, i.e., alocar espaço para os registros
        streamHashFile.write((char*)&bufferPage, BLOCO_SIZE);
        // contabilizando o novo bloco
        ++totalBlocks_;
      }
    }

    if (totalBlocks_ != (QTD_BUCKETS * QTD_BLOCOS_POR_BUCKET)) Log::errorMessageExit((QTD_BUCKETS * QTD_BLOCOS_POR_BUCKET) - totalBlocks_, " blocos nao alocados");
    Log::basicMessage("\tarquivo de dados em hash criado em:", pathToHashFile_);

    #ifdef DEBUG
      struct stat results;
      if ( !stat(pathToHashFile_, &results) ) {
       fprintf(stdout, "- externalHash[%d] Análise do arquivo criado\n", __LINE__);
       fprintf(stdout, "\tThe file size in GB: %f\n", results.st_size / 1000000000.0);
       fprintf(stdout, "\tNumber of blocks allocated: %ld\n", results.st_blocks);
       fprintf(stdout, "\tBlock size in bytes: %ld\n" , results.st_blksize);
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
  void closeStream(void){
    streamHashFile.close();
  }

  /**
   * Fecha a stream do arquivo de dados e o apaga.
   *
   * @see #closeStream
   * @author Micael Levi
   * @date 2017-08-30
   */
  void deleteHashfile(void){
    closeStream();
    remove(pathToHashFile_);
    #ifdef DEBUG
      fprintf(stdout, "- externalHash[%d] Arquivo '%s' foi removido!\n", __LINE__, pathToHashFile_);
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
    if (!streamHashFile.is_open()) return false;

    // identifcar o bucket ao qual o registro será inserido
    unsigned long bucketNumber = applyHashOn(record);

    // mover cursor do arquivo de dados para o bucket correspondente, i.e., byte: bucketNumber * (m * B)
    streamHashFile.seekg(bucketNumber * QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE, std::ios::beg);

    // carregar bloco(s) do disco enquanto não houver espaço para inserir, i.e., encontrar bloco com espaço
    Bloco bufferPage = { 0 }; // quando o bloco está na memória, ele é chamado de "página"
    unsigned long currPage;

    // iterando sobre os blocos do bucket encontrado
    for (currPage = 0; currPage < QTD_BLOCOS_POR_BUCKET; ++currPage) {
      // ler bloco no byte corrente
      streamHashFile.read((char*)&bufferPage, BLOCO_SIZE);

      // bloco válido encontrado;
      // os próximos sempre estarão livres (se existirem) mas o primeiro livre encontrado é o escolhido
      // ocasionando um crescimento top-down no bucket
      if (bufferPage.qtdRegistros < BLOCKING_FACTOR) break; // para quebrar o loop e evitar o incremento de currPage
    }

    // currPage sempre será menor que QTD_BLOCOS_POR_BUCKET se algum bloco válido foi encontrado
    if (currPage >= QTD_BLOCOS_POR_BUCKET) return false;

    // recupera a posição que o registro estará no bloco e atualiza o número de registro no bloco
    unsigned long indexRecord = bufferPage.qtdRegistros++; // pós-incremento pois os índices começam em 0
    // std::memcpy(&bufferPage.dados[indexRecord * RECORD_SIZE], (char*)&record, RECORD_SIZE);
    TypeRecord* records = (TypeRecord*) bufferPage.dados; // trata os dados como um arranjo de registros
    records[indexRecord] = record;

    #ifdef DEBUG
      fprintf(stdout, "- externalHash[%u] registro inserido no bloco %lu (bucket %lu) na posição %lu\n", __LINE__, currPage, bucketNumber, indexRecord);
    #endif

    // reescrevendo no arquivo de dados o bloco encontrado que foi atualizado
    streamHashFile.seekp(- BLOCO_SIZE, std::ios::cur); // voltar para o primeiro byte do bloco encontrado
    streamHashFile.write((char*)&bufferPage, BLOCO_SIZE); // escrever a página no arquivo

    return true;
  }

  /**
   * Abre o arquivo de dados para leitura
   * e procura um registro de acordo com o valor do campo de hashing.
   *
   * @param record O registro que será buscado, i.e., a chave de pesquisa deve ser obtida por ele.
   * @param recordFind Guardará true se o registro foi encontrado, false caso contrário.
   * @return A quantidade de blocos lidos para encontrar (ou não) tal registro.
   *
   * @author Micael Levi
   * @date 2017-08-30
   */
  unsigned long findRecord(TypeRecord& record, bool& recordFind){
    recordFind = false;
    // ===================== leitura do arquivo de dados ===================== //
    std::ifstream streamHashFileInput(pathToHashFile_, std::ios::binary);
    if (!streamHashFileInput.is_open()) Log::errorMessageExit("ao abrir o arquivo de dados de nome", pathToHashFile_);
    #ifdef DEBUG
      fprintf(stdout, "- externalHash[%u] lendo o arquivo de dados...\n", __LINE__);
    #endif

    // identificar o bucket ao qual o registro buscado pertence
    unsigned long bucketNumber = applyHashOn(record);
    auto findKey = getKey_(record);
    #ifdef DEBUG
      fprintf(stdout, "- externalHash[%u] buscando pela chave %ld\n", __LINE__, findKey);
    #endif

    // ir para o bucket encontrado,
    // i.e., mover cursor do arquivo de dados para o bloco âncora do bucket
    streamHashFileInput.seekg(bucketNumber * QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE, std::ios::beg);
    #ifdef DEBUG
      fprintf(stdout, "- externalHash[%u] cursor movido para posição %ld\n", __LINE__, bucketNumber * QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE);
    #endif

    // ========== procurar o registro em cada bloco desse bucket ============= //
    Bloco bufferPage = { 0 }; // buffer pra o bloco que será carregado
    unsigned long currPage;

    // iternado sobre os blocos do buket
    for (currPage = 0; currPage < QTD_BLOCOS_POR_BUCKET; ++currPage) {
      #ifdef DEBUG
        fprintf(stdout, "- externalHash[%u] bloco corrente = %lu\n", __LINE__, currPage);
      #endif
      streamHashFileInput.read((char*)&bufferPage, BLOCO_SIZE); // ler bloco encontrado ao mover cursor

      // iterando sobre os registros no bloco até encontrar o procurado (ou o último caso não encontre)
      TypeRecord* records = (TypeRecord*) bufferPage.dados; // trata os dados como um arranjo de registros
      for (unsigned long i = 0; i < bufferPage.qtdRegistros; ++i) {
        TypeRecord currRecord = records[i];
        // std::memcpy(&currRecord, (char*)&bufferPage.dados[i * RECORD_SIZE], RECORD_SIZE);

        if (findKey == getKey_(currRecord)) { // encontrou o registro
          #ifdef DEBUG
            fprintf(stdout, "- externalHash[%u] registro encontrado na posição %lu do bloco %lu (bucket %lu)\n", __LINE__, i, currPage, bucketNumber);
          #endif
          recordFind = true;
          record = currRecord;

          return currPage+1;
        }
      }
    }

    return currPage; // registro não encontrado
  }

  /**
   * Para obter a quantidade total de blocos alocados
   * nesta hash externa.
   *
   * @return O número de blocos alocados.
   *
   * @author Micael Levi
   * @date 2017-08-30
   */
  unsigned long getTotalBlocks(void){
    return totalBlocks_;
  }


};



#endif
