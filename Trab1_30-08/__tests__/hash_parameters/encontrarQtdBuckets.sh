#!/bin/bash

: '
Admitindo:
1 bloco tem 4096 bytes
1 bloco guarda 1 registro completo

O retorno deste script serve para verificar
quantos blocos irão colidir, i.e., ir para
o mesmo bucket,
dado uma determinada quantia de buckets.
'

QTD_BUCKETS=${1:-215000}

awk -v qtdbuckets="$QTD_BUCKETS" '{ print $0 % qtdbuckets }' only-ids.txt |
#               ^- função hash sobre o ID
awk '{ ocorrencias[$0]++ } ; END { for(id in ocorrencias) print id ":" ocorrencias[id] }' |
sort -k2 -t':' -n |
tail -n1 |
## retorna <id>:<quantidade_maxima_blocos>
sed -r "s/.+:(.+)$/(em $QTD_BUCKETS buckets) pelo menos um contém \1 blocos./"



# cut -d';' -f1 ../../_artigo.csv |
# tr -d '"' |
# awk '{print $0 %215000}' |
# awk '{a[$0]++} ; END{ for(i in a){ print i ":" a[i]} }' |
# sort -k2 -t':' -n
