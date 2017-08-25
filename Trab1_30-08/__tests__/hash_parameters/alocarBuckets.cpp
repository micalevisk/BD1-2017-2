// cria um arquivo binário (temporário)
// que contém uma quantidade de buckets de

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstring>


#define ARQUIVO_HASH "__hashfile"
#define QTD_BUCKETS 215000
#define QTD_BLOCOS_POR_BUCKET 1


using namespace std;


struct Bloco {//4096B
  size_t qtdRegistros;
  char dados[4088];
};



int main()
{

  fstream arqDados(ARQUIVO_HASH, fstream::in | fstream::out | fstream::trunc | ios::binary);
  if(!arqDados.is_open()) return 1;

  Bloco bufferBloco = { .qtdRegistros = 0 };
  // strcpy(bufferBloco.dados, "some data");
  bufferBloco.dados[0] = '\0';

  // escrevendo os buckets no arquivo
  for(unsigned i=0; i < QTD_BUCKETS; ++i){
    arqDados.write((char*)&bufferBloco, sizeof(Bloco) * QTD_BLOCOS_POR_BUCKET);
    bufferBloco.qtdRegistros = 1;
  }

  // lendo blocos escritos
  arqDados.seekg(0 * sizeof(Bloco), ios::beg);// posição do primeiro bloco do primeiro bucket
  arqDados.read((char*)&bufferBloco, sizeof(Bloco));
  cout << "O primeiro bloco contém " << bufferBloco.qtdRegistros << " registros" << endl;
  arqDados.seekg(1 * sizeof(Bloco), ios::beg);// posição do segundo bloco do primeiro bucket (ou primeiro bloco do segundo bucket)
  arqDados.read((char*)&bufferBloco, sizeof(Bloco));
  cout << "O segundo bloco contém " << bufferBloco.qtdRegistros << " registros" << endl;

  arqDados.close();

  struct stat results;
  if (!stat(ARQUIVO_HASH, &results)) {
   cout << "Análise do arquivo:";
   printf("\n\tThe file size in GB: %f", results.st_size / 1000000000.0);
   printf("\n\tNumber of blocks allocated: %ld", results.st_blocks);
   printf("\n\tBlock size: %ld bytes" , results.st_blksize);
  }

  remove(ARQUIVO_HASH);
  cout << "\nTamanho de 1 Bloco em bytes: " << sizeof(Bloco) << endl;
}
