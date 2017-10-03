#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi and Victor Meireles on 2017-10-03
#


class Similar:
    """ """
    def __init__(self, asin:str, asin_similar:str):
        self.asin = asin
        self.asin_similar = asin_similar

    def __str__(self):
        return "ASIN:'{asin}', asin_similar:'{asin_similar}'".format(**self.__dict__)

    __repr__ = __str__


class SimilarDAO:
    """ """
    def __init__(self, dbconnection):
        self.dbconnection = dbconnection
