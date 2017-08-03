//:description: ler CSV usando o CSVReader
//:compile: g++ -std=c++11 001_3.cc -lpthread -o 001_3.exe

#include "../../../include/CSVReader.hpp"
#include <iostream>

using namespace std;
// using namespace io;

int main(){
  io::CSVReader< 7, io::trim_chars<' ', '\t'>, io::double_quote_escape<';','\"'> > in("../exemplo.input.csv");
  in.set_header("id", "titulo", "ano", "autores", "citacoes", "atualizacao", "snippet"); // usando quando o arquivo não possui um header
  // in.read_header(io::ignore_extra_column, "id", "titulo", "ano", "autores", "citacoes", "atualizacao", "snippet"); // usado quando a primeira linha é o header

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
}
