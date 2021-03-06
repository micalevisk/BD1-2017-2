//:description: ler CSV - test 1
//:compile: g++ 001_1.cc -o 001_1.exe

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct CSVRow
{
  std::string const& operator[](std::size_t index) const { return m_data[index]; }
  std::size_t size() const { return m_data.size(); }

  void readNextRow(std::istream& str){
    std::string line;
    std::getline(str, line);

    std::stringstream lineStream(line);
    std::string cell;

    m_data.clear();
    while(std::getline(lineStream, cell, ';')) m_data.push_back(cell);
    if (!lineStream && cell.empty()) m_data.push_back("");
  }

  private:
    std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data){
  data.readNextRow(str);
  return str;
}

struct CSVIterator
{
  typedef std::input_iterator_tag     iterator_category;
  typedef CSVRow                      value_type;
  typedef std::size_t                 difference_type;
  typedef CSVRow*                     pointer;
  typedef CSVRow&                     reference;

  CSVIterator(std::istream& str)  :m_str(str.good()?&str:NULL) { ++(*this); }
  CSVIterator()                   :m_str(NULL) {}

  // Pre Increment
  CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = NULL;}}return *this;}
  // Post increment
  CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
  CSVRow const& operator*()   const       {return m_row;}
  CSVRow const* operator->()  const       {return &m_row;}

  bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));}
  bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}

  private:
    std::istream*       m_str;
    CSVRow              m_row;
};


int main()
{
    std::ifstream  file("../exemplo.input.csv");

    std::cout << "id ## titulo ## ano ## autores ## citacoes ## atualizacao ## snippet \n";

    for(CSVIterator loop(file); loop != CSVIterator(); ++loop){
      for(unsigned i=0; i < 6; ++i){
        std::cout<< (*loop)[i] << " ## ";
      }
      std::cout<< (*loop)[6] << "\n";
    }
}
