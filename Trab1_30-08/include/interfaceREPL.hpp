//
//  [interfaceREPL.hpp]
//  Created by Micael Levi on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//


#ifndef INTERFACEREPL_HPP
#define INTERFACEREPL_HPP



#include <cstdio>


/**
 * Mensagens que deverão ser exibidas
 * ao usuário ao lançar um erro.
 */
static const char* ERROR_COMMAND_NOT_FOUND = "command not found";
static const char* ERROR_INVALID_PARAMETER = "invalid parameter";


inline void showError(const char* message, FILE* stream = stdout)
{
  fprintf(stream, "\033[31mERROR: \033[31;1m%s\033[m\n", message);
}


static void showHelp(FILE* stream = stdout)
{
  fprintf(stream, "# COMANDOS DISPONÍVEIS:\n");
  // fprintf(stream, "# todos devem ser finalizados com ponto e vírgula (';')\n");
  fprintf(stream, "- exit           # apagar tudo e fechar REPL (Read-Eval-Print-Loop)\n");
  fprintf(stream, "- upload \"file\"  # carrega os dados para o banco\n\n");

  fprintf(stream, "# Os comandos abaixo retornam (em relação ao arquivo utilizado):\n");
  fprintf(stream, "# registro encontrado | quantidade de blocos lidos | total de blocos\n");
  fprintf(stream, "- findrec ID       # busca no arquivo dados um registro pelo seu ID {inteiro}\n");
  fprintf(stream, "- seek1 ID         # busca no arquivo de índice primário um registro pelo seu ID {inteiro}\n");
  fprintf(stream, "- seek2 \"TITULO\"   # busca no arquivo de índice secundário um registro pelo seu TITULO {alfa 300}\n\n");
}



#endif
