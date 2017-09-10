//:description: ler CSV (com quebras de linhas nos registros)
//:compile: g++ -std=c++11 001_4.cc -lpthread -o 001_4.exe
//:time: 10~20s for 1021439 records

#include "../../../include/csv.h"
#include <iostream>


size_t countFields(const std::string& str, const char separator = ';'){
  size_t occurrences = std::count(str.begin(), str.end(), separator);
  return occurrences + (!str.empty());
}


using namespace std;

int main(int argc, char* argv[]){
  if (argc != 2) return 1;

  string bufferUltimaLinha;
  bool ultimoNaoFinalizado = false;

  const unsigned columnCount = 7;
  unsigned long qtdRegistros = 0;

  io::LineReader in(argv[1]);
  while (char* line = in.next_line()) {
    string currLine = string(line);

    if (ultimoNaoFinalizado) {
      bufferUltimaLinha += currLine;

      if ( !(countFields(bufferUltimaLinha) < columnCount) ) {
        ultimoNaoFinalizado = false;
        qtdRegistros++;
      }
    }
    else if ( countFields(currLine) < columnCount ) {
      ultimoNaoFinalizado = true;
      bufferUltimaLinha = currLine;
    }
    else {
      qtdRegistros++;
    }

  }


  cout << "leu " << qtdRegistros << " registros\n";
}
