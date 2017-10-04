#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi 2017-10-03
#

from psycopg2 import sql


class ProductCategory:
    """ """
    def __init__(self, asin:str, leaf_cid:str):
        self.asin = asin
        self.leaf_cid = leaf_cid

    def __str__(self):
        return "ASIN:'{asin}', leaf_cid:'{leaf_cid}'".format(**self.__dict__)

    __repr__ = __str__



QUERY_CREATE_TABLE_PRODUCT_CATEGORIES = """
CREATE TABLE IF NOT EXISTS product_categories
(
  asin VARCHAR(10) NOT NULL,
  leaf_cid INT NOT NULL
);
"""

QUERY_ADD_CONSTRAINTS_PRODUCT_CATEGORIES = """
ALTER TABLE product_categories
  ADD CONSTRAINT reviews_asin_leaf_cid_key UNIQUE (asin, leaf_cid),
  ADD CONSTRAINT product_categories_products_asin_fk FOREIGN KEY (asin) REFERENCES products (asin) ON DELETE CASCADE,
  ADD CONSTRAINT product_categories_categories_cid_fk FOREIGN KEY (leaf_cid) REFERENCES categories (cid) ON DELETE CASCADE;
"""

QUERY_TEMPLATE_INSERT_PRODUCT_CATEGORIES = """
INSERT INTO product_categories (asin, leaf_cid)
SELECT %s, {0}
WHERE NOT EXISTS (
  SELECT asin FROM product_categories
  WHERE
    asin = %s AND leaf_cid = {0}
);
"""


class ProductCategoryDAO:
    """ """
    def __init__(self, dbconnection):
        self._db = dbconnection

    def createTable(self, doCommit=False):
        return self._db.manipular(QUERY_CREATE_TABLE_PRODUCT_CATEGORIES, None, doCommit, True)

    def addConstraints(self, doCommit=False):
        return self._db.manipular(QUERY_ADD_CONSTRAINTS_PRODUCT_CATEGORIES , None, doCommit)

    def insert(self, pcategory:ProductCategory, doCommit=False):
        queryTemplate = sql.SQL(QUERY_TEMPLATE_INSERT_PRODUCT_CATEGORIES).format( sql.Literal(pcategory.leaf_cid) )
        return self._db.manipular(queryTemplate, [
            pcategory.asin,

            pcategory.asin
        ], doCommit, True)
