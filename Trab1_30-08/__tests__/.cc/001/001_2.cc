//:description: ler CSV - test 2
//:compile: g++ 001_2.cc -o 001_2.exe

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main (void)
{
  ifstream file("../exemplo.input.csv");
  string id, ano, citacoes;
  string titulo, autores, atualizacao, snippet;
  string buffer;
  char delimitador = ';';

  std::cout << "id ## titulo ## ano ## autores ## citacoes ## atualizacao ## snippet \n";

  while ( getline(file, id, delimitador) ) {
    cout << id << " ## ";

    getline(file, titulo, delimitador) ;
    cout << titulo << " ## ";

    getline(file, ano, delimitador) ;
    cout << ano << " ## ";

    getline(file, autores, delimitador) ;
    cout << autores << " ## ";

    getline(file, citacoes, delimitador) ;
    cout << citacoes << " ## ";

    getline(file, atualizacao, delimitador) ;
    cout << atualizacao << " ## ";

    getline(file, snippet);
    cout << snippet << "\n" ;
  }

  return 0;
}
