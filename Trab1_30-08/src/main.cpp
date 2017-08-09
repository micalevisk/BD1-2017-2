/*
 * Copyright (c) 2017 Micael Levi and Victor Meireles. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 *
 *
 */

#include <iostream>
#include <cstdio>

using namespace std;


// == declaração dos programas principais == //
void _upload(string);
void _findrec(unsigned);
void _seek1(unsigned);
void _seek2(unsigned);


void showHelp(){

  cout << "# comandos válidos:\n\n";
  cout << "- upload \"file\"  # carrega os dados para o banco\n\n";

  cout << "# Os comandos abaixo retornam (em relação ao arquivo utilizado):\n";
  cout << "# registro encontrado | quantidade de blocos lidos | total de blocos\n\n";

  cout << "- findrec \"ID\"   # busca no arquivo dados um registro pelo seu ID\n";
  cout << "- seek1 \"ID\"     # busca no arquivo de índice primário um registro pelo seu ID\n";
  cout << "- seek2 \"TITULO\" #  busca no arquivo de índice secundário um registro pelo seu TITULO\n\n";

}


inline void throwError(string message){
  cout << "ERROR: " << message << endl;
}

// FIXME: switch case com string
void commandHandler(string command){
  string file;
  unsigned id;

  switch (command) {
    case "upload":
      cin >> file;
      _upload(file);
      break;

    case "findrec":
      cin >> id;
      _findrec(id);
      break;

    case "seek1":
      cin >> id;
      _seek1(id);
      break;

    case "seek2":
      cin >> id;
      _seek2(id);
      break;

    default:
      throwError("command not found");

  }
}


int main(int argc, char const *argv[]) {
  string userInput;

  showHelp();

  do{//REPL

    cin >> userInput;
    commandHandler(userInput);

  }while(true);

}
