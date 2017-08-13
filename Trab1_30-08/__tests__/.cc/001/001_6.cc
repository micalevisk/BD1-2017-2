//:description: ler CSV (com quebras de linhas nos registros) salvando registros em uma estrutura específica e em um arquivo binário, além de realizar a leitura do mesmo
//:compile: g++ -std=c++11 001_6.cc -lpthread -o 001_6.exe

#include "../../../include/csv.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <sys/stat.h>


namespace StringUtils {

  // (c) https://stackoverflow.com/questions/16749069/c-split-string-by-regex
  std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
      if (item.length() > 0) elems.push_back(item);
    return elems;
  }

  // (c) https://stackoverflow.com/questions/16749069/c-split-string-by-regex
  std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
  }

  template<unsigned max_elements>
  std::vector<std::string> split(const std::string& str, const char separator, unsigned& amountData){
    std::vector<std::string> elems(max_elements);
    std::stringstream buffer(str);
    std::string currElem;

    for (amountData=0; std::getline(buffer, currElem, separator); ++amountData) {
      if (currElem.length() > 0) elems.at(amountData) = currElem;
    }

    return elems;
  }

  std::string removeFirstAndLast(std::string str, char c){
    return (str.empty() || str[0] != c || str[ str.size()-1 ] != c)
           ? str
           : str.substr(1, str.size() - 2);
  }

  int stringToInt(std::string strNumber){
    try {
      int asInt = stoi(strNumber); //don't call c_str()
      return asInt;
    } catch (std::exception const &e) {
      std::cerr << "error in 'stringToInt': " << e.what() << std::endl;
      return 0;
    }
  }

  std::string removeDoubleQuotes(std::string strWithDoubleQuotes){
    return removeFirstAndLast(strWithDoubleQuotes, '"');
  }

  int removeDoubleQuotesAndParseInt(std::string strNumberWithDoubleQuotes){
    return stringToInt( removeDoubleQuotes(strNumberWithDoubleQuotes) );
  }

};


struct CSVRow {
  int id;
  std::string titulo;
  int ano;
  std::string autores;
  int citacoes;
  std::string atualizacao;//AAAA-MM-DD hh:mm:ss
  std::string snippet;

  virtual const char* what() const throw(){
    return "CSVRow";
  }

  CSVRow() {}

  CSVRow(int _id, std::string _titulo, int _ano, std::string _autores, int _citacoes, std::string _atualizacao, std::string _snippet)
  : id(_id), titulo(_titulo), ano(_ano), autores(_autores), citacoes(_citacoes), atualizacao(_atualizacao), snippet(_snippet) {}

  CSVRow(std::vector<std::string> registro) {
    try{
      if (registro.size() != 7) throw 1;
      this->id          = StringUtils::removeDoubleQuotesAndParseInt( registro.at(0) );
      this->titulo      = StringUtils::removeDoubleQuotes( registro.at(1) );
      this->ano         = StringUtils::removeDoubleQuotesAndParseInt( registro.at(2) );
      this->autores     = StringUtils::removeDoubleQuotes( registro.at(3) );
      this->citacoes    = StringUtils::removeDoubleQuotesAndParseInt( registro.at(4) );
      this->atualizacao = StringUtils::removeDoubleQuotes( registro.at(5) );
      this->snippet     = StringUtils::removeDoubleQuotes( registro.at(6) );
    } catch (int errorCode) {
      std::cerr << "error in 'CSVRow' constructor\n";
      exit(errorCode);
    }
  }

  friend std::ostream& operator <<(std::ostream& out, const CSVRow& d){
    out << d.id << " ## "
        << d.titulo << " ## "
        << d.ano << " ## "
        << d.autores << " ## "
        << d.citacoes << " ## "
        << d.atualizacao << " ## "
        << d.snippet;
    return out;
  }

  bool operator==(const CSVRow& lhs){
    return this->id == lhs.id;
  }
};



using namespace std;

//OBS: o construtor de 'RowDataType' deve ter 1 parâmetro do tipo 'std::vector<std::string>'
template<class RowDataType, unsigned columnCount, char sep>
class ReadCSV {
  string filename;

  size_t countFields(const string& str){
    size_t occurrences = std::count(str.begin(), str.end(), sep);
    return occurrences + (!str.empty());
  }

  public:
    vector<RowDataType> records;

    ReadCSV(string _filename) : filename(_filename) {}

    vector<RowDataType> readRecords(){
      io::LineReader in(filename);

      string bufferUltimaLinha;
      bool ultimoNaoFinalizado = false;
      unsigned qtdCamposLidos;

      auto adicionarRegistro = [&](string linha){
        vector<string> registro = StringUtils::split<columnCount>(linha, sep, qtdCamposLidos);
        RowDataType curr(registro);
        records.push_back(registro);
      };

      while (char* line = in.next_line()) {
        string currLine = string(line);

        if (ultimoNaoFinalizado) {
          bufferUltimaLinha += currLine;
          if (countFields(bufferUltimaLinha) >= columnCount) {
            ultimoNaoFinalizado = false;
            adicionarRegistro(bufferUltimaLinha);
          }
        } else if (countFields(currLine) < columnCount) {
          ultimoNaoFinalizado = true;
          bufferUltimaLinha = currLine;
        } else {
          adicionarRegistro(currLine);
        }
      }

      return records;
    }
};




struct Data {
  int    key;
  double value;
};


int main(int argc, char* argv[]){
  if (argc != 3) return 1;

  const char* csvFilename = argv[1];
  const char* outputBinaryFilename = argv[2];

  // ===== ler arquivo de texto CSV
  ReadCSV<CSVRow, 7, ';'> dados(argv[1]);
  dados.readRecords();
  vector<CSVRow> v = dados.records;


  // ===== salvar registros em arquivo binário
  std::fstream outputfile;
  outputfile.open(outputBinaryFilename, std::ios::binary | std::ios::out);
  if(!outputfile.is_open()){
    std::cerr << "error while opening the file";
    return EXIT_FAILURE;
  }
  outputfile.write((char*)v.data(), sizeof(CSVRow) * v.size());

  outputfile.seekg(0);
  struct stat results;// http://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html
  if (!stat(outputBinaryFilename, &results)) {
    cout << "Análise do arquivo:";
    cout << "\n\tThe file size in bytes: "         << results.st_size;
    cout << "\n\tNumber of blocks allocated: "     << results.st_blocks;
    cout << "\n\tBlock size:"                      << results.st_blksize;
    cout << "\n\tTime of last access: "            << results.st_atime;
    cout << "\n\tTime of last data modification: " << results.st_mtime;
    cout << "\n\tTime of last status change: "     << results.st_ctime;
  }

  outputfile.close();


  // ===== ler registros de arquivo binário
  std::ifstream inputFile;
  inputFile.open(outputBinaryFilename, std::ios::in | std::ios::binary | std::ios::ate);
  if(!inputFile.is_open()){
    std::cerr << "error while opening the file";
    return EXIT_FAILURE;
  }

  int beginning, end;
  inputFile.seekg(0);
  beginning = inputFile.tellg();
  inputFile.seekg(0, std::ios::end);
  end = inputFile.tellg();
  ifstream::pos_type fileAmountBytes = (end - beginning);
  int qtdRegistros = fileAmountBytes / sizeof(CSVRow);
  CSVRow* memblock = new CSVRow[qtdRegistros];
  inputFile.seekg(0, std::ios::beg);
  inputFile.read((char*)memblock, fileAmountBytes);
  inputFile.close();

  cout << "\nPrimeiro registro:\n\t" << *(&memblock[0]) << endl;
  cout << "Último registro:\n\t"     << *(&memblock[5]) << endl;

  cout << "\n\nTodos Lidos:\n";
  vector<CSVRow> registros(memblock, memblock + qtdRegistros);
  for(const auto& dado : registros){
    cout << dado << endl;
  }

  inputFile.close();


/* LIXO

  // ===== salvar em arquivo binário
  Data a; a.key=1; a.value = 2.55;
  Data b; b.key=2; b.value = 3.4551;
  Data *y = new Data[2];
  y[0] = a;
  y[1] = b;

  std::fstream outputfile;
	outputfile.open(outputBinaryFilename, std::ios::binary | std::ios::out);
  if(!outputfile.is_open()){
    std::cerr << "error while opening the file";
    return EXIT_FAILURE;
  }

  vector<Data> myVector(y, y+2);
  outputfile.write((char*)myVector.data(), sizeof(Data) * myVector.size());
  // outputfile.write((char*)&myVector[0], sizeof(Data) * myVector.size());

  outputfile.seekg(0);
  struct stat results;
  if (!stat(outputBinaryFilename, &results))
    cout << "\n\narquivo com " << results.st_size << " bytes\n";

  outputfile.close();

  // ===== ler de arquivo binário
  Data* Y = new Data[2];
  std::ifstream inputFile(outputBinaryFilename, std::ios::in | std::ios::binary);
  inputFile.read((char*)Y, sizeof(Data) * 2);
  cout << Y[0].key << " " << Y[0].value << endl;
  cout << Y[1].key << " " << Y[1].value << endl;

  inputFile.close();
*/
  /*
  const int DIM = 6;
  int array[DIM] = {1,2,3,4,5,6};
  std::vector<int> myVector(array, array + DIM);
  try {
    std::copy(myVector.begin(), myVector.end(), std::ostreambuf_iterator<char>(outputfile));
  } catch (std::exception const &e) {
    std::cerr << "error in '': " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream INFILE(path, std::ios::in | std::ifstream::binary);
  std::istreambuf_iterator iter(INFILE);
  std::copy(iter.begin(), iter.end(), std::back_inserter(newVector));
  */


}
