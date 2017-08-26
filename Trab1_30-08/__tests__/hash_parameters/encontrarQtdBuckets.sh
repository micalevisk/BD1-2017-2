#!/bin/bash

ARQUIVO_CSV="../_artigo.csv"

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
QTD_BLOCOS_POR_BUCKET=${2:-1}
QTD_REGISTROS_POR_BLOCO=${3:-1} #7
TAM_BLOCO=4096

# maior número de colisões
QTD_COLISOES=$(
  # awk -F';' -v qtdbuckets="$QTD_BUCKETS" '$1 ~ /^"[0-9]+"/ { gsub(/"/, "", $1) ; print $1 % qtdbuckets } ' "$ARQUIVO_CSV" |
  awk -v qtdbuckets="$QTD_BUCKETS" "$FUNCAO_HASH" only-ids.txt |
  awk '{ ocorrencias[$0]++ } ; END { for(id in ocorrencias) print id ":" ocorrencias[id] }' |
  sort -k2 -t':' -n |
  tail -n1 |
  ## retorna <id>:<quantidade_maxima_blocos>
  cut -d: -f2
)

echo -e "(com $QTD_BUCKETS buckets com $QTD_BLOCOS_POR_BUCKET blocos cada e com $QTD_REGISTROS_POR_BLOCO registros por bloco)\na quantidade máxima de colisões foi: $((QTD_COLISOES - 1))"



TAM_BUCKET=$(( QTD_BLOCOS_POR_BUCKET * TAM_BLOCO ))
TAM_ARQUIVO_DADOS=$(( TAM_BUCKET * QTD_BUCKETS ))

TAM_ARQUIVO_DADOS_GB=$(echo "scale=5; $TAM_ARQUIVO_DADOS/1000000000" | bc)

echo "Serão reservados $TAM_ARQUIVO_DADOS bytes = $TAM_ARQUIVO_DADOS_GB gigabytes"
