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


// ============= ARQUIVO QUE SERÁ CRIADO ============= //
#define PATH_HASH_FILE DIR_ARQUIVOS_CRIADOS"__hashfile"

// ============= PARÂMETROS PARA A HASH EXTERNA ============= //
#ifndef QTD_BUCKETS
	#define QTD_BUCKETS 1549147 // M
#endif
#define QTD_BLOCOS_POR_BUCKET 1 // m
#define BUCKET_SIZE (QTD_BLOCOS_POR_BUCKET * BLOCO_SIZE) // m * B
#define FATOR_BLOCO (BLOCO_SIZE / ARTIGO_SIZE) // bfr = quantidade máxima de registros por bloco
#define BLOCO_SIZE (sizeof(Bloco)) // B


/**
 * Descreve como será tratado um bloco
 * lido (ou escrito) do disco para a memória principal.
 * Guarda a quantidade de registros que possui (até bfr registros),
 * seguidos dos mesmos.
 */
struct Bloco {// 4096 bytes
  unsigned qtdRegistros;// (entre 0 e FATOR_BLOCO - 1) 8 bytes
  char dados[4092];// para completar os 4KB
};




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
template<typename F, typename T>
unsigned long applyHashOn(T data, F getKey){
  return getKey(data) % QTD_BUCKETS;
}

/**
 * Insere um registro (que será passado) no arquivo de dados.
 *
 * @param hashFile Uma stream para o arquivo de dados.
 * @param artigo O registro que será inserido.
 * @param getKey Função que retorna (número) a chave que será usada pela função hash.
 * @return True se o registro foi inserido. False se não há espaço no bucket.
 *
 * @author Micael Levi
 * @date 2017-08-30
 */
template<typename F>
bool insertRecordOnHashFile(std::fstream& hashFile, const Artigo& artigo, F getKey){

  // calcular o bucket que será inserido
  unsigned long bucketNumber = applyHashOn(artigo, getKey);

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
  hashFile.write((char*)&bufferPage, BLOCO_SIZE); // escrever a página no arquivo

  return true;
}



#endif