//
//  [stringUtils.hpp]
//  Created by Micael Levi and Victor Meireles on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//
//  Container de funções utilitárias que trabalham
//  em cima de uma ou mais strings.
//


#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP



#include <stdexcept>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>

namespace StringUtils {

/**
 * Verifica o conteúdo de duas strings
 * comparando apenas se são iguais.
 *
 * @param str1
 * @param str2
 * @return True se str1 e str2 tiverem o mesmo conteúdo.
 *
 * @author Micael Levi
 * @date 2017-08-11
 */
bool areEquals(std::string str1, std::string str2){
  return !str1.compare(str2);
}

/**
 * Remove um caractere do início de uma string, i.e.,
 * caso ele esteja na posição inicial.
 *
 * @param str A string alvo.
 * @param c O caractere que será removido.
 * @return A string alterada, se o caractere foi encontrado.
 *
 * @author Micael Levi
 * @date 2017-08-22
 */
std::string removeFirstCharFrom(std::string str, const char &c){
  if (!str.empty() && str[0] == c) str.erase(str.begin());
  return str;
}

/**
 * Remove o último caractere de uma string, i.e.,
 * caso ele esteja na posição string.length() - 1.
 *
 * @param str A string alvo.
 * @param c O caractere que será removido.
 * @return A string alterada, se o caractere foi encontrado.
 *
 * @author Micael Levi
 * @date 2017-08-26
 */
std::string removeLastCharFrom(std::string str, const char &c){
  if (!str.empty() && str[str.length()-1] == c) str.resize(str.length()-1);
  return str;
  /*
  return (str.empty() || str[str.length()-1] != c)
         ? str
         : str.substr(0, str.length()-1);
  */
}

/**
 * Remove um caractere do início e do final de uma string,
 * caso ele esteja nessas duas posições.
 *
 * @param str A string alvo.
 * @param c O caractere que será removido.
 * @return A string alterada.
 *
 * @author Micael Levi
 * @date 2017-08-11
 */
std::string removeFirstAndLastCharFrom(std::string str, char c){
  return (str.empty() || str[0] != c || str[ str.length()-1 ] != c)
         ? str
         : str.substr(1, str.length() - 2);
}

/**
 * Remove a aspas duplas do início e do final
 * de uma string, caso ela tenha.
 *
 * @param strWithDoubleQuotes A string alvo que começa e termina com aspas duplas.
 * @return A string com as aspas duplas removidas.
 *
 * @see StringUtils::removeFirstAndLastCharFrom
 * @author Micael Levi
 * @date 2017-08-11
 */
std::string removeFirstAndLastDoubleQuotes(std::string strWithDoubleQuotes){
  return removeFirstAndLastCharFrom(strWithDoubleQuotes, '"');
}

/**
 * Realiza a conversão de uma string para inteiro.
 *
 * @param strNumber A string alvo, cujo conteúdo é um inteiro.
 * @return O valor inteiro da string alvo.
 *
 * @throws std::exception Lançado se a string não contiver um inteiro.
 * @author Micael Levi
 * @date 2017-08-11
 */
int stringToInt(std::string strNumber){
  try {
    int asInt = std::stoi(strNumber);
    return asInt;
  } catch (std::exception const &e) {
    throw std::invalid_argument( "error in 'stringToInt': " + std::string(e.what()) );
  }
}

/**
 * Remove as aspas duplas do início e do final de uma string
 * e converte para inteiro, se possível.
 *
 * @param strNumberWithDoubleQuotes A string (um inteiro) que começa e termina com aspas duplas.
 * @return O valor inteiro da string alvo.
 *
 * @see StringUtils::stringToInt
 * @see StringUtils::removeFirstAndLastDoubleQuotes
 * @author Micael Levi
 * @date 2017-08-11
 */
int removeDoubleQuotesAndParseInt(std::string strNumberWithDoubleQuotes){
  return stringToInt( removeFirstAndLastDoubleQuotes(strNumberWithDoubleQuotes) );
}

/**
 * Dividr uma string em uma lista de strings
 * ao separar a string em substrings por um caractere "delimitador".
 *
 * @param str A string alvo.
 * @param delim O caractere que separa os itens.
 * @param elems
 * @return Um vector de string contendo os itens.
 *
 * @see std::getline
 * @author https://stackoverflow.com/questions/16749069/c-split-string-by-regex
 * @date 2017-08-11
 */
std::vector<std::string>& split(const std::string &str, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(str);
  std::string item;
  while (std::getline(ss, item, delim))
    if (item.length() > 0) elems.push_back(item);
  return elems;
}

/**
 * Dividr uma string em uma lista de strings
 * ao separar a string em substrings por um caractere "delimitador".
 *
 * @param str A string alvo.
 * @param delim O caractere que separa os itens.
 * @return Um vector de string contendo os itens.
 *
 * @see StringUtils::split
 * @author https://stackoverflow.com/questions/16749069/c-split-string-by-regex
 * @date 2017-08-11
 */
std::vector<std::string> split(const std::string &str, char delim) {
  std::vector<std::string> elems;
  split(str, delim, elems);
  return elems;
}

/**
 * Dividr uma string em uma lista de strings
 * ao separar a string em substrings por um caractere "delimitador".
 * Mas com a otimização de já alocar a lista com um tamanho máximo/exato.
 *
 * @tparam max_elements A quantidade máxima de itens que deverá obtido.
 * @param str A string alvo.
 * @param delim O caractere que separa os itens.
 * @param amountData Guarda a quantidade de itens obtidos.
 * @return Um vector de string contendo os itens.
 *
 * @see std::getline
 * @author Micael Levi
 * @date 2017-08-11
 */
template<unsigned max_elements>
std::vector<std::string> split(const std::string& str, const char &delim, unsigned &amountData){
  std::vector<std::string> elems(max_elements);
  std::stringstream buffer(str);
  std::string currElem;

  for (amountData=0; (amountData < max_elements) && std::getline(buffer, currElem, delim); ++amountData) {
    if (currElem.length() > 0)
    elems.at(amountData) = currElem;
  }

  return elems;
}

/**
 * Dividr uma string em uma lista de strings
 * ao separar a string em substrings por uma string "delimitadora".
 * Mas com a otimização de já alocar a lista com um tamanho máximo/exato.
 *
 * @tparam max_elements A quantidade máxima de itens que deverá obtido.
 * @param str A string alvo.
 * @param delim A string que separa os itens.
 * @return Um vector de string contendo os itens.
 *
 * @author https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
 * @date 2017-08-26
 */
template<unsigned max_elements>
std::vector<std::string> split(std::string str, const std::string &delim){
    std::string token;
    std::vector<std::string> fields(max_elements);
    size_t pos, delimLength = delim.length();

    unsigned fieldsCount = 0;
    while ( ((pos = str.find(delim)) != std::string::npos) && (fieldsCount < max_elements - 1) ) {
        token = str.substr(0, pos);
        fields.at(fieldsCount++) = token;

        str.erase(0, pos + delimLength);
    }
    fields.at(fieldsCount) = str;

    return fields;
}

/**
 * Copia os N primeiros caracteres de uma string
 * para um vetor de caracteres.
 *
 * @param str A string alvo.
 * @param asCharArray O destino da string (memória previamente alocada).
 * @param length Quantidade de caracteres que serão copiados da string alvo.
 *
 * @author Micael Levi
 * @date 2017-08-30
 */
void stringToCharArray(const std::string& str, char* asCharArray, size_t length){
  std::memcpy(asCharArray, &str[0], length);
  // str.resize(length);
  //std::strcpy(asCharArray, str.c_str());
}


};



#endif
