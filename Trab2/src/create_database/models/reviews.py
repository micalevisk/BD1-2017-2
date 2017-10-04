#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi on 2017-10-03
#

from psycopg2 import sql


class Review:
    """ """
    def __init__(self, asin:str, time:str=None, cutomer:str=None, rating:int=0, votes:int=0, helpful:int=0):
        self.asin = asin
        self.time = time
        self.cutomer = cutomer
        self.rating = rating
        self.votes = votes
        self.helpful = helpful

    def __str__(self):
        return "ASIN:'{asin}', time:'{time}', cutomer:'{cutomer}', rating:{rating}, votes:{votes}, helpful:{helpful}".format(**self.__dict__)

    __repr__ = __str__



QUERY_CREATE_TABLE_REVIEWS = """
CREATE TABLE IF NOT EXISTS reviews
(
  asin VARCHAR(10),
  time VARCHAR(10) NOT NULL,
  cutomer VARCHAR(15) NOT NULL,
  rating INT NOT NULL,
  votes INT NOT NULL,
  helpful INT NOT NULL
);
"""

QUERY_ADD_CONSTRAINTS_REVIEWS = """
ALTER TABLE reviews
  ADD CONSTRAINT reviews_asin_cutomer_key UNIQUE (asin, cutomer),
  ADD CONSTRAINT reviews_products_asin_fk FOREIGN KEY (asin) REFERENCES products (asin) ON DELETE CASCADE;
"""

QUERY_TEMPLATE_INSERT_REVIEWS = """
INSERT INTO reviews (asin, time, cutomer, rating, votes, helpful)
SELECT %s, %s, %s, {0}, {1}, {2}
WHERE NOT EXISTS (
  SELECT asin FROM reviews
  WHERE
    asin = %s AND cutomer = %s
);
"""


class ReviewDAO:
    """ """
    def __init__(self, dbconnection):
        self._db = dbconnection

    def createTable(self, doCommit=False):
        return self._db.manipular(QUERY_CREATE_TABLE_REVIEWS, None, doCommit, True)

    def addConstraints(self, doCommit=False):
        return self._db.manipular(QUERY_ADD_CONSTRAINTS_REVIEWS , None, doCommit)

    def insert(self, review:Review, doCommit=False):
        queryTemplate = sql.SQL(QUERY_TEMPLATE_INSERT_REVIEWS).format( sql.Literal(review.rating), sql.Literal(review.votes), sql.Literal(review.helpful) )
        return self._db.manipular(queryTemplate, [
            review.asin,
            review.time,
            review.cutomer,

            review.asin,
            review.cutomer,
        ], doCommit, True)
