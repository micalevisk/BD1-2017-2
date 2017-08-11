//
//  [utils.hpp]
//  Created by Micael Levi on 2017-08-11
//  Copyright (c) 2017 mllc@icomp.ufam.edu.br All rights reserved.
//


#ifndef UTILS_HPP
#define UTILS_HPP



namespace utils {

  /**
   *
   * @author
   * @date
   */
  bool areEquals(std::string str1, std::string str2){
    return !str1.compare(str2);
  }

  /**
   *
   * @author
   * @date
   */
  string removeFirstAndLastCharFrom(string str, char c){
    return (str.empty() || str[0] != c || str[ str.size()-1 ] != c)
           ? str
           : str.substr(1, str.size() - 2);
  }

  /**
   *
   * @author
   * @date
   */
  string removeLastCharFrom(string str, char c){
    return (str.empty() || str[ str.size()-1 ] != c)
           ? str
           : str.substr(0, str.size() - 2);
  }

};



#endif
