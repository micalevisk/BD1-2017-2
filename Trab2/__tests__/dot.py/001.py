##:description: usando o psycopg2 (c) http://www.devmedia.com.br/como-criar-uma-conexao-em-postgresql-com-python/34079
##:run: python3 001.py

import sys
import psycopg2 # precisa ser instalado



class Conexao(object):
    _dbconn=None
    _cursor=None

    def __init__(self, mhost, db, usr, pwd):
        try:
            self._dbconn = psycopg2.connect(host=mhost, database=db, user=usr,  password=pwd)
            self._cursor = self._dbconn.cursor()
        except:
            sys.exit("[ERROR_CONNECTION]")

    def manipular(self, query, varsx=None, efetive=True):
        try:
            cur = self._cursor
            cur.execute(query, varsx)
            if efetive: self._dbconn.commit() # encerrar transação pode causar lentidão
        except psycopg2.Error as e:
            print('[ERROR_TO_RUN]:', cur.query, e)
            self._dbconn.rollback()
            return False

        return True

    def consultar(self, query, varsx=None, fetch_all=True):
        rs=None
        try:
            cur = self._cursor
            cur.execute(query, varsx)
            rs = (cur.fetchone, cur.fetchall)[fetch_all]()
        except psycopg2.Error as e:
            self._dbconn.rollback()
            print('[ERROR_TO_RUN]:', cur.query, e)
            return None

        return rs

    def proximaPK(self, tabela, chave):
        sql= 'SELECT MAX('+chave+') FROM '+ tabela
        rs = self.consultar(sql)
        return rs if (rs == None) else rs[0][0]+1

    def fechar(self):
        self._cursor.close()
        self._dbconn.close()


def main():
    con = Conexao('localhost', 'postgres', 'bd1', 'bd1')

    # query_create_cidade = "CREATE TABLE IF NOT EXISTS cidade (id SERIAL PRIMARY KEY, nome VARCHAR(100) UNIQUE NOT NULL, uf VARCHAR(2) NOT NULL)"
    query_create_cidade = "CREATE TABLE IF NOT EXISTS cidade (id SERIAL, nome VARCHAR(100), uf VARCHAR(2))"
    if con.manipular(query_create_cidade) :
        print('tabela criada/existe com sucesso!')

    '''
    query_insert_cidade = "INSERT INTO cidade (id, nome, uf) VALUES (DEFAULT, %s, %s)"
    if con.manipular(query_insert_cidade, ('Manaus', 'AM',)) :
        print('cidade inserida com sucesso!')
    '''

    print('proxima PK:', con.proximaPK('cidade', 'id'))
    recset = con.consultar('SELECT * FROM cidade')
    if recset:
        for rec in recset:
            print(rec)


    con.fechar()


if __name__ == "__main__":
    main()
    print('------ fim ------')
