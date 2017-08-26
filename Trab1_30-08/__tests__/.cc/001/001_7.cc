//:description: ler CSV (bem quebrado) e exibir os campos lidos. #FIXME
//:compile: g++ -std=c++11 001_7.cc -o 001_7.exe
//:time: x for 1021439 records

#include "../../../include/stringUtils.hpp"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

long unsigned qtdRegistrosLidos = 0;

// leitura de um campo específica para o CSV fornecido
// deve tratar: TODO
// campos com o delimitador
// campos com aspas duplas (aspas aninhadas)
// campos sem valor
// campos com valor "NULL" (sem as aspas)
// campos quebrados em linhas consecutivas
template<const char delimiter = ';'>
string getField(fstream& stream, bool& validField){
  string field;
  char lastChar, currChar;
  unsigned quotesAmount = 0;
  bool end = false;

  auto endOfField = [&](){
    if (   !quotesAmount // é um campo sem as aspas
        || (lastChar == '"' && !(quotesAmount&1)) // OU o último caractere lido foi aspas e a quantidade de aspas é par
       ) end = true;
  };

  do{
    if (!stream.get(currChar)) return "";

    switch(currChar){
      case '"':
        quotesAmount++;
        break;

      case '\r':
        stream.get(); // tratar EOL com CRLF
        break;

      case '\n':
        endOfField();
        goto _end_of_loop;

      case delimiter:
        if (field == "NULL") return field;
        endOfField();

      default:;
    }

    field += currChar;
    _end_of_loop:
    lastChar = currChar;
  }while(!end);

  validField = (field.length() > 0); // caso tenha sido apenas uma linha em branco
  return StringUtils::removeLastCharFrom(field, delimiter);
}

void lerCamposEMostrar(fstream& inputStream){
  while (!inputStream.eof()) {

    for(unsigned qtdCamposLidos=0; qtdCamposLidos < 7; ){
      bool campoValido = false;
      string campoCurr = getField(inputStream, campoValido);

      if (campoValido){
        cout << '{' << campoCurr << '}';
        ++qtdRegistrosLidos;
        qtdCamposLidos++;

        // cout << campoCurr;
        if (qtdCamposLidos <= 6) cout << ';';

      } else if (inputStream.eof()) return;
    }

    cout << "\n\n";
  }
}


int main(int argc, char** argv)
{
  if (argc != 2) return 1;
  fstream inputStream(argv[1]);

  if (inputStream.is_open()) lerCamposEMostrar(inputStream);

  // cout << "leu " << qtdRegistrosLidos/7 << " registros\n";
}
