/*
 * Copyright (c) 2017 All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Created by Micael Levi and Victor Meireles on 2017-08-11
 *
 * Container de funções utilitárias para
 * padronizar menssagens de erro/depuração/log.
 */


#ifndef LOG_HPP
#define LOG_HPP



#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <iostream>


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
void variadic_printf(FILE* stream, const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  vfprintf(stream, fmt, args);
  va_end(args);
}


/**
 *
 * @tparam separator O caractere que irá separar os valores passados.
 *
 * @author https://stackoverflow.com/questions/27375089/what-is-the-easiest-way-to-print-a-variadic-parameter-pack-using-stdostream
 * @date 2017-08-31
 */
template<const char separator = ' ', typename Arg, typename... Args>
void variadic_print(std::ostream& out, Arg&& arg, Args&&... args){
  out << std::forward<Arg>(arg);
  using expander = int[];
  (void)expander{0, (void(out << separator << std::forward<Args>(args)), 0)...};
}


/**
 *
 * @tparam code O número do erro que será retornado pelo processo.
 *
 * @author Micael Levi
 * @date 2017-08-31
 */
template<int code = EXIT_FAILURE, typename... Args>
void errorMessageExit(Args... args){
  variadic_print(std::cerr, "[ERRO]", args..., '\n');
  exit(code);
}


/**
 *
 *
 *
 * @author Micael Levi
 * @date 2017-08-31
 */
template<typename... Args>
void basicMessage(Args... args){
  variadic_print(std::cout, args..., '\n');
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
