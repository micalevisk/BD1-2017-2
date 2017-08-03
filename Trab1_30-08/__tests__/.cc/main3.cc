// ler CSV usando o CSVReader

#include "../../include/CSVReader.h"
#include <iostream>

using namespace std;
// using namespace io;

int main(){
  io::CSVReader< 7, io::trim_chars<' ', '\t'>, io::double_quote_escape<';','\"'> > in("../exemplo.csv"); // deve ter a linha de header
  in.read_header(io::ignore_extra_column, "id", "titulo", "ano", "autores", "citacoes", "atualizacao", "snippet");

  // colunas:
  int id, ano;
  std::string titulo, autores, citacoes, atualizacao, snippet;


  cout << "+----+--------+-----+---------+----------+-------------+---------+" << '\n';
  cout << "| ID | TITULO | ANO | AUTORES | CITACOES | ATUALIZACAO | SNIPPET |" << '\n';
  cout << "+----+--------+-----+---------+----------+-------------+---------+" << '\n';

  while ( in.read_row(id, titulo, ano, autores, citacoes, atualizacao, snippet) ) {
    cout << id << " ## "
         << titulo  << " ## "
         << ano << " ## "
         << autores << " ## "
         << citacoes << " ## "
         << atualizacao << " ## "
         << snippet << '\n';
  }

}



// g++ -std=c++11 main3.cc -o main -lpthread
