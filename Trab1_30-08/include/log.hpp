//
//  [log.hpp]
//  Created by Micael Levi and Victor Meireles on 2017-08-31
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//
//  Container de funções utilitárias para
//  padronizar menssagens de erro/depuração/log.
//


#ifndef LOG_HPP
#define LOG_HPP



#include <cstdio>
#include <cstdarg>


namespace Log {

/**
 *
 *
 * @param stream
 * @return fmt
 *
 * @author Micael Levi
 * @date 2017-08-31
 */
void _variadic_(FILE* stream, const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  vfprintf(stream, fmt, args);
  va_end(args);
}

/**
 *
 *
 * @param filename Nome do arquivo fonte em que ocorreu o erro.
 * @param line Número da linha em que ocorreu o erro.
 * @param msg Menssagem de erro.
 *
 * @author Micael Levi
 * @date 2017-08-31
 */
void errorMessageExit(const char* filename, const unsigned& line, const char* msg){
  fprintf(stderr, "{ERROR at %s, line %u: %s}\n", filename, line, msg);
  exit(EXIT_FAILURE);
}

};



#endif
