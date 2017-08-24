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



#define Artigo __dados_registro__
#define ARTIGO_SIZE (sizeof(Artigo))

#define ARTIGO_TITULO_MAX_SIZE 300
#define ARTIGO_ATORES_MAX_SIZE 1024
#define ARTIGO_ATUALIZACAO_MAX_SIZE 19
#define ARTIGO_SNIPPET_MAX_SIZE 1024



struct Artigo {//quase 2.4KB
  int id;//4
  int ano;//4
  int citacoes;//4

  char titulo[ARTIGO_TITULO_MAX_SIZE+1];//alfa 300
  char autores[ARTIGO_ATORES_MAX_SIZE+1];//alfa 1024
  char atualizacao[ARTIGO_ATUALIZACAO_MAX_SIZE+1];//19 (AAAA-MM-DD hh:mm:ss)
  char snippet[ARTIGO_SNIPPET_MAX_SIZE+1];//alfa 1024
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
  #ifdef DONE
  const char* csvFilename = "../exemplo.input.csv";

  // ===== ler arquivo de texto CSV
  ReadCSV<CSVRow, 7, ';'> dados(csvFilename);
  dados.readRecords();
  // vector<CSVRow> v = dados.records;
  cout << "leu: " << dados.qtdRegistros << endl;
  #endif


  cout << "1 registro terá " << sizeof(Artigo) << " bytes" << endl;

}
