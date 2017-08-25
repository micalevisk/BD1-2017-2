## Algoritmo para criar o _arquivo de dados_ organizado em hash (estático)
> - sem páginas de overflow, i.e., a quantidade de buckets deve suportar todos os registros de `artigo.csv` (sem colisões)
> - as entradas de dados não serão ordenadas (para facilitar a implementação)

No programa `upload`, a criação do arquivo de dados será da seguinte forma: <br>
Definições:
- PATH_DATA_FILE: o caminho para o arquivo que contém os registros (texto)
- PATH_HASH_FILE: o caminho para o arquivo de dados (binário)
- M:              o número de buckets (cluster de blocos) para a hash externa
- m:              o número de blocos por bucket
- B:              o tamanho de um bloco (4096)
- bfr:            o número de registros por bloco (fator de bloco)
- Artigo:         estrutura de tamanho 2384 bytes para guardar um registro
- Bloco:          estrutura de tamanho 4096 bytes para guardar o valor bfr do bloco e os bfr registros


1. criar arquivo PATH_HASH_FILE para leitura/escrita
2. definir o espaço de endereços de disco (alocar os M buckets, i.e., `M * (m * B)` bytes)
3. **PARA CADA** registro lido para um buffer Artigo, fazer:
   - calcular `bucketAlocado := hash(ID_do_registro)` o número do bucket (que inicia no byte `bucketAlocado * (m * B)`)
   - **PARA CADA** buffer Bloco lido de PATH_HASH_FILE do bucket calculado, fazer:
       + se a quantidade de registros do bloco for maior que bfr, (houve colisão) ir para o próximo
       + senão, inserir registro corrente no bloco corrente, i.e., copiar os bytes do artigo o array `bloco.dados`
          - incrementar em 1 a `bloco.qtdRegistros`
          - voltar o cursor para o início do buffer bloco
          - escrever o buffer bloco no PATH_HASH_FILE
          - fim da escrita do buffer artigo
