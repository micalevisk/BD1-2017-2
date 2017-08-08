//:description: ler CSV formatado usando uma classe específica
//:compile: g++ -std=c++11 001_4.cc -o 001_4.exe

#include "../../../include/csv.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace io;


struct CSVRow {
  int id;
  string titulo;
  int ano;
  string autores;
  string citacoes;
  string atualizacao;//AAAA-MM-DD hh:mm:ss
  string snippet;

  CSVRow(int _id, string _titulo, int _ano, string _autores, string _citacoes, string _atualizacao, string _snippet)
  : id(_id), titulo(_titulo), ano(_ano), autores(_autores), citacoes(_citacoes), atualizacao(_atualizacao), snippet(_snippet) {}
};

class ReadCSV {
  string filename;
  char sep;

  public:
    vector<CSVRow> rows;

    ReadCSV(string _filename, char _sep = ';') : filename(_filename), sep(_sep) {}

    vector<CSVRow> readAllRow(){
      CSVReader< 7, trim_chars<' ', '\t'>, double_quote_escape<';', '\"'> > in(filename);
      int id, ano;
      string titulo, autores, citacoes, atualizacao, snippet;

      while ( in.read_row(id, titulo, ano, autores, citacoes, atualizacao, snippet) ) {
        CSVRow curr(id, titulo, ano, autores, citacoes, atualizacao, snippet);
        rows.push_back(curr);

        cout << id << " ## "
        << titulo  << " ## "
        << ano << " ## "
        << autores << " ## "
        << citacoes << " ## "
        << atualizacao << " ## "
        << snippet << '\n';
      }

      return rows;
    }

};


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

};



size_t countFields(const std::string& str, const char separator = ';'){
  size_t occurrences = std::count(str.begin(), str.end(), separator);
  return occurrences + (!str.empty());
}


int main(){

  string bufferUltimaLinha;
  bool ultimoNaoFinalizado = false;

  const unsigned columnCount = 7;
  LineReader in("../exemplo.input.csv");
  while (char* line = in.next_line()) {
    // printf("{%s}\n", line);

    string currLine = string(line);

    if (ultimoNaoFinalizado) {
      ultimoNaoFinalizado = false;
      bufferUltimaLinha += currLine;
      cout << bufferUltimaLinha << endl;
    }
    else if ( countFields(currLine) != columnCount ) {
      ultimoNaoFinalizado = true;
      bufferUltimaLinha = currLine;
    }
    else {
      cout << currLine << endl;
    }

  }

  /*
  io::CSVReader<
    7,
    io::trim_chars<' ', '\t'>,
    io::double_quote_escape<';','\"'>
  > in("../exemplo.input.csv");
  in.set_header("id", "titulo", "ano", "autores", "citacoes", "atualizacao", "snippet"); // usando quando o arquivo não possui um header
  // in.read_header(io::ignore_missing_column, "id", "titulo", "ano", "autores", "citacoes", "atualizacao", "snippet"); // usado quando a primeira linha é o header

  // colunas:
  int id, ano;
  std::string titulo, autores, citacoes, atualizacao, snippet;

  cout << "id ## titulo ## ano ## autores ## citacoes ## atualizacao ## snippet \n";

  while ( in.read_row(id, titulo, ano, autores, citacoes, atualizacao, snippet) ) {
    cout << id << " ## "
    << titulo  << " ## "
    << ano << " ## "
    << autores << " ## "
    << citacoes << " ## "
    << atualizacao << " ## "
    << snippet << '\n';
  }
  */


}
