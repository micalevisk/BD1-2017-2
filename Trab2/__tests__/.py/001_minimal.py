##:description: usando o psycopg2 (c) http://www.devmedia.com.br/como-criar-uma-conexao-em-postgresql-com-python/34079
##:run: python3 001_minimal.py

import sys
import psycopg2 # precisa ser instalado

try:
  con = psycopg2.connect(host='localhost', database='postgres', user='bd1', password='bd1')
except:
  sys.exit("erro ao conectar")

cur = con.cursor()

try:
  sql = 'CREATE TABLE cidade (id SERIAL PRIMARY KEY, nome VARCHAR(100) UNIQUE NOT NULL, uf VARCHAR(2) NOT NULL)'
  cur.execute(sql)
except:
  print('[ERROR]:', cur.query, cur.withhold)
  con.commit()

try:
  query_template = 'INSERT INTO cidade (nome, uf) VALUES(%s, %s)'
  cur.execute(query_template, ('São Paulo', 'SP'))
except:
  print('[ERROR]:', cur.query)
  con.commit()

con.commit()
cur.execute('SELECT * FROM cidade')

recset = cur.fetchall()

for rec in recset:
  print(rec)

con.close()
