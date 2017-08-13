//
//  [stringUtils.hpp]
//
//  Container de funções utilitárias que trabalham
//  em cima de uma ou mais strings.
//
//  Created by Micael Levi and Victor Meireles on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//


#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP



#include <stdexcept>

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
  string removeFirstAndLastCharFrom(string str, char c){
    return (str.empty() || str[0] != c || str[ str.size()-1 ] != c)
           ? str
           : str.substr(1, str.size() - 2);
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
      int asInt = stoi(strNumber);
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
  std::vector<std::string> split(const std::string& str, const char delim, unsigned& amountData){
    std::vector<std::string> elems(max_elements);
    std::stringstream buffer(str);
    std::string currElem;

    for (amountData=0; std::getline(buffer, currElem, delim); ++amountData) {
      if (currElem.length() > 0)
        elems.at(amountData) = currElem;
    }

    return elems;
  }

};



#endif
