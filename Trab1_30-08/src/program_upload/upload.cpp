//
//  [upload.cpp]
//  Created by Micael Levi on 2017-08-26
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
//
//  A priori, recebe o caminho para o arquivo CSV
//  para criar um arquivo binário,
//  o arquivo de dados, usando hashing estática.
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

// ---------------------------------------------------------------------------------------- //


using namespace std;

int main(const int argc, const char *argv[])
{
  if (argc != 2) return 1;// TODO mostrar help (como usar este programa)

  const char* PATH_ARQUIVO_COM_DADOS = argv[1];
  ifstream arqComDados; // arquivo de entrada
  fstream arqDados; // arquivo que contém a hash

  /*
  cout << "tamanho do Artigo = " << ARTIGO_SIZE << endl;
  cout << "bfr = " << FATOR_BLOCO << endl;
  cout << "tamanho do arquivo = " << BUCKET_SIZE * QTD_BUCKETS << endl;
  */


  /* ===================== alocação do arquivo de dados ===================== */
  arqDados.open(PATH_HASH_FILE, fstream::in | fstream::out | fstream::trunc | ios::binary);
  if (!arqDados.is_open()) exit(EXIT_FAILURE);

  Bloco bufferBloco = { 0 };
  // escrevendo os buckets no arquivo
  for(unsigned i=0; i < QTD_BUCKETS; ++i){
    for(unsigned numBloco=0; numBloco < QTD_BLOCOS_POR_BUCKET; ++numBloco){
      // escrever 1 bloco, i.e., alocar espaço para os registros
      arqDados.write((char*)&bufferBloco, BLOCO_SIZE);
    }
  }

  arqDados.close();

  struct stat results;
  if ( !stat(PATH_HASH_FILE, &results) ) {
   fprintf(stderr, "Análise do arquivo:\n");
   fprintf(stderr, "\tThe file size in GB: %f\n", results.st_size / 1000000000.0);
   fprintf(stderr, "\tNumber of blocks allocated: %ld\n", results.st_blocks);
   fprintf(stderr, "\tBlock size in bytes: %ld\n" , results.st_blksize);
  }

  /* ========================os valores para o arquivo de dados ============= */
  arqComDados.open(PATH_ARQUIVO_COM_DADOS);
  if (!arqComDados.is_open()) exit(EXIT_FAILURE);




  arqComDados.close();

  remove(PATH_HASH_FILE);
}
