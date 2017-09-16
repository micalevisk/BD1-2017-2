#define Artigo struct __Artigo
// ...
struct __Artigo {
  int id, ano, citacoes;
  char atualizacao[ARTIGO_ATUALIZACAO_MAX_SIZE + 1];
  char titulo[ARTIGO_TITULO_MAX_SIZE + 1];
  char autores[ARTIGO_AUTORES_MAX_SIZE + 1];
  char snippet[ARTIGO_SNIPPET_MAX_SIZE + 1];
};