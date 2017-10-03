#
#   Copyright (c) 2017. All rights reserved.
#   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#   Created by Micael Levi and Victor Meireles on 2017-10-03
#


class Category:
    """ """
    def __init__(self, cid:int, cname:str, super_cid:int=None):
        self.cid = cid
        self.cname = cname
        self.super_cid = super_cid

    def __str__(self):
        return "cid:{cid}, cname:'{cname}', super_cid:{super_cid}".format(**self.__dict__)

    __repr__ = __str__


class CategoryDAO:
    """ """
    def __init__(self, dbconnection):
        self.dbconnection = dbconnection
