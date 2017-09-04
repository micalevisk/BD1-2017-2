//:description: ler CSV (com quebras de linhas nos registros) salvando registros em uma estrutura específica
//:compile: g++ -std=c++11 001_5.cc -lpthread -o 001_5.exe

#include "../../../include/csv.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>


namespace StringUtils {

  // (c) https://stackoverflow.com/questions/16749069/c-split-string-by-regex
  std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
      if (item.length() > 0) elems.push_back(item);
    return elems;
  }

  // (c) https://stackoverflow.com/questions/16749069/c-split-string-by-regex
  std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
  }

  template<unsigned max_elements>
  std::vector<std::string> split(const std::string& str, const char separator, unsigned& amountData){
    std::vector<std::string> elems(max_elements);
    std::stringstream buffer(str);
    std::string currElem;

    for (amountData=0; std::getline(buffer, currElem, separator); ++amountData) {
      if (currElem.length() > 0) elems.at(amountData) = currElem;
    }

    return elems;
  }

  std::string removeFirstAndLast(std::string str, char c){
    return (str.empty() || str[0] != c || str[ str.size()-1 ] != c)
           ? str
           : str.substr(1, str.size() - 2);
  }

  int stringToInt(std::string strNumber){
    try {
      int asInt = stoi(strNumber); //don't call c_str()
      return asInt;
    } catch (std::exception const &e) {
      std::cerr << "error in 'stringToInt': " << e.what() << std::endl;
      return 0;
    }
  }

  std::string removeDoubleQuotes(std::string strWithDoubleQuotes){
    return removeFirstAndLast(strWithDoubleQuotes, '"');
  }

  int removeDoubleQuotesAndParseInt(std::string strNumberWithDoubleQuotes){
    return stringToInt( removeDoubleQuotes(strNumberWithDoubleQuotes) );
  }

};


struct CSVRow {
  int id;
  std::string titulo;
  int ano;
  std::string autores;
  int citacoes;
  std::string atualizacao;//AAAA-MM-DD hh:mm:ss
  std::string snippet;

  virtual const char* what() const throw(){
    return "CSVRow";
  }


  CSVRow(int _id, std::string _titulo, int _ano, std::string _autores, int _citacoes, std::string _atualizacao, std::string _snippet)
  : id(_id), titulo(_titulo), ano(_ano), autores(_autores), citacoes(_citacoes), atualizacao(_atualizacao), snippet(_snippet) {}

  CSVRow(std::vector<std::string> registro) {
    try{
      if (registro.size() != 7) throw 1;
      this->id          = StringUtils::removeDoubleQuotesAndParseInt( registro.at(0) );
      this->titulo      = StringUtils::removeDoubleQuotes( registro.at(1) );
      this->ano         = StringUtils::removeDoubleQuotesAndParseInt( registro.at(2) );
      this->autores     = StringUtils::removeDoubleQuotes( registro.at(3) );
      this->citacoes    = StringUtils::removeDoubleQuotesAndParseInt( registro.at(4) );
      this->atualizacao = StringUtils::removeDoubleQuotes( registro.at(5) );
      this->snippet     = StringUtils::removeDoubleQuotes( registro.at(6) );
    } catch (int errorCode) {
      std::cerr << "error in 'CSVRow' constructor\n";
      exit(errorCode);
    }
  }

  friend std::ostream& operator <<(std::ostream& out, const CSVRow& d){
    out << d.id << " ## "
        << d.titulo << " ## "
        << d.ano << " ## "
        << d.autores << " ## "
        << d.citacoes << " ## "
        << d.atualizacao << " ## "
        << d.snippet;
    return out;
  }
};



using namespace std;

//OBS: o construtor de 'RowDataType' deve ter 1 parâmetro do tipo 'std::vector<std::string>'
template<class RowDataType, unsigned columnCount, char sep>
class ReadCSV {
  string filename;

  size_t countFields(const string& str){
    size_t occurrences = count(str.begin(), str.end(), sep);
    return occurrences + (!str.empty());
  }

  public:
    vector<RowDataType> records;

    ReadCSV(string _filename) : filename(_filename) {}

    vector<RowDataType> readRecords(){
      io::LineReader in(filename);

      string bufferUltimaLinha;
      bool ultimoNaoFinalizado = false;
      unsigned qtdCamposLidos;

      auto adicionarRegistro = [&](string linha){
        vector<string> registro = StringUtils::split<columnCount>(linha, sep, qtdCamposLidos);
        RowDataType curr(registro);
        records.push_back(registro);
      };

      while (char* line = in.next_line()) {
        string currLine = string(line);

        if (ultimoNaoFinalizado) {
          bufferUltimaLinha += currLine;
          if (countFields(bufferUltimaLinha) >= columnCount) {
            ultimoNaoFinalizado = false;
            adicionarRegistro(bufferUltimaLinha);
          }
        } else if (countFields(currLine) < columnCount) {
          ultimoNaoFinalizado = true;
          bufferUltimaLinha = currLine;
        } else {
          adicionarRegistro(currLine);
        }
      }

      return records;
    }
};


int main(int argc, char* argv[]){
  if (argc != 2) return 1;

  ReadCSV<CSVRow, 7, ';'> dados(argv[1]);
  dados.readRecords();
  cout << "id ## titulo ## ano ## autores ## citacoes ## atualizacao ## snippet \n";
  vector<CSVRow> v = dados.records;

  for(const auto& dado : v){
    cout << dado << endl;
  }
}
