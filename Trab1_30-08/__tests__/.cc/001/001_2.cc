//:description: ler CSV - test 2
//:compile: g++ 001_2.cc -o 001_2.exe

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

string removeFirstAndLast(string str, char c){
  return (str.empty() || str[0] != c || str[ str.size()-1 ] != c)
         ? str
         : str.substr(1, str.size() - 2);
}

string removeFirstAndLastDoubleQuotes(string str){
  return removeFirstAndLast(str, '"');
}

int main (void)
{
  ifstream file("../exemplo.input.csv");
  string id, ano, citacoes;
  string titulo, autores, atualizacao, snippet;
  string buffer;
  char delimitador = ';';

  std::cout << "id ## titulo ## ano ## autores ## citacoes ## atualizacao ## snippet \n";

  while ( getline(file, id, delimitador) ) {
    cout << removeFirstAndLastDoubleQuotes(id) << " ## ";

    getline(file, titulo, delimitador) ;
    cout << removeFirstAndLastDoubleQuotes(titulo) << " ## ";

    getline(file, ano, delimitador) ;
    cout << removeFirstAndLastDoubleQuotes(ano) << " ## ";

    getline(file, autores, delimitador) ;
    cout << removeFirstAndLastDoubleQuotes(autores) << " ## ";

    getline(file, citacoes, delimitador) ;
    cout << removeFirstAndLastDoubleQuotes(citacoes) << " ## ";

    getline(file, atualizacao, delimitador) ;
    cout << removeFirstAndLastDoubleQuotes(atualizacao) << " ## ";

    getline(file, snippet);
    cout << removeFirstAndLastDoubleQuotes(snippet) << "\n" ;
  }

  return 0;
}
