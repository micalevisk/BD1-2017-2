#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi on 2017-10-03
#

import sys
import psycopg2
from psycopg2.extras import DictCursor


class DBConnection(object):
    _dbconn=None
    _cursor=None

    def __init__(self, mhost, db, usr, pwd):
        try:
            self._dbconn = psycopg2.connect(host=mhost, database=db, user=usr,  password=pwd)
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


    def doCommit(self):
        return self._dbconn.commit()


    def fechar(self):
        self._cursor.close()
        self._dbconn.close()
