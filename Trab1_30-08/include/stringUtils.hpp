//
//  [stringUtils.hpp]
//
//  Created by Micael Levi on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//


#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP



namespace StringUtils {

  /**
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
   *
   * @param strWithDoubleQuotes A string alvo que começa e termina com aspas duplas.
   * @return A string com as aspas duplas removidas.
   *
   * @author Micael Levi
   * @date 2017-08-11
   */
  std::string removeFirstAndLastDoubleQuotes(std::string strWithDoubleQuotes){
    return removeFirstAndLastCharFrom(strWithDoubleQuotes, '"');
  }

  /**
   *
   * @author Micael Levi
   * @date 2017-08-11
   */
  int stringToInt(std::string strNumber){
    try {
      int asInt = stoi(strNumber); //don't call c_str()
      return asInt;
    } catch (std::exception const &e) {
      std::cerr << "error in 'stringToInt': " << e.what() << std::endl;
      return 0;
    }
  }

  /**
   *
   * @param strNumberWithDoubleQuotes A string (um inteiro) que começa e termina com aspas duplas.
   * @return O valor inteiro da string alvo.
   *
   * @author Micael Levi
   * @date 2017-08-11
   */
  int removeDoubleQuotesAndParseInt(std::string strNumberWithDoubleQuotes){
    return stringToInt( removeFirstAndLastDoubleQuotes(strNumberWithDoubleQuotes) );
  }

  /**
   *
   * @param str A string alvo.
   * @param delim O caractere que separa os itens.
   * @param elems
   * @return Um vector de string contendo os itens.
   *
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
   *
   * @param str A string alvo.
   * @param delim O caractere que separa os itens.
   * @return Um vector de string contendo os itens.
   *
   * @author https://stackoverflow.com/questions/16749069/c-split-string-by-regex
   * @date 2017-08-11
   */
  std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> elems;
    split(str, delim, elems);
    return elems;
  }

  /**
   *
   * @tparam max_elements A quantidade máxima de itens que deverá obtido.
   * @param str A string alvo.
   * @param delim O caractere que separa os itens.
   * @param amountData Guarda a quantidade de itens obtidos.
   * @return Um vector de string contendo os itens.
   *
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
