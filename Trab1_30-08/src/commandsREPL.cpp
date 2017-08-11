/*
 * Copyright (c) 2017 Micael Levi and Victor Meireles. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 *
 *
 */


#include "commandsREPL.hpp"
#include "utils.hpp"


/**
 *
 * @author
 * @date
 */
void _upload(){
  // char* file = (char*) malloc(sizeof(char)*201);
  char file[201];
  int qtdLidos = scanf(" \"%201[^\"]\"", file);

  // string file = string(__file);
  // file = utils::removeFirstAndLastCharFrom(file, '"');
  // file = utils::removeFirstAndLastCharFrom(file, '\'');
  // file = utils::removeLastCharFrom(file, '\n');
  try {
    if (!qtdLidos) throw ERROR_INVALID_PARAMETER;
  } catch (const char* msg) {
    return showError(msg);
  }

  cout << "# arquivo '" << file << "' carregado\n";
}

/**
 *
 * @author
 * @date
 */
void _findrec(){
  int id;

  cin >> id;
  try {
    if (cin.fail()) throw ERROR_INVALID_PARAMETER;
  } catch (const char* msg) {
    return showError(msg);
  }

  cout << "# o registro com ID " << id << " foi encontrado\n";
}

/**
 *
 * @author
 * @date
 */
void _seek1(){
  int id;

  cin >> id;
  try {
    if (cin.fail()) throw ERROR_INVALID_PARAMETER;
  } catch (const char* msg) {
    return showError(msg);
  }

  cout << "# o registro com ID " << id << " foi encontrado\n";
}

/**
 *
 * @author
 * @date
 */
void _seek2(){
  char titulo[201];
  int qtdLidos = scanf(" \"%201[^\"]\"", titulo);

  try {
    if (!qtdLidos) throw ERROR_INVALID_PARAMETER;
  } catch (const char* msg) {
    return showError(msg);
  }

  cout << "# o registro com título '" << titulo << "' foi encontrado\n";
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
