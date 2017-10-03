#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi and Victor Meireles on 2017-10-03
#


class ProductCategory:
    """ """
    def __init__(self, asin:str, leaf_cid:str):
        self.asin = asin
        self.leaf_cid = leaf_cid

    def __str__(self):
        return "ASIN:'{asin}', leaf_cid:'{leaf_cid}'".format(**self.__dict__)

    __repr__ = __str__


class ProductCategoryDAO:
    """ """
    def __init__(self, dbconnection):
        self.dbconnection = dbconnection
