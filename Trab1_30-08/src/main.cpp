/*
 * Copyright (c) 2017 Micael Levi and Victor Meireles. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 *
 *
 */


#include "interfaceREPL.hpp"
#include "commandsREPL.hpp"

#include <iostream>
#include <algorithm>



using namespace std;


int main(const int argc, const char *argv[])
{
  ios_base::sync_with_stdio(true);
  cin.tie(NULL);

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
