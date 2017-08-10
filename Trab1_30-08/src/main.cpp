/*
* Copyright (c) 2017 Micael Levi and Victor Meireles. All Rights Reserved.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
*
*
*/

#include <iostream>
#include <cstdio>
#include <map>
#include <algorithm>
#include <string>


namespace utils {//TODO mover para header

  bool equals(std::string str1, std::string str2){
    return !str1.compare(str2);
  }

};


using namespace std;
using namespace utils;


// == declaração dos programas principais == //
void _upload(string);
void _findrec(unsigned);
void _seek1(unsigned);
void _seek2(unsigned);
void _exit(void);



/*
// map para comandos com protótipo: vod func(string);
map<string, void (*)(string)> COMMANDS_AVAILABLE1 = { {"upload", _upload} };
// map para comandos com protótipo: void func(unsigned);
map<string, void (*)(unsigned)> COMMANDS_AVAILABLE2 = { {"findrec", _findrec}, {"seek1", _seek1}, {"seek2", _seek2} };
// map para comandos com protótipo: void func(void);
map<string, void (*)(void)> COMMANDS_AVAILABLE3 = { {"exit", _exit} };
*/

map<string, int> COMMANDS_AVAILABLE = { {"exit", 1}, {"upload", 2}, {"findrec", 3}, {"seek1", 4}, {"seek2", 5} };
const char* ERROR_COMMAND_NOT_FOUND = "command not found";
const char* ERROR_READ_PARAMETER = "invalid parameter type";

//

/**
 * @param index
 * @return Ponteiro pra função de um comando
 * que tem o protótipo:
 * void func(unsigned)
 */
auto get_fun(int index) -> void (*)(unsigned) // same as: void (*get_fun(int))(unsigned)
{
  switch (index){
    case 3: return _findrec;
    case 4: return _seek1;
    case 5: return _seek2;
    default: return [](unsigned x){};
  }
}

inline void throwError(const char* message, FILE* stream = stdout)
{
  fprintf(stream, "\033[31mERROR: \033[31;1m%s\033[m\n", message);
}

void showHelp(FILE* stream = stdout)
{
  fprintf(stream, "# COMANDOS DISPONÍVEIS:\n");
  fprintf(stream, "- exit           # apagar tudo e fechar REPL (Read-Eval-Print-Loop)\n");
  fprintf(stream, "- upload \"file\"  # carrega os dados para o banco\n\n");

  fprintf(stream, "# Os comandos abaixo retornam (em relação ao arquivo utilizado):\n");
  fprintf(stream, "# registro encontrado | quantidade de blocos lidos | total de blocos\n");
  fprintf(stream, "- findrec \"ID\"   # busca no arquivo dados um registro pelo seu ID {inteiro}\n");
  fprintf(stream, "- seek1 \"ID\"     # busca no arquivo de índice primário um registro pelo seu ID {inteiro}\n");
  fprintf(stream, "- seek2 \"TITULO\" # busca no arquivo de índice secundário um registro pelo seu TITULO {alfa 300}\n\n");
}

void commandHandler(string command)
{
  int commandIndex = COMMANDS_AVAILABLE[command];
  if (!commandIndex) return throwError(ERROR_COMMAND_NOT_FOUND);

  string file;
  unsigned id;

  if (equals(command, "upload")) {
    cin >> file;
    try{
      if (cin.fail()) throw 1;
      _upload(file);
    } catch(int errorcode){
      throwError(ERROR_READ_PARAMETER);
      return;
    }
  } else if (equals(command, "exit")) {
    _exit();
  } else {
    cin >> id;
    try{
      if (cin.fail()) throw 2;
      // TODO checar se os arquivos foram criados

      auto commandExecution = get_fun(commandIndex);
      commandExecution(id);
      /*
      if (equals(command, "findrec")) {
        _findrec(id);
      } else if (equals(command, "seek1")) {
        _seek1(id);
      } else if (equals(command, "seek2")) {
        _seek2(id);
      }
      */
    } catch(int errorcode){
      throwError(ERROR_READ_PARAMETER);
      return;
    }
  }
}



int main(const int argc, const char *argv[])
{
  string userInput;

  showHelp();
  do{//REPL

    cout << "> ";
    cin >> userInput;
    commandHandler(userInput);
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

  }while(true);

}




/**
 *
 * @author
 * @date
 * @param file Caminho para o arquivo de dados do banco.
 */
void _upload(string file)
{
  cout << "# arquivo '" << file << "' carregado\n";
}

/**
 *
 * @author
 * @date
 * @param id O identificador único do registro.
 */
void _findrec(unsigned id)
{
  cout << "# o registro com ID " << id << " foi encontrado\n";
}

/**
 *
 * @author
 * @date
 * @param id O identificador único do registro.
 */
void _seek1(unsigned id)
{
  cout << "# o registro com ID " << id << " foi encontrado\n";
}

/**
 *
 * @author
 * @date
 * @param id O identificador único do registro.
 */
void _seek2(unsigned id)
{
  cout << "# o registro com ID " << id << " foi encontrado\n";
}

/**
 * Função para o comando 'exit'
 * que apagará os arquivos criados (se existirem)
 * e fechará o programa
 * @author
 * @date
 */
void _exit()
{
  //TODO apagar arquivos criados
  cout << "bye\n";
  exit(0);
}
