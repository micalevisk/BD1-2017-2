//
//  [commandsREPL.hpp]
//  Created by Micael Levi on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//


#ifndef COMMANDSREPL_HPP
#define COMMANDSREPL_HPP



#include "interfaceREPL.hpp"
#include <iostream>
#include <string>
#include <map>

using namespace std;



// == declaração dos programas principais == //
void _exit(void);
void _upload(void);
void _findrec(void);
void _seek1(void);
void _seek2(void);

/**
 * Mapeia comandos para função com protótipo:
 * void func(void);
 */
static map<string, void (*)(void)> COMMANDS_AVAILABLE = {
   {"exit", _exit}
  ,{"upload", _upload}
  ,{"findrec", _findrec}
  ,{"seek1", _seek1}
  ,{"seek2", _seek2}
};


/**
 *
 * @author
 * @date
 * @param command O comando executado pelo usuário.
 */
static void commandHandler(string command){
  auto commandExecution = COMMANDS_AVAILABLE[command];
  return (commandExecution)
        ? commandExecution()
        : showError(ERROR_COMMAND_NOT_FOUND);
}



#endif
