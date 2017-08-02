// ler CSV - test 2
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main (void)
{
  ifstream file("../exemplo.csv");
  string id, ano, citacoes;
  string titulo, autores, atualizacao, snippet;
  string buffer;
  char delimitador = ',';

  if(!getline(file, buffer)) return 1;

  while ( getline(file, id, delimitador) ) {
    cout << "id: " << id << "\n" ;

    getline(file, titulo, delimitador) ;
    cout << "titulo: " << titulo << "\n" ;

    getline(file, ano, delimitador) ;
    cout << "ano: " << ano << "\n" ;

    getline(file, autores, delimitador) ;
    cout << "autores: " << autores << "\n" ;

    getline(file, citacoes, delimitador) ;
    cout << "citacoes: " << citacoes << "\n" ;

    getline(file, atualizacao, delimitador) ;
    cout << "atualizacao: " << atualizacao << "\n" ;

    getline(file, snippet);
    cout << "snippet: " << snippet << "\n" ;

    cout << "\n";

  }

  return 0;
}
