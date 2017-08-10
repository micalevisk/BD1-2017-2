/*
 * Copyright (c) 2017 Micael Levi and Victor Meireles. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 *
 *
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <climits>

using namespace std;


// == declaração dos programas principais == //
void _upload(string);
void _findrec(unsigned);
void _seek1(unsigned);
void _seek2(unsigned);

vector<string> COMMANDS_AVAILABLE = {"upload", "findrec", "seek1", "seek2"};
const char* ERROR_COMMAND_NOT_FOUND = "command not found!";
const char* ERROR_READ_PARAMETER = "invalid parameter!";


void showHelp(){
  cout << "# comandos válidos:\n\n";
  cout << "- upload \"file\"  # carrega os dados para o banco\n\n";

  cout << "# Os comandos abaixo retornam (em relação ao arquivo utilizado):\n";
  cout << "# registro encontrado | quantidade de blocos lidos | total de blocos\n\n";

  cout << "- findrec \"ID\"   # busca no arquivo dados um registro pelo seu ID\n";
  cout << "- seek1 \"ID\"     # busca no arquivo de índice primário um registro pelo seu ID\n";
  cout << "- seek2 \"TITULO\" #  busca no arquivo de índice secundário um registro pelo seu TITULO\n\n";
}


inline void throwError(const char* message){
  cout << "ERROR: " << message << endl;
}



void commandHandler(string command){
	if (command.empty()) return;
	vector<string>::iterator it = find(COMMANDS_AVAILABLE.begin(), COMMANDS_AVAILABLE.end(), command);
	if (it == COMMANDS_AVAILABLE.end()) return;
	
  string file;
  unsigned id;

  if (!command.compare("upload")) {
    cin >> file;
		try{
			if (cin.fail()) throw 1;
	    _upload(file);
		} catch(int errorcode){
			throwError(ERROR_READ_PARAMETER);
			return;
		}

  } else {
		cin >> id;
		try{	
			if (cin.fail()) throw 2;
			// TODO checar se os arquivos foram criados

			if (!command.compare("findrec")) {
				_findrec(id);
			} else if (!command.compare("seek1")) {
				_seek1(id);
			} else if (!command.compare("seek2")) {
				_seek2(id);
			} else {
				throwError(ERROR_COMMAND_NOT_FOUND);
			}
		} catch(int errorcode){
			throwError(ERROR_READ_PARAMETER);
			return;
		}
  }
}


int main(int argc, char const *argv[]) {
  string userInput;

  showHelp();
  do{//REPL

		cout << "> ";
    cin >> userInput;
    commandHandler(userInput);
		cin.clear();
		cin.ignore(INT_MAX, '\n');

  }while(true);

}




/**
 * 
 * @author
 * @date
 * @param file Caminho para o arquivo de dados do banco.
 */
void _upload(string file){
	cout << "# arquivo '" << file << "' carregado\n";
}

/**
 *
 * @author
 * @date
 * @param id O identificador único do registro.
 */
void _findrec(unsigned id){
	cout << "# o registro com ID " << id << " foi encontrado\n";
}

/**
 *
 * @author
 * @date
 * @param id O identificador único do registro.
 */
void _seek1(unsigned id){
	cout << "# o registro com ID " << id << " foi encontrado\n";
}

/**
 *
 * @author
 * @date
 * @param id O identificador único do registro.
 */
void _seek2(unsigned id){
	cout << "# o registro com ID " << id << " foi encontrado\n";
}
