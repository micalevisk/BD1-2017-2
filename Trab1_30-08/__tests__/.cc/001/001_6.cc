//:description: ler CSV (com quebras de linhas nos registros) salvando registros em uma estrutura específica e em um arquivo binário, além de realizar a leitura do mesmo
//:compile: g++ -std=c++11 001_6.cc -lpthread -o 001_6.exe
//:test: g++ -std=c++11 001_6.cc -lpthread -o 001_6.exe && ./001_6.exe && rm ./001_6.exe

#include "../../../include/csv.h"
#include "../../../include/stringUtils.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <climits>



#define Artigo __type_artigo__ // tipo dos registros que serão lidos
#define ARTIGO_SIZE (sizeof(Artigo))
#define ARTIGO_TITULO_MAX_SIZE 300
#define ARTIGO_ATORES_MAX_SIZE 1024
#define ARTIGO_ATUALIZACAO_MAX_SIZE 19
#define ARTIGO_SNIPPET_MAX_SIZE 1024

#define Bloco __type_bloco__ // tipo do bloco que será lido (de uma vez) do disco
#define BLOCO_SIZE (sizeof(Bloco))
#define FATOR_BLOCO (BLOCO_SIZE / ARTIGO_SIZE) // quantidade máxima de registros por bloco


struct Artigo {//2384 bytes
  int id;//4
  int ano;//4
  int citacoes;//4

  char titulo[ARTIGO_TITULO_MAX_SIZE+1];//alfa 300
  char autores[ARTIGO_ATORES_MAX_SIZE+1];//alfa 1024
  char atualizacao[ARTIGO_ATUALIZACAO_MAX_SIZE+1];//19 (AAAA-MM-DD hh:mm:ss)
  char snippet[ARTIGO_SNIPPET_MAX_SIZE+1];//alfa 1024
};

struct Bloco {
  size_t qtdRegistros;//(entre 0 e FATOR_BLOCO - 1) 8 bytes
  char dados[4088];//para completar os 4KB
};



//TODO tratar reformular leitura do CSV real.
#ifdef DONE

struct CSVRow {
  //id;titulo;ano;autores;citacoes;atualizacao;snippet
  Artigo dadosArtigo;

  CSVRow() {}

  /**
   * @param registro Um vector de string contendo os dados de um registro.
   */
  CSVRow(std::vector<std::string> registro) {
    try{
      if (registro.size() != 7) throw 1;
      recdata.id          = StringUtils::removeDoubleQuotesAndParseInt( registro.at(0) );
      recdata.titulo      = StringUtils::removeFirstAndLastDoubleQuotes( registro.at(1) );
      recdata.ano         = StringUtils::removeDoubleQuotesAndParseInt( registro.at(2) );
      recdata.autores     = StringUtils::removeFirstAndLastDoubleQuotes( registro.at(3) );
      recdata.citacoes    = StringUtils::removeDoubleQuotesAndParseInt( registro.at(4) );
      recdata.atualizacao = StringUtils::removeFirstAndLastDoubleQuotes( registro.at(5) );
      recdata.snippet     = StringUtils::removeFirstAndLastDoubleQuotes( registro.at(6) );
    } catch (int errorCode) {
      std::cerr << "error in 'CSVRow' constructor\n";
      exit(errorCode);
    }
  }

  /**
   * Mostra os dados do registro
   * separando-os por "##".
   */
  friend std::ostream& operator <<(std::ostream& out, const CSVRow& d){
    out << d.recdata.id << " ## "
        << d.recdata.titulo << " ## "
        << d.recdata.ano << " ## "
        << d.recdata.autores << " ## "
        << d.recdata.citacoes << " ## "
        << d.recdata.atualizacao << " ## "
        << d.recdata.snippet;
    return out;
  }

  /**
   * Compara pelo id.
   */
  bool operator==(const CSVRow& lhs){
    return recdata.id == lhs.recdata.id;
  }
};



using namespace std;

//OBS: o construtor de 'RowDataType' deve ter 1 parâmetro do tipo 'std::vector<std::string>'
template<class RowDataType, unsigned columnCount, char sep>
class ReadCSV {
  string filename;

  size_t countFields(const string& str){
    size_t occurrences = std::count(str.begin(), str.end(), sep);
    return occurrences + (!str.empty());
  }

  public:
    vector<RowDataType> records;
    unsigned long qtdRegistros;

    ReadCSV(string _filename) : filename(_filename), qtdRegistros(0) {}

    vector<RowDataType> readRecords(){
      io::LineReader in(filename);

      string bufferUltimaLinha;
      bool ultimoNaoFinalizado = false;

      //FIXME erro ao ler o artigo reduzido (leu apenas os 4 primeiros)
      auto adicionarRegistro = [&](string linha){
        unsigned qtdCamposLidos;
        vector<string> registro = StringUtils::split<columnCount>(linha, sep, qtdCamposLidos);
        // vector<string> registro = StringUtils::split<columnCount>(linha, ";\"", qtdCamposLidos);
        if(qtdCamposLidos == columnCount){
          RowDataType curr(registro);
          records.push_back(registro);
        }
      };

      while (char* line = in.next_line()) {
        string currLine = string(line);

        if (ultimoNaoFinalizado) {
          bufferUltimaLinha += currLine;
          if (countFields(bufferUltimaLinha) >= columnCount) {
            ultimoNaoFinalizado = false;
            adicionarRegistro(bufferUltimaLinha);
            qtdRegistros++;
          }
        } else if (countFields(currLine) < columnCount) {
          ultimoNaoFinalizado = true;
          bufferUltimaLinha = currLine;
        } else {
          adicionarRegistro(currLine);
          qtdRegistros++;
        }
      }

      return records;
    }
};

#endif


using namespace std;
int main(int argc, char* argv[]){

  cout << "1 registro terá " << ARTIGO_SIZE << " bytes" << endl;
  cout << "1 bloco terá " << BLOCO_SIZE << " bytes" << endl;
  cout << "  e guardará até " << FATOR_BLOCO << " registros" << endl;


  #ifdef DONE
  if (argc != 3) return 1;

  const char* csvFilename = argv[1];
  const char* outputBinaryFilename = argv[2];

  // ===== ler arquivo de texto CSV
  ReadCSV<CSVRow, 7, ';'> dados(csvFilename);
  dados.readRecords();
  // vector<CSVRow> v = dados.records;
  cout << "leu: " << dados.qtdRegistros << endl;

  // ===== salvar registros em arquivo binário
  std::fstream outputfile;
  outputfile.open(outputBinaryFilename, std::ios::binary | std::ios::out);
  if(!outputfile.is_open()){
   std::cerr << "error while opening the file";
   return EXIT_FAILURE;
  }
  outputfile.write((char*)v.data(), sizeof(CSVRow) * v.size());
  outputfile.close();
  struct stat results;// http://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html
  if (!stat(outputBinaryFilename, &results)) {
   cout << "Análise do arquivo:";
   cout << "\n\tThe file size in bytes: "         << results.st_size;
   cout << "\n\tNumber of blocks allocated: "     << results.st_blocks;
   cout << "\n\tBlock size:"                      << results.st_blksize;
   cout << "\n\tTime of last access: "            << results.st_atime;
   cout << "\n\tTime of last data modification: " << results.st_mtime;
   cout << "\n\tTime of last status change: "     << results.st_ctime;
  }
  // ===== ler registros de arquivo binário
  std::ifstream inputFile;
  inputFile.open(outputBinaryFilename, std::ios::in | std::ios::binary | std::ios::ate);
  if(!inputFile.is_open()){
   std::cerr << "error while opening the file";
   return EXIT_FAILURE;
  }

  inputFile.seekg(0, std::ios::end);// vai para o último byte
  ifstream::pos_type fileAmountBytes = inputFile.tellg();
  int qtdRegistros = fileAmountBytes / sizeof(CSVRow);
  CSVRow* memblock = new CSVRow[qtdRegistros];
  inputFile.seekg(0, std::ios::beg);
  inputFile.read((char*)memblock, fileAmountBytes);
  inputFile.close();
  std::remove(outputBinaryFilename);

  cout << "\nPrimeiro registro:\n\t" << *(&memblock[0]) << endl;
  cout << "Último registro:\n\t"     << *(&memblock[5]) << endl;
  cout << "\n\nTodos Lidos:\n";
  vector<CSVRow> registros(memblock, memblock + qtdRegistros);
  for(const auto& dado : registros){
   cout << dado << endl;
  }
  #endif

}
