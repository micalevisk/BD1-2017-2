#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi on 2017-10-03
#

from psycopg2 import sql


class Product:
    """ Representar um registro da relação 'products' """
    def __init__(self, id:int, asin:str, ## required
                title:str=None, name_group:str=None, salesrank:int=None, count_similars:int=0, count_categories:int=0, count_total_reviews:int=0):
        self.id = id
        self.asin = asin
        self.title = title
        self.name_group = name_group
        self.salesrank = salesrank
        self.count_similars = count_similars
        self.count_categories = count_categories
        self.count_total_reviews = count_total_reviews

    def __str__(self):
        return "Id:{id}, ASIN:'{asin}', title:'{title}', group:'{name_group}', salesrank:{salesrank}, similar:{count_similars}, categories:{count_categories}, totalreviews:{count_total_reviews}".format(**self.__dict__)

    __repr__ = __str__



QUERY_CREATE_TABLE_PRODUCTS = """
CREATE TABLE IF NOT EXISTS products
(
  id INT NOT NULL,
  asin VARCHAR(10),
  title VARCHAR(500),
  name_group VARCHAR(5),
  salesrank INT,
  count_similars INT DEFAULT 0,
  count_categories INT DEFAULT 0,
  count_total_reviews INT DEFAULT 0
);
"""

QUERY_ADD_CONSTRAINTS_PRODUCTS = """
ALTER TABLE products
  ADD CONSTRAINT products_id UNIQUE (id),
  ADD CONSTRAINT products_pkey PRIMARY KEY (asin);
"""

QUERY_TEMPLATE_INSERT_PRODUCT = """
INSERT INTO products (id, asin, title, name_group, salesrank, count_similars, count_categories, count_total_reviews)
SELECT {0}, %s, %s, %s, %s, %s, %s, %s
WHERE NOT EXISTS (
  SELECT asin FROM products
  WHERE
    id = {0} AND asin = %s
);
"""


class ProductDAO:
    """ Manipular um registro na relação 'products' """
    def __init__(self, dbconnection):
        self._db = dbconnection

    def createTable(self, doCommit=False):
        return self._db.manipular(QUERY_CREATE_TABLE_PRODUCTS, None, doCommit, True)

    def addConstraints(self, doCommit=False):
        return self._db.manipular(QUERY_ADD_CONSTRAINTS_PRODUCTS, None, doCommit)

    def insert(self, product:Product, doCommit=False):
        queryTemplate = sql.SQL(QUERY_TEMPLATE_INSERT_PRODUCT).format( sql.Literal(product.id) )
        return self._db.manipular(queryTemplate, [
            product.asin,
            product.title,
            product.name_group,
            product.salesrank,
            product.count_similars,
            product.count_categories,
            product.count_total_reviews,

            product.asin
        ], doCommit, True)
