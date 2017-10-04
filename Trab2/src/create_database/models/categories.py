#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi on 2017-10-03
#

from psycopg2 import sql


class Category:
    """ """
    def __init__(self, cid:int, cname:str, super_cid:int=None):
        self.cid = cid
        self.cname = cname
        self.super_cid = super_cid

    def __str__(self):
        return "cid:{cid}, cname:'{cname}', super_cid:{super_cid}".format(**self.__dict__)

    __repr__ = __str__



QUERY_CREATE_TABLE_CATEGORIES = """
CREATE TABLE IF NOT EXISTS categories
(
  cid INT,
  cname VARCHAR(500),
  super_cid INT
);
"""

QUERY_ADD_CONSTRAINTS_CATEGORIES = """
ALTER TABLE categories
  ADD CONSTRAINT categories_pkey PRIMARY KEY (cid);
"""

QUERY_TEMPLATE_INSERT_CATEGORIES = """
INSERT INTO categories (cid, cname, super_cid)
SELECT {0}, %s, %s
WHERE NOT EXISTS (
  SELECT cid FROM categories
  WHERE
    cid = {0}
);
"""


class CategoryDAO:
    """ """
    def __init__(self, dbconnection):
        self._db = dbconnection

    def createTable(self, doCommit=False):
        return self._db.manipular(QUERY_CREATE_TABLE_CATEGORIES, None, doCommit, True)

    def addConstraints(self, doCommit=False):
        return self._db.manipular(QUERY_ADD_CONSTRAINTS_CATEGORIES , None, doCommit)

    def insert(self, category:Category, doCommit=False):
        queryTemplate = sql.SQL(QUERY_TEMPLATE_INSERT_CATEGORIES).format( sql.Literal(category.cid) )
        return self._db.manipular(queryTemplate, [
            category.cname,
            category.super_cid
        ], doCommit, True)
