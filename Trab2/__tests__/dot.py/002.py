##:description: usando o psycopg2 para criar um BD, criar tabela, inserir registro e listar
##:run: python3 002.py

import sys
import psycopg2
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT
from psycopg2.extras import DictCursor


## ----------------------------------------------- ##
class DATABASE_CONFIGS:
  root = "postgres"
  name = "db_micaellevi_victorroque"
  host = "localhost"
  port = "5432"
  user = "bd1"
  password = "bd1"
## ----------------------------------------------- ##

# template que inserção segura na tabela 'cidade', i.e., sem duplicatas (já que a tabela ainda não possui restrições); admitindo 'id' como PK
QUERY_INSERT_CIDADE = """
INSERT INTO cidade (nome, uf)
SELECT '{nome}', '{uf}'
WHERE NOT EXISTS (
  SELECT id FROM cidade
  WHERE
    nome = '{nome}' AND uf = '{uf}'
)
"""

QUERY_ADD_CONSTRAINTS_CIDADE = """
ALTER TABLE cidade
  ADD CONSTRAINT cidade_pkey PRIMARY KEY (id),
  ADD CONSTRAINT cidade_nome_uf_key UNIQUE (nome, uf),
  ALTER COLUMN nome SET NOT NULL,
  ALTER COLUMN uf SET NOT NULL
"""

def getQuery_insert_cidade(nome:str, uf:str):
    return QUERY_INSERT_CIDADE.format(nome=nome, uf=uf)

def getQuery_add_constraints_cidade():
    return QUERY_ADD_CONSTRAINTS_CIDADE


class Conexao(object):
    _dbconn=None
    _cursor=None

    def __init__(self, mhost, db, usr, pwd, isTemporary=False):
        try:
            self._dbconn = psycopg2.connect(host=mhost, database=db, user=usr,  password=pwd)
            if isTemporary: # ou seja, a conexão vai ser fechada em breve
                self._dbconn.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)

            self._cursor = self._dbconn.cursor(cursor_factory=DictCursor)
        except psycopg2.Error as err:
            sys.exit("[ERROR_CONNECTION]", err)

    def manipular(self, query, varsx=None, efetive=False, exitOnError=False):
        try:
            cur = self._cursor
            cur.execute(query, varsx)
            if efetive: self._dbconn.commit()
        except psycopg2.Error as err:
            if err.pgcode == '42P04': return True # erro database already exists

            if exitOnError: sys.exit(err)
            print('[ERROR_TO_RUN]', cur.query, '::', err)
            self._dbconn.rollback()
            return False
        return True

    def consultar(self, query, varsx=None, fetch_all=True):
        rs=None
        try:
            cur = self._cursor
            cur.execute(query, varsx)
            rs = (cur.fetchone, cur.fetchall)[fetch_all]()
        except psycopg2.Error as err:
            self._dbconn.rollback()
            print('[ERROR_TO_RUN]', cur.query, '::', err)
            return None
        return rs

    def proximaPK(self, tabela, chave):
        sql= 'SELECT MAX('+chave+') FROM '+ tabela
        rs = self.consultar(sql, None, False)
        return None if (rs == None or rs[0] == None) else rs[0]+1

    def fechar(self):
        self._cursor.close()
        self._dbconn.close()


def main():

    ## iniciar conexão temporária para criar um novo database
    con = Conexao(DATABASE_CONFIGS.host, DATABASE_CONFIGS.root, DATABASE_CONFIGS.user, DATABASE_CONFIGS.password, True)
    if not con.manipular("CREATE DATABASE %s" % DATABASE_CONFIGS.name): sys.exit()
    else: con.fechar()

    ## iniciar conexão real utilizando o database criado (acima)
    con = Conexao(DATABASE_CONFIGS.host, DATABASE_CONFIGS.name, DATABASE_CONFIGS.user, DATABASE_CONFIGS.password, False)

    query_create_cidade = "CREATE TABLE IF NOT EXISTS cidade (id SERIAL, nome VARCHAR(100), uf VARCHAR(2))"
    con.manipular(query_create_cidade, None, True, True)

    # print('proxima PK:', con.proximaPK('cidade', 'id'))
    con.manipular( getQuery_insert_cidade('São Paulo', 'SP'), None, True )
    # con.manipular( getQuery_add_constraints_cidade(), None, True )

    recset = con.consultar('SELECT * FROM cidade')
    if recset:
        for rec in recset:
            print(rec['id'], rec['nome'], rec['uf'], sep=' :: ')


    con.fechar()


if __name__ == "__main__":
    print('------ início -------')
    main()
    print('-------- fim --------')
