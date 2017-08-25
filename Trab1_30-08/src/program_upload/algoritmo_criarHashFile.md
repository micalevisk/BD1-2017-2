## Algoritmo para criar o _arquivo de dados_ organizado em hash (estático)
> - sem páginas de overflow, i.e., a quantidade de buckets é suporta todos os registros de `artigo.csv` (sem duplicatas)
> - as entradas de dados não serão ordenadas (para facilitar a implementação)

No programa `upload`, a criação do arquivo de dados será da seguinte forma: <br>
Definições:
- PATH_HASH_FILE: o caminho para o arquivo de dados
- M:              o número de buckets (cluster de blocos) para a hash externa
- m:              o número de registros por bucket
- bfr:            o número de registros por bloco (fator de bloco)

1. abrir arquivo binário PATH_HASH_FILE como leitura/escrita
2. definir o espaço de endereços de disco (alocar os M buckets, i.e., _m * M_ bytes)
