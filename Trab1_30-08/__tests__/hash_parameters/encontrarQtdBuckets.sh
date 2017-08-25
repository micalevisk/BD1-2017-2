#!/bin/bash

# função hash: valor % quantidade_de_buckets
FUNCAO_HASH="{ print \$0 % qtdbuckets }"


: '
Admitindo:
1 bloco tem 4096 bytes
1 bloco guarda 1 registro completo

O retorno deste script serve para verificar
quantos blocos irão colidir, i.e., ir para
o mesmo bucket, dado uma determinada quantia de buckets.
'

QTD_BUCKETS=${1:-215000}
TAM_BLOCO=4097 # igual ao tamanho de um registro pois cada bloco guardará apenas 1 registro


QTD_BLOCOS_POR_BUCKET=$(
  awk -v qtdbuckets="$QTD_BUCKETS" "$FUNCAO_HASH" only-ids.txt |
  awk '{ ocorrencias[$0]++ } ; END { for(id in ocorrencias) print id ":" ocorrencias[id] }' |
  sort -k2 -t':' -n |
  tail -n1 |
  ## retorna <id>:<quantidade_maxima_blocos>
  cut -d: -f2
)
echo "(em $QTD_BUCKETS) pelo menos um contém $QTD_BLOCOS_POR_BUCKET blocos/registros de $TAM_BLOCO bytes"
echo "Serão reservados $(( QTD_BUCKETS * (QTD_BLOCOS_POR_BUCKET*TAM_BLOCO) )) bytes"


# cut -d';' -f1 ../../_artigo.csv |
# tr -d '"' |
# awk '{print $0 %215000}' |
# awk '{a[$0]++} ; END{ for(i in a){ print i ":" a[i]} }' |
# sort -k2 -t':' -n
