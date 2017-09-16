#define Bloco struct __Bloco
// ...
struct __Bloco {// 4096 bytes
  unsigned long qtdRegistros;// (entre 0 e bfr - 1)
  char dados[4084];// para armazenar os bfr registros
};