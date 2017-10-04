#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi on 2017-10-03
#

from psycopg2 import sql


class Similar:
    """ """
    def __init__(self, asin:str, asin_similar:str):
        self.asin = asin
        self.asin_similar = asin_similar

    def __str__(self):
        return "ASIN:'{asin}', asin_similar:'{asin_similar}'".format(**self.__dict__)

    __repr__ = __str__



QUERY_CREATE_TABLE_SIMILARS = """
CREATE TABLE IF NOT EXISTS similars
(
  asin VARCHAR(10),
  asin_similar VARCHAR(10) NOT NULL
);
"""

QUERY_ADD_CONSTRAINTS_SIMILARS = """
ALTER TABLE similars
  ADD CONSTRAINT reviews_asin_asin_similar_key UNIQUE (asin, asin_similar),
  ADD CONSTRAINT similars_products_asin_fk FOREIGN KEY (asin) REFERENCES products (asin) ON DELETE CASCADE;
"""

QUERY_TEMPLATE_INSERT_SIMILAR = """
INSERT INTO similars (asin, asin_similar)
SELECT %s, %s
WHERE NOT EXISTS (
  SELECT asin FROM similars
  WHERE
    asin = %s AND asin_similar = %s
);
"""


class SimilarDAO:
    """ """
    def __init__(self, dbconnection):
        self._db = dbconnection

    def createTable(self, doCommit=False):
        return self._db.manipular(QUERY_CREATE_TABLE_SIMILARS, None, doCommit, True)

    def addConstraints(self, doCommit=False):
        return self._db.manipular(QUERY_ADD_CONSTRAINTS_SIMILARS , None, doCommit)

    def insert(self, similar:Similar, doCommit=False):
        return self._db.manipular(QUERY_TEMPLATE_INSERT_SIMILAR, [
            similar.asin,
            similar.asin_similar,

            similar.asin,
            similar.asin_similar
        ], doCommit, True)
